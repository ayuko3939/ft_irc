/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/04/04 21:36:58 by yohasega         ###   ########.fr       */
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
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE PRIVMSG_USAGE END << std::endl;
		return (false);
	}
	if (text.empty())
	{
		errMessage = ERR_NOTEXTTOSEND(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE PRIVMSG_USAGE END << std::endl;
		return (false);
	}
	return (true);
}

static void splitMessageText(std::string &text, std::vector<std::string> &msg)
{
	// もしテキストの長さが MSGLEN 以下なら、そのまま追加して終了
	if (text.size() <= MSGLEN)
	{
		msg.push_back(text);
		return;
	}

	size_t start_pos = 0;
	// テキスト全体を MSGLEN 文字ごとに分割する
	while (start_pos < text.size())
	{
		msg.push_back(text.substr(start_pos, MSGLEN));
		start_pos += MSGLEN;
	}
}

static void SendAllMsg(Server *server, int const clientFd, std::string &nick, std::string &user,
	std::string &target, std::vector<std::string> &splitText)
{
	std::string text = "";
	std::string message = "";

	// 1. チャンネルに参加している全メンバーにメッセージを送信
	for (std::vector<std::string>::iterator it = splitText.begin(); it != splitText.end(); ++it)
	{
		text = *it;
		message = RPL_PRIVMSG(IRC_PREFIX(nick, user), '#' + target, text);
		addToClientSendBuf(server, clientFd, message);
	}
}

static void broadcastMsg(Server *server, Channel &channel, Client &client, 
	std::string &target, std::vector<std::string> &splitText)
{
	std::string senderNick = client.getNickname();

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		if (it->second.getNickname() != senderNick)
			SendAllMsg(server, it->first, senderNick, client.getUserName(), target, splitText);
	}
}

// PRIVMSG <target> <text to be sent>
void privmsg(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string senderNick = client.getNickname();
	std::string target = "";
	std::string text = "";
	std::string errmessage = "";
	std::vector<std::string> splitText;

	// 1. ターゲットとメッセージを取得（最初のスペースで区切り、以降はまとめて１つのテキストとする）
	getTargetAndText(cmdInfo.message, target, text);
	
	// 2. 入力パラメータをチェック
	if (!checkArguments(server, clientFd, senderNick, target, text))
	return ;
	
	// 3. メッセージを MSGLEN 文字ごとに分割
	splitMessageText(text, splitText);

	// 4-1. ターゲットがチャンネルの場合
	if (cmdInfo.message[0] == '#')
	{
		// 5. ターゲットが存在するか確認
		if (!server->isChannelExist(target))
		{
			errmessage = ERR_NOSUCHCHANNEL(senderNick, target);
			addToClientSendBuf(server, clientFd, errmessage);
			return ;
		}

		std::map<std::string, Channel> &channels = server->getChannelList();
		Channel &channel = channels.find(target)->second;

		// 6. 送信者がそのチャンネルに参加しているか確認
		if (!channel.isClientInChannel(clientFd))
		{
			errmessage = ERR_NOTONCHANNEL(senderNick, target);
			addToClientSendBuf(server, clientFd, errmessage);
			return ;
		}

		// 7. チャンネル内の全メンバー（送信元以外）に対して、メッセージをブロードキャスト
		broadcastMsg(server, channel, client, target, splitText);
	}
	// 4-2. ターゲットがユーザーの場合
	else
	{
		// 4. ターゲットが存在するか確認
		if (!server->isClientExist(target))
		{
			errmessage = ERR_NOSUCHNICK(senderNick, target);
			addToClientSendBuf(server, clientFd, errmessage);
			return ;
		}
		
		// 5. ターゲットにメッセージを送信
		int targetFd = server->getClientFdByNick(target);
		for (std::vector<std::string>::iterator it = splitText.begin(); it != splitText.end(); ++it)
		{
			std::string msg = senderNick + " PRIVMSG " + target + " " + *it + "\r\n";
			addToClientSendBuf(server, targetFd, msg);
		}
	}
}

/*
  :nickname!username@localhost PRIVMSG #channelName :message string		// for channel
  :nickname PRIVMSG targetName :message string							// for user
*/
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