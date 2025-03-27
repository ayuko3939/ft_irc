/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/27 23:02:06 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
	// 引数が1つまたは2つでない場合はエラー（チャンネル名が必須、キーは任意）
	if (words.size() < 1 || words.size() > 2)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(JOIN_USAGE));
		return false;
	}
	return true;
}

static std::string getChannelNameFromWord(std::string &word)
{
	if (word[0] == '#')
		return (word.substr(1));
	return (word);
}

static bool	isValid(std::string &word, std::string &key)
{
	if (word.empty() || (word.size() > 10))
		return (false);
	
	for (std::size_t i = 0; i < word.size(); i++)
	{
		if (!isalnum(word[i]))
			return (false);
	}
	for (std::size_t i = 0; i < key.size(); i++)
	{
		if (!isalnum(key[i]))
			return (false);
	}
	return (true);
}

static bool checkJoinEligibility(Server *server, Client client, Channel &channel, std::string key)
{
	std::string clientNickname = client.getNickname();
	const int clientFd = client.getClientFd();
	std::string channelName = channel.getName();

	// 既に参加している場合、エラーを返す
	if (channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(clientNickname, clientNickname, channelName));
		return (false);
	}

	// パスワード保護(mode:k)されている場合、キーの一致を確認する
	if (channel.getMode("key"))
	{
		if (key != channel.getPassword())
		{
			addToClientSendBuf(server, clientFd, ERR_BADCHANNELKEY(clientNickname, channelName));
			return (false);
		}
	}
	
	// 招待制(mode:i)されている場合、はJOINできない
	if (channel.getMode("invite"))
	{
		addToClientSendBuf(server, clientFd, ERR_INVITEONLYCHAN(clientNickname, channelName));
		return (false);
	}

	// 参加人数制限チェック
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity())
	{
		addToClientSendBuf(server, clientFd, ERR_CHANNELISFULL(clientNickname, channelName));
		return (false);
	}

	return (true);
}

static void broadcastNewMember(Server *server, Channel &channel,const std::string channelName, Client client)
{
	std::string newClientNick = client.getNickname();

	// チャンネルメンバー全員に新しいメンバーの参加を通知
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), RPL_JOIN(newClientNick, channelName));
	}
}

// JOIN <channel> [<key>]
void join(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	// 1. ユーザー入力をスペース区切りで取得し、引数の数をチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return;

	// 2. チャンネル名とキーを取得
	std::string channelName = getChannelNameFromWord(words[0]);
	std::string key = "";
	if (words.size() > 1)
		key = words[1];

	// 3. チャンネル名の妥当性をチェック
	if (!isValid(channelName, key))
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(JOIN_REQUIREMENTS));
		return ;
	}

	// 4. クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);

	// 5. サーバーのチャンネルリストから対象チャンネルを探す
	std::map<std::string, Channel> &channels = server->getChannelList();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);

	// 6-1. 存在しない場合は新規作成
	if (it == channels.end())
	{
		// チャンネルを作成
		server->addChannel(channelName);
		it = channels.find(channelName);

		// チャンネルの初期設定（オペレーター、パスワード等）
		it->second.setOperatorList(clientFd);
		if (key != "")
		{
			it->second.setMode('k', true);
			it->second.setPassword(key);
		}
	}
	// 6-2. 既存のチャンネルに参加できるかチェック
	else
	{
		if (!checkJoinEligibility(server, client, it->second, key))
			return ;
	}

	// 7. クライアントをチャンネルに追加
	server->addClientToChannel(channelName, client);

	// 8. チャンネル参加後、JOINメッセージ、トピック、参加者一覧などの情報を送信
	broadcastNewMember(server, it->second, channelName, client);
}
