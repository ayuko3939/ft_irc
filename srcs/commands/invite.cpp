/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 23:30:04 by yohasega         ###   ########.fr       */
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

bool isValid(Server *server, int const clientFd, std::string guestNick, std::string channelName)
{
	Client &client = retrieveClient(server, clientFd);
	std::string inviterNick = client.getNickname();

	// 対象ニックネームとチャンネル名が空文字でないことを確認
	if (guestNick.empty() || channelName.empty())
	{
		addToClientSendBuf(server, clientFd, ERR_PARM_EMPTY);
		return (false);
	}
	// 対象チャンネルが存在することを確認
	if (!server->isChannelExist(channelName))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(guestNick, channelName));
		return (false);
	}
	// 対象クライアントが存在することを確認
	if (!server->isClientExist(guestNick))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(inviterNick, guestNick));
		return (false);
	}
	return (true);
}

static void broadcastNewMember(Server *server, Channel &channel, std::string &inviter, std::string &guest)
{
	// チャンネルメンバー全員に新しいメンバーの参加を通知
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), RPL_INVITE(inviter, guest, channel.getName()));
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
	std::string guestNick = words[0];
	std::string channelName = words[1];

	// 4. パラメータの妥当性を確認する
	if (!isValid(server, clientFd, guestNick, channelName))
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
	int guestFd = server->getClientFdFromNick(guestNick);
	if (channel.isClientInChannel(guestFd))
	{
		addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(inviterNick, guestNick, channelName));
		return;
	}

	// 7. 招待対象のユーザーをチャンネルに追加する
	Client &guest = retrieveClient(server, guestFd);
	channel.addClientToChannel(guest);

	// 8. サーバーのクライアントリストを走査し、招待対象ユーザーにINVITEメッセージを送信する
	broadcastNewMember(server, channel, inviterNick, guestNick);
}
