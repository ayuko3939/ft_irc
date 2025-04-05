/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/29 19:21:52 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// PART <channel>は必須、<reason>はオプション
static bool checkAndGetArguments(Server *server, int clientFd, 
	std::string &argument, std::string &channelName, std::string &reason)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (argument.empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "PART");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE PASS_USAGE END << std::endl;
		return (false);
	}
	
	// チャンネル名と理由を取得
	getTargetAndText(argument, channelName, reason);

	// 理由が長すぎる場合、切り詰めて末尾に"[CUT]"を追加
	if (reason.size() > PARTLEN)
	{
		reason = reason.substr(0, PARTLEN);
		reason += "[CUT]";
	}
	return (true);
}

static bool checkPartEligibility(Server *server, int clientFd, 
	std::string &channelName)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// チャンネルが存在するか確認
	if (!server->isChannelExist(channelName))
	{
		errMessage = ERR_NOSUCHCHANNEL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;
	
	// クライアントがそのチャンネルに参加しているか確認
	if (!channel.isClientInChannel(clientFd))
	{
		errMessage = ERR_NOTONCHANNEL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

/*
  :nickname!username@localhost PART #channelName
  :nickname!username@localhost PART #channelName :the reason is 42.
*/
static void broadcastPart(Server *server, int const clientFd, 
	Channel &channel, std::string &channelName, std::string &reason)
{
	Client &client = retrieveClient(server, clientFd);
	std::string nickname = client.getNickname();

	std::string notice = RPL_PART(IRC_PREFIX(nickname, client.getUserName()), channelName, reason);

	addToClientSendBuf(server, clientFd, notice);

	std::map<const int, Client>	&clientList = channel.getClientList();
	for (std::map<const int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->first, notice);
	}
}

// PARTコマンドの処理
// コマンド形式: PART <channel> [<reason>]
void part(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	std::string channelName = "";
	std::string reason = "";

	// 1. ユーザー入力から引数を取得し、引数をチェック
	if (!checkAndGetArguments(server, clientFd, cmdInfo.message, channelName, reason))
		return ;

	// 2. パラメータの妥当性を確認する
	if (!checkPartEligibility(server, clientFd, channelName))
		return ;
		
	// 3. クライアントをチャンネルから削除する
	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;
	channel.removeClient(clientFd);

	// 4. 退室するユーザーに成功通知の送信、チャンネル内全メンバーに退室通知を送信する
	broadcastPart(server, clientFd, channel, channelName, reason);
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
*/