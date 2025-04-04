/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/04/04 16:33:29 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Server *server, Channel &channel, Client &client, bool sign)
{
	// 既にトピック保護モードが設定されている場合、通知を送って true を返す
	if (channel.getMode("t") == sign)
	{
		std::string msg = "Topic protect mode is already ";
		msg += (sign ? "ON\r\n" : "OFF\r\n");
		addToClientSendBuf(server, client.getClientFd(), msg);
		return true;
	}
	return false;
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign)
{
	std::string modeString = (sign ? "+t" : "-t");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

void topicProtectMode(Server *server, Channel &channel, Client &client, bool sign)
{
	// 1. 現在の状態をチェック
	if (isAlreadySet(server, channel, client, sign))
		return;

	// 2.モードを変更
	channel.setMode(sign, 't');

	// 3. モード変更を全員に通知
	broadcastModeChange(server, channel, client, sign);

	// （オプション）ログ出力（デバッグ用）
	// std::cout << "[DEBUG] Topic protect mode " << (sign ? "on" : "off")
	// 			<< " for channel " << channel.getName() << std::endl;
}
