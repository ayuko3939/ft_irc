/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/29 19:25:34 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::string getReason(std::string &cmdLine)
{
	std::string reason = trim(cmdLine);

	if (reason.size() > QUITLEN)
		reason = reason.substr(0, QUITLEN);

	return ("Quit: " + reason);	
}

// コマンド形式: QUIT [<reason>]
void quit(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);

	// 1. 理由（reason）の取得
	std::string reason = getReason(cmdInfo.message);
	std::string notice = RPL_QUIT(IRC_PREFIX(client.getNickname(), client.getUserName()), reason);

	// 2. QUIT通知を送信
	std::map<const int, Client>	&clientList = server->getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->first, notice);
	}
		
	// 3. 発行者を全チャンネルから削除
	std::map<std::string, Channel> &channelList = server->getChannelList();
	for (std::map<std::string, Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		if (it->second.isClientInChannel(clientFd))
			it->second.removeClient(clientFd);
	}

	// 4. 発行者に最終通知を送信して接続を閉じる
	addToClientSendBuf(server, clientFd, SEY_BYE(IRC_PREFIX(client.getNickname(), client.getUserName())));

	client.setToDeconnect();
}
/*
 :nickname!username@localhost QUIT :Quit: Bye for now!
 */
/*
Numeric Replies:
	None
*/