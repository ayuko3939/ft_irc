/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/12 17:44:50 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::string	retrieveNickname(std::string message)
{
	std::string nickname = trim(message);

	if (!message.empty() && message.find(' '))
	{
		nickname = message.substr(0, message.find(' '));
	}

	return (nickname);
}

static bool	isValidNickname(std::string nickname)
{
	if (nickname.empty() || (nickname.size() > 10))
		return (false);
	
	for (std::size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]))
			return (false);
	}
	return (true);
}

static bool	isAlreadyUsed(Server *server, int clientFd, std::string nickname)
{
	std::map<int, Client>::iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it)
	{
		if (it->first != clientFd && it->second.getNickname() == nickname)
			return (true);
	}
	return (false);
}

void nick(Server *server, int clientFd,s_ircCommand cmdInfo)
{
	// 1. ユーザー入力の受け取り（retrieveNickname() で cmdInfo.message からニックネームを抽出）
	std::string newNick = retrieveNickname(cmdInfo.message);

	// クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);

	// 2. 入力の空チェック
	if (!isValidNickname(newNick))
	{
		addToClientSendBuf(server, clientFd, ERR_ERRONEUSNICKNAME(client.getNickname(), newNick) + 
		"[!] nickname requirements: len(1-10), char(a-z, A-Z, 0-9)\n");
		return ;
	}

	// 3. 現在のニックネームとの比較（同じ場合は何もしない）
	if (client.getNickname() == newNick)
		return ;

	// 6. 重複チェック（他のユーザーが既に使用しているか）
	if (isAlreadyUsed(server, clientFd, newNick)) {
		addToClientSendBuf(server, clientFd, "Error: Nickname is already in use.\n");
		return ;
	}

	// 7. ニックネームの更新
	// 現在のニックネームを旧ニックネームとして保持し、新しいニックネームに更新
	if (client.getNickname().empty())
	{
		client.setNickname(newNick);
		client.setOldNickname(newNick);
		client.incrementNmInfo();
	}
	else
	{
		std::string oldNick = client.getNickname();
		client.setNickname(newNick);
	}

	// 8. 成功通知の送信
	// RPL_NICK マクロ（もしくは関数）で新旧の情報を含む成功メッセージを生成し、クライアントに送信
	addToClientSendBuf(server, clientFd, RPL_NICK(client.getNickname(), newNick));
}
