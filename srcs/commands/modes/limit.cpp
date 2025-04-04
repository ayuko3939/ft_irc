/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limit.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/04/04 16:33:52 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySetLimit(Server *server, Channel &channel, Client &client, bool sign, const std::string &limitStr)
{
	// 文字列を整数に変換
	std::istringstream iss(limitStr);
	int limit;
	iss >> limit;

	// 既に+lモードが有効で、かつ現在の制限値が同じ場合は通知してtrueを返す
	if (channel.getMode("l") == sign && channel.getCapacity() == static_cast<size_t>(limit))
	{
		std::string msg = "User limit mode is already ";
		msg += (sign ? ("set to " + limitStr + "\r\n") : "OFF\r\n");
		addToClientSendBuf(server, client.getClientFd(), msg);
		return true;
	}
	return false;
}

static bool isValidLimit(const std::string &limitStr)
{
	// 空文字チェック
	if (limitStr.empty())
		return false;
	// 数字のみかどうかをチェック
	for (size_t i = 0; i < limitStr.size(); i++)
	{
		if (!isdigit(limitStr[i]) || limitStr[0] == '+')
			return false;
	}

	// 文字列を整数に変換
	std::istringstream iss(limitStr);
	int limit;
	iss >> limit;
	if (iss.fail())
		return false;

	// 要件：1～MAX_CLIENTSの整数であること
	if (limit < 1 || limit > MAX_CLIENTS)
		return false;
	return true;
}

static void broadcastLimitModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &limitStr)
{
	std::string modeString = (sign ? ("+l " + limitStr) : "-l");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);

	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notify);
	}
}

// modeArgs：+lの場合は制限人数（文字列で受け取る）
void userLimitMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs)
{
	// 1. 引数の検証
	if (!isValidLimit(modeArgs))
	{
		std::string errMessege = ERR_INVALID_PARM;
		errMessege += MODE_REQ_L_LIMIT;
		addToClientSendBuf(server, client.getClientFd(), errMessege);
		return;
	}

	// 2. 既に同じ制限状態なら、通知して処理終了
	if (isAlreadySetLimit(server, channel, client, sign, modeArgs))
		return;

	// 3-1. +l（有効化）の場合、limitの検証を行う
	if (sign)
	{
		int limit = std::atoi(modeArgs.c_str());
		// 制限人数を更新（例：channel.setCapacity()でユーザー数制限を設定）
		channel.setMode(sign, 'l');
		channel.setCapacity(limit);
	}
	// 3-2. -l（無効化）の場合、制限を解除する
	else
	{
		channel.setMode(sign, 'l');
		// 制限解除時は、0に戻す
		channel.setCapacity(0);
	}

	// 4. 変更内容をチャンネル全体に通知
	broadcastLimitModeChange(server, channel, client, sign, modeArgs);
}
