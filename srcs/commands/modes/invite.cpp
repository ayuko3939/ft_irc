/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/04/04 16:34:08 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Channel &channel, bool sign)
{
	// 既に指定されたモードが設定されている場合はエラー
	if (channel.getMode("i") == sign)
	{
		std::string msg = "Invite-only mode is already ";
		msg += (sign ? "ON\r\n" : "OFF\r\n");
		std::cout << GUIDE << msg << END << std::endl;
		return (true);
	}
	return (false);
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign)
{
	std::string modeString = (sign ? "+i" : "-i");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

void inviteOnlyMode(Server *server, Channel &channel, Client &client, bool sign)
{
	// 1. 現在の状態をチェック
	if (isAlreadySet(channel, sign))
		return;

	// 2.モードを変更
	channel.setMode(sign, 'i');

	// 3. モード変更を全員に通知
	broadcastModeChange(server, channel, client, sign);

	// // 3. （オプション）ログ出力（デバッグ用）
	// std::cout << "[DEBUG] Invite-only mode " << (sign ? "on" : "off")
	// 			<< " for channel " << channel.getName() << std::endl;
}
