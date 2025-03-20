/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/20 20:55:23 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// PARTコマンドの引数数チェック (<channel> [<reason>])
static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
	// 少なくともチャンネル名が必要。理由は任意なので、1個または2個でなければエラー
	if (words.size() < 1 || words.size() > 2)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(PART_USAGE));
		return false;
	}
	return true;
}

// PARTコマンドの処理
// コマンド形式: PART <channel> [<reason>]
void part(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return;

	// 2. 発行者のクライアント情報を取得
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();

	// 3. パラメータから対象のチャンネル名と（あれば）理由を取得
	std::string channelName = words[0];
	std::string reason = (words.size() == 2 ? words[1] : "");

	// 4. チャンネルが存在するか確認
	if (!server->isChannelExist(channelName))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(clientNick, channelName));
		return;
	}

	// 5. チャンネルオブジェクトの取得
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(channelName)->second;

	// 6. クライアントがそのチャンネルに参加しているか確認
	if (!channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(clientNick, channelName));
		return;
	}

	// // 7. チャンネル内の唯一のオペレーターである場合、PARTを拒否
	// if (channel.isOperator(clientFd))
	// {
	// 	std::vector<int> opList = channel.getOperatorList();
	// 	if (opList.size() == 1)
	// 	{
	// 		std::string msg = ":" + clientNick + " :You are the only operator in " + channelName + ", you cannot part.\r\n";
	// 		addToClientSendBuf(server, clientFd, msg);
	// 		return;
	// 	}
	// }

	// 8. PARTメッセージを作成し、チャンネル内全メンバーに通知
	std::string partMsg = ":" + clientNick + " PART " + channelName;
	if (!reason.empty())
		partMsg += " :" + reason;
	partMsg += "\r\n";

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), partMsg);
	}

	// 9. クライアントをチャンネルから削除する
	channel.removeClient(clientFd);
}
