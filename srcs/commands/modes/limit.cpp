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
		msg += (sign ? ("set to " + limitStr + "\r\n") : "off\r\n");
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

	// 要件：1～10の整数であること
	if (limit < 1 || limit > MAX_CLIENTS)
		return false;
	return true;
}

static void broadcastLimitModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &limitStr)
{
    std::string modeString = "User limit mode ";
    modeString += (sign ? ("set to " + limitStr) : "off");
    std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString);

    std::map<const int, Client> &clientList = channel.getClientList();
    for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
    {
        addToClientSendBuf(server, it->second.getClientFd(), notify);
    }
}

// modeArgs：+lの場合は制限人数（文字列で受け取る）
void userLimitMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs)
{
    // 1. 既に同じ制限状態なら、通知して処理終了
    if (isAlreadySetLimit(server, channel, client, sign, modeArgs))
        return;

    // 2-1. +l（有効化）の場合、limitの検証を行う
    if (sign)
    {
        if (!isValidLimit(modeArgs))
        {
			std::string max = std::to_string(MAX_CLIENTS);
            addToClientSendBuf(server, client.getClientFd(), ERR_INVALID_PARM + std::string(MODE_REQ_L_LIMIT(max)));
            return;
        }
        int limit = std::atoi(modeArgs.c_str());
        // 制限人数を更新（例：channel.setCapacity()でユーザー数制限を設定）
        channel.setMode('l', sign);
        channel.setCapacity(limit);
    }
    // 2-2. -l（無効化）の場合、制限を解除する
    else
    {
        channel.setMode('l', sign);
        // 制限解除時は、デフォルトの最大人数（例：MAX_CLIENTS）に戻す
        channel.setCapacity(MAX_CLIENTS);
    }

    // 3. 変更内容をチャンネル全体に通知
    broadcastLimitModeChange(server, channel, client, sign, modeArgs);
}
