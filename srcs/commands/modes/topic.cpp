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

static bool isAlreadySet(Server *server, Channel &channel, Client &client, bool sign)
{
    // 既にトピック保護モードが設定されている場合、通知を送って true を返す
    if (channel.getMode("t") == sign)
    {
        std::string msg = "Topic protect mode is already ";
        msg += (sign ? "on\r\n" : "off\r\n");
        addToClientSendBuf(server, client.getClientFd(), msg);
        return true;
    }
    return false;
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign)
{
    std::string modeString = "Topic protect mode ";
    modeString += (sign ? "on" : "off");
    std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString);

    std::map<const int, Client> &clientList = channel.getClientList();
    for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
    {
        addToClientSendBuf(server, it->second.getClientFd(), notify);
    }
}

void topicProtectMode(Server *server, Channel &channel, Client &client, bool sign)
{
    // 0. 既に同じ状態なら通知して終了する
    if (isAlreadySet(server, channel, client, sign))
        return;

    // 1. チャンネルのモードフラグを更新（'t' オプション）
    channel.setMode('t', sign);

    // 2. 変更内容をチャンネル内全メンバーに通知する
    broadcastModeChange(server, channel, client, sign);

    // （オプション）ログ出力（デバッグ用）
//    std::cout << "[DEBUG] Topic protect mode " << (sign ? "on" : "off")
//              << " for channel " << channel.getName() << std::endl;
}
