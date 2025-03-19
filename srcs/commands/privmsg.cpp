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

void privmsg(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 送信元クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);
	std::string senderNick = client.getNickname();

	// 2. 入力パラメータの分割（最初のスペースで区切り、以降はまとめて１つのテキストとする）
	std::string argument = trim(cmdInfo.message);
	size_t pos = argument.find(' ');
	if (pos == std::string::npos)
	{
		// スペースが見つからなければ、送信先またはメッセージ本文が不足していると判断
		addToClientSendBuf(server, clientFd, ERR_NORECIPIENT(senderNick, "PRIVMSG"));
		return;
	}
	std::string target = argument.substr(0, pos);
	std::string message = trim(argument.substr(pos + 1));

	// 送信先または本文が空の場合、エラーを返す
	if (target.empty() || message.empty())
	{
		addToClientSendBuf(server, clientFd, ERR_NORECIPIENT(senderNick, "PRIVMSG"));
		return;
	}

	// 3. ターゲットがチャンネルかユーザーかで処理を分岐
	if (target[0] == '#')
	{
		// チャンネルの場合
		if (!server->isChannelExist(target))
		{
			addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(senderNick, target));
			return;
		}
		std::map<std::string, Channel> &channels = server->getChannelList();
		Channel &channel = channels.find(target)->second;

		// 送信元がそのチャンネルに参加しているか確認
		if (!channel.isClientInChannel(clientFd))
		{
			addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(senderNick, target));
			return;
		}

		// 4. チャンネル内の全メンバー（送信元以外）に対して、メッセージをブロードキャスト
		std::map<const int, Client> &clientList = channel.getClientList();
		for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		{
			if (it->first != clientFd)
			{
				// RPL_PRIVMSGは ":sender PRIVMSG target :message\r\n" の形式を生成すると想定
				std::string notify = RPL_PRIVMSG(senderNick, target, message);
				addToClientSendBuf(server, it->second.getClientFd(), notify);
			}
		}
	}
	else
	{
		// ユーザーの場合
		if (!server->isClientExist(target))
		{
			addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(senderNick, target));
			return;
		}
		int targetFd = server->getClientFdFromNick(target);
		std::string notify = RPL_PRIVMSG(senderNick, target, message);
		addToClientSendBuf(server, targetFd, notify);
	}
}
