/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 22:13:36 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// <channel>は必須、<topic>はオプション
static bool checkAndGetArguments(Server *server, int clientFd,
	std::string &argument, std::string &channelName, std::string &topic)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (argument.empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "TOPIC");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE TOPIC_USAGE END << std::endl;
		return (false);
	}
	
	// チャンネル名とトピックを取得
	getTargetAndText(argument, channelName, topic);

	// トピックが長すぎる場合、切り詰めて末尾に"[CUT]"を追加
	if (topic.size() > TOPICLEN)
	{
		topic = topic.substr(0, PARTLEN);
		topic += "[CUT]";
	}
	return (true);
}
	
static void broadcastNewTopic(Server *server, Channel &channel, 
	std::string &nickname, std::string &userName, std::string topic)
{
	std::string notice = RPL_TOPICSET(IRC_PREFIX(nickname, userName), channel.getName(), topic);
	std::map<const int, Client> &clientList = channel.getClientList();

	// チャンネルメンバー全員に新しいメンバーの参加を通知
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notice);
	}
}

// TOPIC <channel> [<topic>]
void topic(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = server->getNickname(clientFd);
	std::string errMessage = "";
	std::string channelName = "";
	std::string topic = "";

	// 1. ユーザー入力をスペース区切りで取得し、引数の数をチェック
	if (!checkAndGetArguments(server, clientFd, cmdInfo.message, channelName, topic))
		return ;

	// 2. サーバーのチャンネルリストから対象チャンネルを検索
	std::map<std::string, Channel> &channelList = server->getChannelList();
	std::map<std::string, Channel>::iterator chanIt = channelList.find(channelName);
	Channel &channel = chanIt->second;

	if (chanIt == channelList.end())
	{
		errMessage = ERR_NOSUCHCHANNEL(clientNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	// 3. クライアントが対象チャンネルに参加しているか確認
	std::map<const int, Client> clientList = channel.getClientList();
	if (clientList.find(clientFd) == clientList.end())
	{
		errMessage = ERR_NOTONCHANNEL(clientNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	// 4-1. 表示要求の場合、トピックを返す
	if (topic.empty())
	{
		std::string currentTopic = channel.getTopic();
		if (currentTopic.empty())
		{
			errMessage = RPL_NOTOPIC(clientNick, channelName);
			addToClientSendBuf(server, clientFd, errMessage);
		}
		else
		{
			errMessage = RPL_TOPIC(clientNick, channelName, currentTopic);
			addToClientSendBuf(server, clientFd, errMessage);
		}
		return;
	}
	// 4-2. 変更要求の場合、トピック変更処理を実行
	else
	{
		// 5-1. 空文字ならトピックをクリア
		if (topic.empty())
		{
			channel.setTopic("");
		}
		// 5-2. それ以外は、保護されたトピックモードの場合、オペレーター権限をチェックする
		else
		{
			// 保護されたトピックモード (mode "t") チェック
			if (channel.getMode("topic") && !channel.isOperator(clientFd))
			{
				errMessage = ERR_CHANOPRIVSNEEDED(clientNick, channelName);
				addToClientSendBuf(server, clientFd, errMessage);
				return;
			}
			// 新しいトピックを設定
			channel.setTopic(topic);
		}

		// 6. チャンネル内全クライアントに、TOPICコマンドとして変更通知をブロードキャスト
		broadcastNewTopic(server, channel, clientNick, client.getUserName(), topic);
	}
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	RPL_NOTOPIC (331)
	// RPL_TOPIC (332)
	// RPL_TOPICWHOTIME (333)
*/