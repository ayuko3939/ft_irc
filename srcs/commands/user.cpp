/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/13 17:12:57 by yohasega         ###   ########.fr       */
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

static bool checkArgumentsNum(Server *server, int clientFd, std::vector<std::string> &words)
{
	// 引数が足りなければエラーを返す
	if (words.size() < 2)
	{
		addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(USER_USAGE));
		return (false);
	}
	// 引数が多すぎればエラーを返す
	else if (words.size() > 2)
	{
		addToClientSendBuf(server, clientFd, ERR_TOO_MANY_ARGS + std::string(USER_USAGE));
		return (false);
	}
	return (true);
}

static bool	isValidName(std::string username, std::string realname)
{
	if (username.empty() || (realname.size() > 10) ||
		realname.empty() || (realname.size() > 10))
		return (false);
	
	for (std::size_t i = 0; i < username.size(); i++)
	{
		if (!isalnum(username[i]))
			return (false);
	}
	for (std::size_t i = 0; i < realname.size(); i++)
	{
		if (!isalpha(realname[i]))
			return (false);
	}
	return (true);
}

static bool	isAlreadyUsed(Server *server, int clientFd, std::string nickname)
{
	std::map<int, Client>::iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it)
	{
		if (it->first != clientFd && it->second.getNickname() == nickname)
			return (true);
	}
	return (false);
}

void user(Server *server, int clientFd,s_ircCommand cmdInfo)
{
	// クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);

	// 1. 既に登録済みの場合はエラーを返す
	if (!client.getUserName().empty())
	{
		addToClientSendBuf(server, clientFd, ERR_ALREADYREGISTERED(client.getNickname()));
		return ;
	}

	// 2. ユーザー入力をスペースで分割し、引数の数が正しいかチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	if (!checkArgumentsNum(server, clientFd, words))
		return ;

	// 3. 入力内容の妥当性チェック（ニックネームの文字数、文字種）
	std::string username = words[0];
	std::string realname = words[1];
	if (!isValidName(username, realname))
	{
		addToClientSendBuf(server, clientFd, USER_REQUIREMENTS);
		return ;
	}

	// 4. 重複チェック（他のユーザーが既に使用しているか）
	if (isAlreadyUsed(server, clientFd, username)) {
		addToClientSendBuf(server, clientFd, ERR_USERNAMEINUSE(client.getNickname(), username));
		return ;
	}

	// 7. ニックネームの更新
	client.setUserName(username);
	client.setRealName(realname);
	client.incrementNmInfo();

	// 8. 成功通知の送信
	addToClientSendBuf(server, clientFd, RPL_USER(client.getNickname(), username, realname));
}
