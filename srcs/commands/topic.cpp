/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 21:31:14 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// パラメータ検証：<channel>は必須、<topic>はオプション
static bool checkTopicArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
	// 単一引数の場合は表示要求、2引数の場合は変更要求
	if (words.size() < 1 || words.size() > 2)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(TOPIC_USAGE));
		return false;
	}
	return true;
}

static void broadcastNewTopic(Server *server, Channel &channel, const std::string clientNick, std::string topic)
{
	// チャンネルメンバー全員に新しいメンバーの参加を通知
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), RPL_TOPI(clientNick, channel.getName(), topic));
	}
}

// TOPIC <channel> [<topic>]
void topic(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータの分割と検証
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkTopicArguments(server, clientFd, words))
		return;

	// 2. チャンネル名とクライアント情報の取得
	std::string channelName = words[0];
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();

	// 3. サーバーのチャンネルリストから対象チャンネルを検索
	std::map<std::string, Channel> &channels = server->getChannelList();
	std::map<std::string, Channel>::iterator chanIt = channels.find(channelName);
	Channel &channel = chanIt->second;
	if (chanIt == channels.end())
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(clientNick, channelName));
		return;
	}

	// 4. クライアントが対象チャンネルに参加しているかチェック
	if (channel.getClientList().find(clientFd) == channel.getClientList().end())
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(clientNick, channelName));
		return;
	}

	// 5-1. 表示要求の場合、トピックを返す
	if (words.size() == 1)
	{
		std::string currentTopic = channel.getTopic();
		if (currentTopic.empty())
			addToClientSendBuf(server, clientFd, RPL_NOTOPIC(clientNick, channelName));
		else
			addToClientSendBuf(server, clientFd, RPL_TOPIC(clientNick, channelName, currentTopic));
		return;
	}
	// 5-2. 変更要求の場合、トピック変更処理を実行
	else
	{
		std::string newTopic = words[1];

		// 6-1. 空文字ならトピックをクリア
		if (newTopic.empty())
		{
			channel.setTopic("");
		}
		// 6-2. それ以外は、保護されたトピックモードの場合、オペレーター権限をチェックする
		else
		{
			// 保護されたトピックモード (mode "t") チェック
			if (channel.getMode("topic") && !channel.isOperator(clientFd))
			{
				addToClientSendBuf(server, clientFd, ERR_CHANOPRIVSNEEDED(clientNick, channelName));
				return;
			}
			// 新しいトピックを設定
			channel.setTopic(newTopic);
		}

		// 7. チャンネル内全クライアントに、TOPICコマンドとして変更通知をブロードキャスト
		broadcastNewTopic(server, channel, clientNick, newTopic);
	}
}
