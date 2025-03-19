/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega           #+#    #+#            */
/*   Updated: 2025/03/16 18:18:02 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

#include <set>

// ブロードキャスト用関数：対象チャンネル内の全メンバーにQUIT通知を送信
static void broadcastQuit(Server *server, Channel &channel, int const clientFd, const std::string &reason)
{
	// std::set を使ってファイルディスクリプタを重複なく格納する
	std::set<int> uniqueFds;
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		uniqueFds.insert(it->first);
	}

	uniqueFds.erase(clientFd);

	// 重複を除いた各FDにQUIT通知を送信
	for (std::set<int>::iterator it = uniqueFds.begin(); it != uniqueFds.end(); ++it)
	{
		addToClientSendBuf(server, *it, RPL_QUIT(server->getNickname(clientFd), reason));
	}
}

// QUIT コマンドの処理
// コマンド形式: QUIT [<reason>]
void quit(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 理由（reason）の取得
	// ※ 理由が指定されていなければデフォルトの理由を使用する
	std::string reason = trim(cmdInfo.message);
	if (reason.empty())
		reason = "Quit: I'm off to hitchhike across the galaxy... Don't panic! I've got my towel! :)";
	else
		reason = "Quit: " + reason;

	// 2. 発行者が参加している各チャンネルに対してQUIT通知を送信
	std::map<std::string, Channel> &channels = server->getChannelList();
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel &channel = it->second;
		if (channel.isClientInChannel(clientFd))
		{
			// ブロードキャスト関数を呼び出して各メンバーにQUIT通知を送る
			broadcastQuit(server, channel, clientFd, reason);
		}
	}
		
	// 3. 発行者を全チャンネルから削除
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->second.isClientInChannel(clientFd))
			it->second.removeClient(clientFd);
	}

	// 4. 発行者に最終通知を送信して接続を閉じる
	Client &client = retrieveClient(server, clientFd);
	addToClientSendBuf(server, clientFd, SEY_BYE(client.getNickname()));

	client.setToDeconnect();
}
