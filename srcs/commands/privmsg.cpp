/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/29 19:20:51 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// <target> <text to be sent> ともに必須
static bool checkArguments(Server *server, int clientFd, 
	std::string &nickname, std::string &target, std::string &text)
{
	std::string errMessage = "";

	if (target.empty())
	{
		errMessage = ERR_NORECIPIENT(nickname, "PRIVMSG");
		errMessage += PRIVMSG_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	if (text.empty())
	{
		errMessage = ERR_NOTEXTTOSEND(nickname);
		errMessage += PRIVMSG_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	if (text.size() > MSGLEN)
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += PRIVMSG_REQUIREMENTS;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}

static void broadcastMsg(Server *server, Channel &channel, Client &client, std::string &target, std::string &text)
{
	std::string message = RPL_PRIVMSG(IRC_PREFIX(client.getNickname(), client.getUserName()), '#' + target, text);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		if (it->second.getNickname() != client.getNickname())
			addToClientSendBuf(server, it->second.getClientFd(), message);
	}
}

// PRIVMSG <target> <text to be sent>
void privmsg(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string senderNick = client.getNickname();
	std::string target = "";
	std::string text = "";
	std::string errMessage = "";

	// 1. ターゲットとメッセージを取得（最初のスペースで区切り、以降はまとめて１つのテキストとする）
	getTargetAndText(cmdInfo.message, target, text);

	// 2. 入力パラメータをチェック
	if (!checkArguments(server, clientFd, senderNick, target, text))
		return ;

	// 3-1. ターゲットがチャンネルの場合
	if (cmdInfo.message[0] == '#')
	{
		// 4. ターゲットが存在するか確認
		if (!server->isChannelExist(target))
		{
			errMessage = ERR_NOSUCHCHANNEL(senderNick, target);
			addToClientSendBuf(server, clientFd, errMessage);
			return ;
		}

		std::map<std::string, Channel> &channels = server->getChannelList();
		Channel &channel = channels.find(target)->second;

		// 5. 送信者がそのチャンネルに参加しているか確認
		if (!channel.isClientInChannel(clientFd))
		{
			errMessage = ERR_NOTONCHANNEL(senderNick, target);
			addToClientSendBuf(server, clientFd, errMessage);
			return ;
		}

		// 6. チャンネル内の全メンバー（送信元以外）に対して、メッセージをブロードキャスト
		broadcastMsg(server, channel, client, target, text);
	}
	// 3-2. ターゲットがユーザーの場合
	else
	{
		// 4. ターゲットが存在するか確認
		if (!server->isClientExist(target))
		{
			errMessage = ERR_NOSUCHNICK(senderNick, target);
			addToClientSendBuf(server, clientFd, errMessage);
			return ;
		}
		
		// 5. ターゲットにメッセージを送信
		int targetFd = server->getClientFdByNick(target);
		std::string message = senderNick + " PRIVMSG " + target + " " + text + "\r\n";
		addToClientSendBuf(server, targetFd, message);
	}
}

/*
Numeric Replies:
	ERR_NOSUCHNICK (401)
	// ERR_NOSUCHSERVER (402)
	// ERR_CANNOTSENDTOCHAN (404)
	// ERR_TOOMANYTARGETS (407) //
	ERR_NORECIPIENT (411)
	ERR_NOTEXTTOSEND (412)
	// ERR_NOTOPLEVEL (413) //
	// ERR_WILDTOPLEVEL (414) //
	// RPL_AWAY (301)
*/