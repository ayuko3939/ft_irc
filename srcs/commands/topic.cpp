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

static std::string getChannelNameFromWord(std::string &word)
{
	if (word[0] == '#')
		return (word.substr(1));
	return (word);
}

static bool isValid(const std::string &topic)
{
	if (topic.size() > 50)
		return (false);

	for (std::size_t i = 0; i < topic.size(); i++)
	{
		if (!isalnum(topic[i]))
			return (false);
	}
	return (true);
}

// パラメータ検証：<channel>は必須、<topic>はオプション
static bool checkArguments(Server *server, int clientFd, 
	std::vector<std::string> &words, std::string &channelName, std::string &topic)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (words.empty() || words[0].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "TOPIC");
		errMessage += TOPIC_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	// 引数が複数ある場合
	if (words.size() > 2)
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += TOPIC_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	
	// チャンネル名とトピックを取得
	channelName = getChannelNameFromWord(words[0]);
	if (words.size() > 1)
		topic = words[1];
	else
		return (true);

	// トピックの妥当性をチェック
	if (!isValid(topic))
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += TOPIC_REQUIREMENTS;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}
	
static void broadcastNewTopic(Server *server, Channel &channel, Client &client, std::string topic)
{
	std::string notice = RPL_TOPIC(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), topic);
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
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	bool isDisplayTopic = (words.size() == 1);
	
	// 2. 引数のチェック
	if (!checkArguments(server, clientFd, words, channelName, topic))
		return;

	// 3. サーバーのチャンネルリストから対象チャンネルを検索
	std::map<std::string, Channel> &channelList = server->getChannelList();
	std::map<std::string, Channel>::iterator chanIt = channelList.find(channelName);
	Channel &channel = chanIt->second;

	if (chanIt == channelList.end())
	{
		errMessage = ERR_NOSUCHCHANNEL(clientNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	// 4. クライアントが対象チャンネルに参加しているか確認
	std::map<const int, Client> clientList = channel.getClientList();
	if (clientList.find(clientFd) == clientList.end())
	{
		errMessage = ERR_NOTONCHANNEL(clientNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	// 5-1. 表示要求の場合、トピックを返す
	if (isDisplayTopic)
	{
		std::string currentTopic = channel.getTopic();
		if (currentTopic.empty())
		{
			errMessage = RPL_NOTOPIC(clientNick, channelName);
			addToClientSendBuf(server, clientFd, errMessage);
		}
		else
		{
			errMessage = RPL_TOPIC(IRC_PREFIX(clientNick, client.getUserName()), channelName, currentTopic);
			addToClientSendBuf(server, clientFd, errMessage);
		}
		return;
	}
	// 5-2. 変更要求の場合、トピック変更処理を実行
	else
	{
		// 6-1. 空文字ならトピックをクリア
		if (topic.empty())
		{
			channel.setTopic("");
		}
		// 6-2. それ以外は、保護されたトピックモードの場合、オペレーター権限をチェックする
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

		// 7. チャンネル内全クライアントに、TOPICコマンドとして変更通知をブロードキャスト
		broadcastNewTopic(server, channel, client, topic);
	}
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	RPL_NOTOPIC (331)
	RPL_TOPIC (332)
	// RPL_TOPICWHOTIME (333)
*/