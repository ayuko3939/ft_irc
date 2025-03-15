/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/15 15:38:11 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::vector<std::string> splitMessage(std::string message)
{
    std::istringstream iss(message);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word)
    {
        trim(word);
        words.push_back(word);
    }
    return words;
}

static std::vector<std::string> splitByComma(const std::string &str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ','))
    {
        trim(token);
        if (!token.empty())
            result.push_back(token);
    }
    return result;
}

static bool checkArgumentsNum(Server *server, int clientFd, std::vector<std::string> &words)
{
    // 引数が1つまたは2つでない場合はエラー（チャンネル名が必須、キーは任意）
    if (words.size() != 1 && words.size() != 2)
    {
        addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(JOIN_USAGE));
        return false;
    }
    return true;
}

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void join(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	// 1. ユーザー入力を空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 引数の数チェック
	if (!checkArgumentsNum(server, clientFd, words))
		return;

	// クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);
	std::string clientNickname = client.getNickname();

	// 2. チャンネル名リストとキーリストをカンマ区切りで取得
	std::vector<std::string> channelNames = splitByComma(words[0]);
	std::vector<std::string> keyList;
	if (words.size() > 1)
		keyList = splitByComma(words[1]);

	// キーリストのサイズがチャンネル数と合わなければ、不足分は空文字とする
	while (keyList.size() < channelNames.size())
		keyList.push_back("");

	// 3. 各チャンネルについて処理
	for (std::size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		std::string key = keyList[i];

		// 3-1. チャンネル名が空ならエラー
		if (channelName.empty())
		{
			addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(JOIN_USAGE));
			continue;
		}

		// 3-2. サーバーのチャンネルリストから対象チャンネルを探す
		std::map<std::string, Channel> &channels = server->getChannelList();
		std::map<std::string, Channel>::iterator it = channels.find(channelName);

		// 存在しない場合は新規作成
		if (it == channels.end())
		{
			server->addChannel(channelName);
			it = channels.find(channelName);
		}
		// 
		else
		{
			// 3.a 既に参加している場合、エラーを返す
			if (it->second.getClientList().find(clientFd) != it->second.getClientList().end())
			{
				addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(clientNickname, clientNickname, channelName));
				continue;
			}
			{
				addToClientSendBuf(server, clientFd, ERR_USERONCHANNEL(clientNickname, channelName));
				continue;
			}
			// 3.d パスワード保護(+k)されている場合、キーの一致を確認する
			if (it->second.getMode().find("k") != std::string::npos)
			{
				if (key != it->second.getChannelPassword())
				{
					addToClientSendBuf(server, clientFd, ERR_BADCHANNELKEY(clientNickname, channelName));
					continue;
				}
			}
		}

		// 3.e 参加人数制限チェック
		if (it->second.getMaxConnections() != -1 &&
			static_cast<int>(it->second.getClientList().size()) >= it->second.getMaxConnections())
		{
			addToClientSendBuf(server, clientFd, ERR_CHANNELISFULL(clientNickname, channelName));
			continue;
		}

		// 3.f BANチェックは省略（BAN機能未対応）

		// 3.g クライアントをチャンネルに追加
		// ユーザー管理はニックネームをキーとして行うので、内部で client.getNickname() を使用する
		addClientToChannel(server, channelName, client);

		// 3.h オペレーターが未設定なら、最初のオペレーターとして設定
		if (it->second.getOperatorList().empty())
			it->second.addFirstOperator(clientNickname);

		// 3.i チャンネル参加後、JOINメッセージ、トピック、参加者一覧などの情報を送信
		sendChanInfos(server, it->second, channelName, client);
	}
}
