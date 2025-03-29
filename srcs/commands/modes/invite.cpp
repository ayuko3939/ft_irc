/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/28 18:51:31 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Server *server, Channel &channel, Client &client, bool sign)
{
	// 既に指定されたモードが設定されている場合はエラー
	if (channel.getMode("i") == sign)
	{
		std::string msg = "Invite-only mode is already ";
		msg += (sign ? "on\r\n" : "off\r\n");
		addToClientSendBuf(server, client.getClientFd(), msg);
		return (true);
	}
	return (false);
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign)
{
	std::string modeString = "Invite-only mode ";
	modeString += (sign ? "on" : "off");
	std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

void inviteOnlyMode(Server *server, Channel &channel, Client &client, bool sign)
{
	// 1. 現在の状態をチェック
	if (isAlreadySet(server, channel, client, sign))
		return;

	// 2.モードを変更
	channel.setMode(sign, 'i');

	// 3. モード変更を全員に通知
	broadcastModeChange(server, channel, client, sign);

	// // 3. （オプション）ログ出力（デバッグ用）
	// std::cout << "[DEBUG] Invite-only mode " << (sign ? "on" : "off")
	// 			<< " for channel " << channel.getName() << std::endl;
}
