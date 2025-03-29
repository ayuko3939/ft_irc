/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/28 18:36:57 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Server *server, Channel &channel, Client &client, bool sign, const std::string &key)
{
	// 既に +k モードが有効で、かつ現在のキーが modeArgs と同じ場合
	if (channel.getMode("k") == sign && channel.getPassword() == key)
	{
		std::string msg = "Channel key mode is already ";
		msg += (sign ? ("set to " + key + "\r\n") : "off\r\n");
		addToClientSendBuf(server, client.getClientFd(), msg);
		return true;
	}
	return false;
}

static bool isValidKey(const std::string &key)
{
	if (key.empty() || key.size() > 20)
		return false;
	for (size_t i = 0; i < key.size(); i++)
	{
		if (!isalnum(key[i]))
			return false;
	}
	return true;
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &key)
{
	std::string modeString = "Channel key mode ";
	modeString += (sign ? ("set to \"" + key + "\"") : "off");
	std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

// MODE コマンドの +k / -k 処理（キーの設定／解除）
void channelKeyMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs)
{
	// 1. 既に同じキー状態なら、通知して処理終了
	if (isAlreadySet(server, channel, client, sign, modeArgs))
		return;

	// 2-1. +k（有効化）の場合、キーの検証を行う
	if (sign)
	{
		if (!isValidKey(modeArgs))
		{
			addToClientSendBuf(server, client.getClientFd(), ERR_INVALID_PARM + std::string(MODE_REQ_K_PASS));
			return;
		}
		// キーが有効なら、チャンネルのキー情報を更新
		channel.setMode(sign, 'k');
		channel.setPassword(modeArgs);
	}
	// 2-2. -k（無効化）の場合、キー情報をクリア
	else
	{
		channel.setMode(sign, 'k');
		channel.setPassword("");
	}

	// 3. 変更内容をチャンネル全体に通知
	broadcastModeChange(server, channel, client, sign, modeArgs);
}
