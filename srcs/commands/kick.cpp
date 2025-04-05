/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/29 19:17:03 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// <channel>は必須、<comment>はオプション
static bool checkAndGetArguments(Server *server, int clientFd, std::string &issuerNick, 
	std::string &argument, std::string &channelName, std::string &targetNick, std::string &comment)
{
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (argument.empty() || argument.find(' ') == std::string::npos)
	{
		errMessage = ERR_NEEDMOREPARAMS(issuerNick, "KICK");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE KICK_USAGE END << std::endl;
		return (false);
	}

	// チャンネル名とユーザー名とコメントを取得
	size_t pos = argument.find(' ');
	channelName = argument.substr(0, pos);
	channelName = getChannelNameFromWord(channelName);
	argument.erase(0, pos + 1);
	getTargetAndText(argument, targetNick, comment);

	// コメントが空の場合、デフォルトのコメントを設定、長すぎる場合、切り詰めて末尾に"[CUT]"を追加
	if (comment.empty())
	{
		comment = DEFAULT_KICK_COMMENT;
	}
	else if (comment.size() > KICKLEN)
	{
		comment = comment.substr(0, KICKLEN);
		comment += "[CUT]";
	}
	return (true);
}

static bool checkKickEligibility(Server *server, int const clientFd, 
	std::string &issuerNick, std::string &channelName, std::string &targetNick)
{
	std::string errMessage = "";

	// 1. 対象チャンネルが存在することを確認
	if (!server->isChannelExist(channelName))
	{
		errMessage = ERR_NOSUCHCHANNEL(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 対象クライアントが存在することを確認
	if (!server->isClientExist(targetNick))
	{
		errMessage = ERR_NOSUCHNICK(issuerNick, targetNick);
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(issuerNick, targetNick));
		return (false);
	}

	// 発行者が対象チャンネルに参加していることを確認
	if (!channel.isClientInChannel(clientFd))
	{
		errMessage = ERR_NOTONCHANNEL(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 対象ユーザーがチャンネルに参加していることを確認
	int targetFd = server->getClientFdByNick(targetNick);
	if (!channel.isClientInChannel(targetFd))
	{
		errMessage = ERR_USERNOTINCHANNEL(issuerNick, targetNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 発行者がチャンネルオペレーターであるか確認
	if (!channel.isOperator(clientFd))
	{
		errMessage = ERR_CHANOPRIVSNEEDED(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// // InviterとTargetが同一でないことを確認
	// if (issuerNick == targetNick)
	// {
	// 	errMessage = ERR_ERRONEUSTARGET(issuerNick, targetNick, " (Inviter and target are the same)");
	// 	addToClientSendBuf(server, clientFd, errMessage);
	// 	return (false);
	// }
	return (true);
}

/*
  :nickname!username@localhost KICK #channelName target comment
*/
static void broadcastKick(Server *server, Channel &channel, Client &client, 
	std::string &issuerNick, std::string &target, std::string &comment)
{
	// チャンネルメンバー全員に追放を通知
	std::map<const int, Client> &clientList = channel.getClientList();
	std::string notice = RPL_KICK(IRC_PREFIX(issuerNick, client.getUserName()), channel.getName(), target, comment);

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notice);
	}
}

// コマンド形式: KICK <channel> <target> [<comment>]
void kick(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();
	std::string channelName = "";
	std::string targetNick = "";
	std::string comment = "";

	// 1. ユーザー入力から引数を取得し、引数をチェック
	if (!checkAndGetArguments(server, clientFd, issuerNick, cmdInfo.message, channelName, targetNick, comment))
		return ;

	// 2. パラメータの妥当性を確認する
	if (!checkKickEligibility(server, clientFd, issuerNick, channelName, targetNick))
		return ;

	// 3. チャンネルメンバー全員に追放を通知
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(channelName)->second;
	broadcastKick(server, channel, client, issuerNick, targetNick, comment);
	
	// 4. 対象ユーザーをチャンネルから削除する
	int targetFd = server->getClientFdByNick(targetNick);
	channel.removeClient(targetFd);
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_CHANOPRIVSNEEDED (482)
	ERR_USERNOTINCHANNEL (441)
	ERR_NOTONCHANNEL (442)
Deprecated Numeric Reply:
	// ERR_BADCHANMASK (476)
*/