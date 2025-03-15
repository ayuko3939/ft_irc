/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/15 12:08:40 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::vector<std::string>	splitMessage(std::string message)
{
	std::istringstream iss(message);
	std::vector<std::string> words;
	std::string word;

	while (iss >> word)
	{
		trim(word);
		words.push_back(word);
	}

	return (words);
}

static bool checkArgumentsNum(Server *server, const int clientFd, std::vector<std::string> &words)
{
	// 引数が足りなければエラーを返す
	if (words.size() < 1)
	{
		addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(NICK_USAGE));
		return (false);
	}
	// 引数が多すぎればエラーを返す
	else if (words.size() > 1)
	{
		addToClientSendBuf(server, clientFd, ERR_TOO_MANY_ARGS + std::string(NICK_USAGE));
		return (false);
	}
	return (true);
}

static bool	isValidNickname(std::string nickname)
{
	if (nickname.empty() || (nickname.size() > 10))
		return (false);
	
	for (std::size_t i = 0; i < nickname.size(); i++)
	{
		if (!isalnum(nickname[i]))
			return (false);
	}
	return (true);
}

static bool	isAlreadyUsed(Server *server, const int clientFd, std::string nickname)
{
	std::map<int, Client>::iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it)
	{
		if (it->first != clientFd && it->second.getNickname() == nickname)
			return (true);
	}
	return (false);
}

void nick(Server *server, const int clientFd, s_ircCommand cmdInfo)
{
	// 1. ユーザー入力をスペースで分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 引数の数が正しいかチェック
	if (!checkArgumentsNum(server, clientFd, words))
		return ;

	// クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);

	// 2. 入力内容の妥当性チェック（ニックネームの文字数、文字種）
	std::string newNick = words[0];
	if (!isValidNickname(newNick))
	{
		addToClientSendBuf(server, clientFd, ERR_ERRONEUSNICKNAME(client.getNickname(), newNick) + NICK_REQUIREMENTS);
		return ;
	}

	// 3. 現在のニックネームとの比較（同じ場合は何もしない）
	if (client.getNickname() == newNick)
		return ;

	// 4. 重複チェック（他のユーザーが既に使用しているか）
	if (isAlreadyUsed(server, clientFd, newNick)) {
		addToClientSendBuf(server, clientFd, ERR_NICKNAMEINUSE(client.getNickname(), newNick));
		return ;
	}

	// 5. ニックネームの更新
	// 未設定の場合は新しいニックネームを設定
	if (client.getNickname().empty())
	{
		client.setNickname(newNick);
		client.setOldNickname(newNick);
		client.incrementNmInfo();
	}
	// 現在のニックネームを旧ニックネームとして保持し、新しいニックネームに更新
	else
	{
		std::string oldNick = client.getNickname();
		client.setNickname(newNick);
	}

	// 6. 成功通知の送信
	addToClientSendBuf(server, clientFd, RPL_NICK(client.getNickname(), newNick));
}
