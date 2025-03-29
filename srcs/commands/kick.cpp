/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/29 19:17:03 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
	// <channel> <user> [<comment>]
	if (words.size() < 2 || words.size() > 3)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(KICK_USAGE));
		return (false);
	}
	// <comment> が指定されている場合、文字数が30文字以内か確認
	if (words.size() == 3 && words[2].size() > 30)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(KICK_REQUIREMENTS));
		return (false);
	}
    return (true);
}

static std::string getChannelNameFromWord(std::string &word)
{
	if (word[0] == '#')
		return (word.substr(1));
	return (word);
}

static bool isValid(Server *server, int const clientFd, std::string targetNick, std::string channelName)
{
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();

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
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(issuerNick, targetNick));
		return (false);
	}
	// InviterとTargetが同一でないことを確認
	if (issuerNick == targetNick)
	{
		addToClientSendBuf(server, clientFd, ":" + issuerNick + " : Issuer and target are the same.\r\n");
		return (false);
	}
	return (true);
}

static void broadcastNewMember(Server *server, Channel &channel, Client &client, std::string &target, std::string &comment)
{
	// チャンネルメンバー全員に追放を通知
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), RPL_KICK(IRC_PREFIX(client.getNickname(), client.getUserName()), target, channel.getName(), comment));
	}
}

// コマンド形式: KICK <channel> <user> [<comment>]
void kick(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return;

	// 2. 発行者のクライアント情報を取得
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();

	// 3. パラメータから対象のチャンネル名と追放対象ユーザーのニックネームを取得
	std::string channelName = getChannelNameFromWord(words[0]);
	std::string targetNick = words[1];

	// 4. コメントが指定されていなければデフォルトメッセージを使用
	std::string comment;
	if (words.size() == 3)
		comment = words[2];
	if (comment.empty())
		comment = DEFAULT_KICK_COMMENT;

	// 5. パラメータの妥当性を確認
	if (!isValid(server, clientFd, targetNick, channelName))
		return;

	// 6. 発行者が対象チャンネルに参加しているか確認
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(channelName)->second;

	if (!channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(issuerNick, channelName));
		return;
	}

	// 7. 発行者がチャンネルオペレーターであるか確認
	if (!channel.isOperator(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_CHANOPRIVSNEEDED(issuerNick, channelName));
		return;
	}

	// 8. 対象ユーザーがチャンネルに参加しているかチェック
	int targetFd = server->getClientFdFromNick(targetNick);
	if (!channel.isClientInChannel(targetFd))
	{
		addToClientSendBuf(server, clientFd, ERR_USERNOTINCHANNEL(issuerNick, targetNick, channelName));
		return;
	}

	// 9. チャンネルメンバー全員に追放を通知
	broadcastNewMember(server, channel, client, targetNick, comment);

	// 10. 対象ユーザーをチャンネルから削除する
	channel.removeClient(targetFd);
}
