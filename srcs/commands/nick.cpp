/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 19:22:09 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool	isValid(std::string nickname)
{
	if ((nickname.size() > 10))
		return (false);

	for (std::size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]))
			return (false);
	}
	return (true);
}

static bool	isAlreadyUsed(Server *server, const int clientFd, std::string nickname)
{
	std::map<int, Client>::iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it)
	{
		if (it->first != clientFd && it->second.getNickname() == nickname)
			return (true);
	}
	return (false);
}

static bool checkAndGetArguments(Server *server, const int clientFd, 
	std::vector<std::string> &words, std::string &newNick)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (words.empty() || words[0].empty())
	{
		errMessage = ERR_NONICKNAMEGIVEN(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	// 引数が複数ある場合
	if (words.size() > 1)
		std::cout << GUIDE NICK_USAGE END << std::endl;

	// 不正なニックネームの場合
	newNick = words[0];
	if (!isValid(newNick))
	{
		errMessage = ERR_ERRONEUSNICKNAME(nickname, newNick);
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE NICK_REQUIREMENTS END << std::endl;
		return (false);
	}
	// 重複チェック（他のユーザーが既に使用しているか）
	if (isAlreadyUsed(server, clientFd, newNick))
	{
		errMessage = ERR_NICKNAMEINUSE(nickname, newNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}

/*
  :oldNick NICK newNick							; for other members
  :oldNick!username@localhost NICK newNick		; for personal use
*/
// static void broadcastNick(Server *server, std::string &userName, std::string &oldNick, std::string &newNick)
// {
// 	std::string notice = oldNick + " NICK " + newNick + "\r\n";
// 	std::string noticeT = RPL_NICK(IRC_PREFIX(oldNick, userName), newNick);

// 	std::map<const int, Client>	&clientList = server->getClientList();
// 	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
// 	{
// 		if (it->second.getNickname() == newNick)
// 			addToClientSendBuf(server, it->first, noticeT);
// 		else
// 			addToClientSendBuf(server, it->first, notice);
// 	}
// }

void nick(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string oldNick = client.getOldNickname();
	std::string newNick = "";

	// 1. ユーザー入力をスペースで分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 2. 引数の数が正しいかチェック（ニックネームの文字数、文字種）
	if (!checkAndGetArguments(server, clientFd, words, newNick))
		return ;

	// 3-1. 未設定の場合は新しいニックネームを設定
	if (client.getNickname().empty())
	{
		client.setNickname(newNick);
		client.incrementNmInfo();
	}
	// 3-2. 現在のニックネームを旧ニックネームとして保持し、新しいニックネームに更新
	else
	{
		oldNick = client.getNickname();
		client.setOldNickname(oldNick);
		client.setNickname(newNick);
	}

	// 4. 成功通知の送信（クライアントだけでいいけど、知らん間に名前変わってたら困るので全員に送信）
	// broadcastNick(server, client.getUserName(), oldNick, newNick);
	addToClientSendBuf(server, clientFd, RPL_NICK(IRC_PREFIX(oldNick, client.getUserName()), newNick));
}

/*
Numeric Replies:
	ERR_NONICKNAMEGIVEN (431)
	ERR_ERRONEUSNICKNAME (432)
	ERR_NICKNAMEINUSE (433)
	// ERR_NICKCOLLISION (436) // 他サーバとの競合
*/