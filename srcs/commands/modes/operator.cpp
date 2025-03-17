/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/12 19:43:58 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySetOperator(Server *server, Channel &channel, Client &client, bool sign, const std::string &targetNick)
{
	// 対象ユーザーのFDを取得
	int targetFd = server->getClientFdFromNick(const_cast<std::string&>(targetNick));
	if (targetFd == -1)
    {
        // 対象ユーザーが存在しない場合
        addToClientSendBuf(server, client.getClientFd(), ERR_NOSUCHNICK(client.getNickname(), targetNick));
        return (false);
    }

	// 既に対象ユーザーがオペレーター状態であれば、同じ状態の変更は不要
	if (channel.isOperator(targetFd) == sign)
	{
		std::string msg = (targetNick + " is already ");
		msg += (sign ? "an operator.\r\n" : "not an operator.\r\n");
		addToClientSendBuf(server, client.getClientFd(), msg);
		return (true);
	}
	return (false);
}

static void broadcastOperatorModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &targetNick)
{
	std::string modeString = targetNick;
	modeString += (sign ? " is now an operator.": " is no longer an operator.");
	std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

// MODE コマンドの +o / -o 処理（オペレーター権限の付与／解除）
void operatorMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs)
{
	// 1. 既に同じ状態なら、通知して処理終了
	if (isAlreadySetOperator(server, channel, client, sign, modeArgs))
		return;

	// 2. 対象ユーザーのFDを取得（存在チェックは事前に行っている前提）
	int targetFd = server->getClientFdFromNick(modeArgs);
	if (targetFd == -1)
	{
		// 対象ユーザーが存在しない場合（通常はチェック済みのはず）
		addToClientSendBuf(server, client.getClientFd(), ERR_NOSUCHNICK(client.getNickname(), modeArgs));
		return;
	}

	// 3-1. +oの場合：対象ユーザーをオペレーターリストに追加
	if (sign)
	{
		channel.setOperatorList(targetFd);
	}
    // 3-2. -oの場合：対象ユーザーをオペレーターリストから削除
	else
	{
		channel.removeOperator(targetFd);
	}

	// 4. 変更内容をチャンネル全体に通知
	broadcastOperatorModeChange(server, channel, client, sign, modeArgs);
}
