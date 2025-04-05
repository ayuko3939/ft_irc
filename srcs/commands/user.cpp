/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 18:48:23 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// user user 127.0.0.1 :real name
static std::string getRealname(std::string &cmdLine, std::string &thirdArgument)
{
	std::size_t pos = cmdLine.find(":");
	if (pos == std::string::npos)
	{
		pos = cmdLine.find(thirdArgument);
		if (pos == std::string::npos)
			return ("");
		return (cmdLine.substr(pos));
	}

	return (cmdLine.substr(pos + 1));
}

static bool	isValid(std::string username, std::string realname)
{
	if (username.empty() || (username.size() > USERNAMELEN) ||
		realname.empty() || (realname.size() > REALNAMELEN))
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

static bool checkAndGetArguments(Server *server, int clientFd, 
	std::string &cmdLine , std::string &username, std::string &realname)
{
	std::vector<std::string> words = splitMessage(cmdLine);
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数が5つであることを確認
	if (words.size() < 4)
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "USER");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE USER_USAGE END << std::endl;
		return (false);
	}

	// ユーザー名と実名の取得
	username = words[0];
	realname = getRealname(cmdLine, words[3]);

	// ユーザー名と実名の妥当性チェック
	if (!isValid(username, realname))
	{
		errMessage = ERR_INVALID_PARM;
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << GUIDE USER_REQUIREMENTS END << std::endl;
		return (false);
	}
	return (true);
}

void user(Server *server, const int clientFd,s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();
	std::string errMessage = "";

	// 1. 既に登録済みの場合はエラーを返す
	if (!client.getUserName().empty())
	{
		errMessage = ERR_ALREADYREGISTERED(clientNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	std::string username = "";
	std::string realname = "";

	// 2. ユーザー入力から引数を取得し、引数をチェック
	if (!checkAndGetArguments(server, clientFd, cmdInfo.message, username, realname))
		return ;

	// 3. ユーザー名と実名の設定
	client.setUserName(username);
	client.setRealName(realname);
	client.incrementNmInfo();

	// 4. 成功通知の送信
	std::string notice = RPL_USER(IRC_PREFIX(clientNick, username), username, realname);
	addToClientSendBuf(server, clientFd, notice);
}

/*
Numeric Replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)
*/