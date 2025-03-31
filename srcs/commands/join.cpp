/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 22:13:43 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::string getChannelNameFromWord(std::string &word)
{
	if (word[0] == '#')
		return (word.substr(1));
	return (word);
}

static bool	isValid(std::string &channelName, std::string &key)
{
	if (channelName.empty() || channelName.size() > 20 || key.size() > 10)
		return (false);
	
	for (std::size_t i = 0; i < channelName.size(); i++)
	{
		if (!isalnum(channelName[i]))
			return (false);
	}
	for (std::size_t i = 0; i < key.size(); i++)
	{
		if (!isalnum(key[i]))
			return (false);
	}
	return (true);
}

static bool checkArguments(Server *server, int clientFd, 
	std::vector<std::string> &words, std::string &channelName, std::string &key)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数がないまたは空の場合
	if (words.empty() || words[0].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "JOIN");
		errMessage += JOIN_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return false;
	}
	// 引数が複数ある場合
	if (words.size() > 2)
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += JOIN_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}
	
	// チャンネル名とキーを取得
	channelName = getChannelNameFromWord(words[0]);
	if (words.size() > 1)
		key = words[1];

	// チャンネル名とキーの妥当性をチェック
	if (!isValid(channelName, key))
	{
		errMessage = ERR_INVALID_PARM;
		errMessage += JOIN_REQUIREMENTS;
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}
	return true;
}

static bool checkJoinEligibility(Server *server, int clientFd, 
	Channel &channel, std::string key)
{
	std::string nickname = server->getNickname(clientFd);
	std::string channelName = channel.getName();
	std::string errMessage = "";

	// 既に参加している場合、エラーを返す
	if (channel.isClientInChannel(clientFd))
	{
		errMessage = ERR_USERONCHANNEL(nickname, nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// パスワード保護(mode:k)されている場合、キーの一致を確認する
	if (channel.getMode("key"))
	{
		if (key != channel.getPassword())
		{
			errMessage = ERR_BADCHANNELKEY(nickname, channelName);
			addToClientSendBuf(server, clientFd, errMessage);
			return (false);
		}
	}
	
	// 招待制(mode:i)されている場合、はJOINできない
	if (channel.getMode("invite"))
	{
		errMessage = ERR_INVITEONLYCHAN(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 参加人数制限チェック
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity())
	{
		errMessage = ERR_CHANNELISFULL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

static void broadcastNewMember(Server *server, Channel &channel,const std::string channelName, Client client)
{
	std::string notice = RPL_JOIN(IRC_PREFIX(client.getNickname(), client.getUserName()), channelName);
	std::map<const int, Client> &clientList = channel.getClientList();

	// チャンネルメンバー全員に新しいメンバーの参加を通知
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		addToClientSendBuf(server, it->second.getClientFd(), notice);
	}
}

// JOIN <channel> [<key>]
void join(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string channelName = "";
	std::string key = "";

	// 1. ユーザー入力をスペース区切りで取得し、引数の数をチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	
	// 2. 引数のチェック
	if (!checkArguments(server, clientFd, words, channelName, key))
		return;

	// 3. サーバーのチャンネルリストから対象チャンネルを探す
	std::map<std::string, Channel> &channelList = server->getChannelList();
	std::map<std::string, Channel>::iterator it = channelList.find(channelName);

	// 4-1. 存在しない場合は新規作成
	if (it == channelList.end())
	{
		// チャンネルを作成
		server->addChannel(channelName);
		it = channelList.find(channelName);

		// チャンネルの初期設定（オペレーター、パスワード等）
		it->second.setOperatorList(clientFd);
		if (key != "")
		{
			it->second.setMode(true, 'k');
			it->second.setPassword(key);
		}
	}
	// 4-2. 既存のチャンネルに参加できるかチェック
	else
	{
		if (!checkJoinEligibility(server, clientFd, it->second, key))
			return ;
		// チャンネルに誰も参加していない場合、オペレーターリストに追加
		if (it->second.getClientList().empty())
			it->second.setOperatorList(clientFd);
	}

	// 5. クライアントをチャンネルに追加
	server->addClientToChannel(channelName, client);

	// 6. チャンネル参加後、JOINメッセージ、トピック、参加者一覧などの情報を送信
	broadcastNewMember(server, it->second, channelName, client);
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	// ERR_NOSUCHCHANNEL (403)
	// ERR_TOOMANYCHANNELS (405)
	ERR_BADCHANNELKEY (475)
	// ERR_BANNEDFROMCHAN (474) // BAN
	ERR_CHANNELISFULL (471)
	ERR_INVITEONLYCHAN (473)
	// ERR_BADCHANMASK (476)
	// RPL_TOPIC (332)
	// RPL_TOPICWHOTIME (333)
	// RPL_NAMREPLY (353)
	// RPL_ENDOFNAMES (366)
*/