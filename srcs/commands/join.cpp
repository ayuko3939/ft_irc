/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 15:55:16 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
    // 引数が1つまたは2つでない場合はエラー（チャンネル名が必須、キーは任意）
    if (words.size() != 1 && words.size() != 2)
    {
        addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(JOIN_USAGE));
        return false;
    }
    return true;
}

static bool	isValid(std::string word)
{
	if (word.empty() || (word.size() > 10))
		return (false);
	
	for (std::size_t i = 0; i < word.size(); i++)
	{
		if (!isalnum(word[i]))
			return (false);
	}
	return (true);
}

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void join(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	// 1. ユーザー入力をスペース区切りで取得し、引数の数をチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	if (!checkArguments(server, clientFd, words))
		return;

	// 2. チャンネル名リストとキーリストをカンマ区切りで取得
	std::vector<std::string> channelNames = splitByComma(words[0]);
	std::vector<std::string> keyList;
	if (words.size() > 1)
		keyList = splitByComma(words[1]);

	// キーリストのサイズがチャンネル数と合わなければ、不足分は空文字とする
	while (keyList.size() < channelNames.size())
		keyList.push_back("");

	// 3. クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);
	std::string clientNickname = client.getNickname();

	// 各チャンネルについて処理
	for (std::size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		std::string key = keyList[i];

		// 4. チャンネル名が空ならエラー
		if (!isValid(channelName))
		{
			addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(JOIN_USAGE));
			continue;
		}

		// 5. サーバーのチャンネルリストから対象チャンネルを探す（追加してitがずれる可能性があるので毎回取得する）
		std::map<std::string, Channel> &channels = server->getChannelList();
		std::map<std::string, Channel>::iterator it = channels.find(channelName);

		// 6. 存在しない場合は新規作成、存在する場合は既存のチャンネルに参加
		if (it == channels.end())
		{
			// チャンネルを作成
			server->addChannel(channelName);
			it = channels.find(channelName);

			// チャンネルの初期設定（オペレーター、クライアント、パスワード等）
			it->second.setOperatorList(clientFd);
			it->second.addClientToChannel(client);
			if (key != "")
			{
				it->second.setMode("+k");
				it->second.setPassword(key);
			}
		}
		else
		{
			// 既に参加している場合、エラーを返す
			if (it->second.getClientList().find(clientFd) != it->second.getClientList().end())
			{
				addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(clientNickname, clientNickname, channelName));
				continue;
			}

			// パスワード保護(+k)されている場合、キーの一致を確認する
			if (it->second.getMode().k)
			{
				if (key != it->second.getPassword())
				{
					addToClientSendBuf(server, clientFd, ERR_BADCHANNELKEY(clientNickname, channelName));
					continue;
				}
			}
		}

		// 4-4. 参加人数制限チェック
		if (it->second.getCapacity() != -1 &&
			static_cast<int>(it->second.getClientList().size()) >= it->second.getCapacity())
		{
			addToClientSendBuf(server, clientFd, ERR_CHANNELISFULL(clientNickname, channelName));
			continue;
		}

		// 3.g クライアントをチャンネルに追加
		server->addClientToChannel(channelName, client);


		// 3.i チャンネル参加後、JOINメッセージ、トピック、参加者一覧などの情報を送信
		// sendChanInfos(server, it->second, channelName, client);
	}
}
