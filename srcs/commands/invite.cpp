/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/17 14:43:29 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// INVITEコマンドの引数数チェック (INVITE <nickname> <channel> であること)
static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
    if (words.size() != 2)
    {
        addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(INVITE_USAGE));
        return false;
    }
    return true;
}

static bool isValid(Server *server, int const clientFd, std::string targetNick, std::string channelName)
{
	Client &client = retrieveClient(server, clientFd);
	std::string inviterNick = client.getNickname();

	// 対象ニックネームとチャンネル名が空文字でないことを確認
	if (targetNick.empty() || channelName.empty())
	{
		addToClientSendBuf(server, clientFd, ERR_PARM_EMPTY);
		return (false);
	}
	// 対象チャンネルが存在することを確認
	if (!server->isChannelExist(channelName))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(targetNick, channelName));
		return (false);
	}
	// 対象クライアントが存在することを確認
	if (!server->isClientExist(targetNick))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(inviterNick, targetNick));
		return (false);
	}
	// InviterとTargetが同一でないことを確認
	if (inviterNick == targetNick)
	{
		addToClientSendBuf(server, clientFd, ":" + inviterNick + " : Inviter and target are the same.\r\n");
		return (false);
	}
	return (true);
}

static void broadcastNewMember(Server *server, Channel &channel, std::string &inviter, std::string &target)
{
	// チャンネルメンバー全員に新しいメンバーの参加を通知
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), RPL_INVITE(inviter, target, channel.getName()));
	}
}

// コマンド形式: INVITE <nickname> <channel>
void invite(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータの分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return;

	// 2. クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);
	std::string inviterNick = client.getNickname();

	// 3. パラメータから招待対象のニックネームとチャンネル名を取得する
	std::string targetNick = words[0];
	std::string channelName = words[1];

	// 4. パラメータの妥当性を確認する
	if (!isValid(server, clientFd, targetNick, channelName))
		return;

	// 5. 招待したユーザーが対象チャンネルに参加しているかチェックする
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(channelName)->second;

	if (!channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(inviterNick, channelName));
		return;
	}

	// 6. 招待対象のユーザーが既にチャンネルに参加しているかチェックする
	int targetFd = server->getClientFdFromNick(targetNick);
	if (channel.isClientInChannel(targetFd))
	{
		addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(inviterNick, targetNick, channelName));
		return;
	}

	// 7. チャンネルが招待制限されている場合、招待対象のユーザーがチャンネルに参加できるかチェックする
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity())
	{
		addToClientSendBuf(server, clientFd, ERR_CHANNELISFULL(inviterNick, channelName));
		return;
	}

	// 8. 招待対象のユーザーをチャンネルに追加する
	Client &target = retrieveClient(server, targetFd);
	channel.addClientToChannel(target);

	// 9. チャンネルメンバー全員に新しいメンバーの参加を通知
	broadcastNewMember(server, channel, inviterNick, targetNick);
}
