/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/27 16:24:30 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, int clientFd, std::string cmdLine)
{
	std::vector<std::string> words = splitMessage(cmdLine);

	// 引数が5つであることを確認
	if (words.size() < 4)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(USER_USAGE));
		return (false);
	}
	return (true);
}

static std::string getUsername(std::string &cmdLine)
{
	std::vector<std::string> words = splitMessage(cmdLine);
	return (words[0]);	
}

// yohasega yohasega 127.0.0.1 :Yoka Hasegawa
static std::string getRealname(std::string &cmdLine)
{
	std::size_t pos = cmdLine.find(":");
	if (pos == std::string::npos)
	{
		std::vector<std::string> words = splitMessage(cmdLine);
		return (words[3]);
	}

	return (cmdLine.substr(pos + 1));
}

static bool	isValid(std::string username, std::string realname)
{
	if (username.empty() || (realname.size() > 20) ||
		realname.empty() || (realname.size() > 20))
		return (false);
	
	for (std::size_t i = 0; i < username.size(); i++)
	{
		if (!isalnum(username[i]))
			return (false);
	}
	for (std::size_t i = 0; i < realname.size(); i++)
	{
		if (!isalpha(realname[i]) && realname[i] != ' ')
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

void user(Server *server, const int clientFd,s_ircCommand cmdInfo)
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
	if (!checkArguments(server, clientFd, cmdInfo.message))
		return ;

	// 3. 入力内容の妥当性チェック（ニックネームの文字数、文字種）
	std::string username = getUsername(cmdInfo.message);
	std::string realname = getRealname(cmdInfo.message);
	if (!isValid(username, realname))
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
	// addToClientSendBuf(server, clientFd, RPL_USER(client.getNickname(), username, realname));
	std::cout << INDIGO "[Server] User registration success" END << std::endl;
}
