/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 22:21:29 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// INVITEコマンドの引数数チェック (INVITE <nickname> <channel> であること)
static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	if (words.size() < 2 || words[0].empty() || words[1].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "INVITE");
		errMessage += INVITE_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	// 引数が複数ある場合
	if (words.size() > 2)
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += INVITE_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}

static bool checkInviteEligibility(Server *server, int const clientFd, 
	std::string &inviterNick, std::string &targetNick, std::string &channelName)
{
	std::string errMessage = "";

	// 1. 対象チャンネルが存在することを確認
	if (!server->isChannelExist(channelName))
	{
		errMessage = ERR_NOSUCHCHANNEL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 2. 招待したユーザーが対象チャンネルに参加していることを確認
	if (!channel.isClientInChannel(clientFd))
	{
		errMessage = ERR_NOTONCHANNEL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 3. 招待制チャンネルの場合、招待したユーザーがオペレーターであることを確認
	if (channel.getMode("invite") && !channel.isOperator(clientFd))
	{
		errMessage = ERR_CHANOPRIVSNEEDED(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 4. 対象クライアントが存在することを確認
	if (!server->isClientExist(targetNick))
	{
		errMessage = ERR_NOSUCHNICK(inviterNick, targetNick);
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(inviterNick, targetNick));
		return (false);
	}

	// 5. InviterとTargetが同一でないことを確認
	if (inviterNick == targetNick)
	{
		errMessage = ERR_ERRONEUSTARGET(inviterNick, targetNick, " (Inviter and target are the same)");
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 6. 招待対象のユーザーが既にチャンネルに参加していることを確認
	int targetFd = server->getClientFdByNick(targetNick);
	if (channel.isClientInChannel(targetFd))
	{
		errMessage = ERR_USERONCHANNEL(inviterNick, targetNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 7. チャンネルが招待制限されている場合、招待対象のユーザーがチャンネルに参加できることを確認
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity())
	{
		errMessage = ERR_CHANNELISFULL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 8. 招待対象のユーザーが既に招待されているか確認
	if (channel.isInvited(clientFd))
	{
		errMessage = ERR_USERONINVITED(inviterNick, targetNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}

// コマンド形式: INVITE <nickname> <channel>
void invite(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string inviterNick = client.getNickname();

	// 1. 入力パラメータの分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return ;

	// 2. パラメータから招待対象のニックネームとチャンネル名を取得する
	std::string targetNick = words[0];
	std::string channelName = getChannelNameFromWord(words[1]);

	// 3. パラメータの妥当性を確認する
	if (!checkInviteEligibility(server, clientFd, inviterNick, targetNick, channelName))
		return ;
		
	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 4. 招待対象のユーザーを招待リストに追加する
	int targetFd = server->getClientFdByNick(targetNick);
	channel.addInvitedList(targetFd);

	// 5. 招待したユーザーに成功通知の送信、招待対象のユーザーに招待通知を送信する
	std::string notice = RPL_INVITE(IRC_PREFIX(inviterNick, client.getUserName()), targetNick, channelName);
	std::string invitation = RPL_INVITING(IRC_PREFIX(inviterNick, client.getUserName()), inviterNick, targetNick, channelName);
	addToClientSendBuf(server, clientFd, notice);
	addToClientSendBuf(server, targetFd, invitation);
}

/*
  :inviterNick!username@localhost INVITE targetNick #channelName			; for inviter
  :inviterNick!username@localhost 341 inviterNick targetNick #channelName	; for target
*/

/*
Numeric Replies:
	RPL_INVITING (341)
	ERR_NEEDMOREPARAMS (461)
	ERR_NOSUCHCHANNEL (403)
	ERR_NOTONCHANNEL (442)
	ERR_CHANOPRIVSNEEDED (482)
	ERR_USERONCHANNEL (443)
*/