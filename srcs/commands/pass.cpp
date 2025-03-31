/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/31 19:22:21 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, const int clientFd, 
	std::vector<std::string> &words, std::string &password)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数が1つであることを確認
	if (words.empty() || words[0].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "PASS");
		errMessage += PASS_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	// パスワードが一致しない場合
	password = words[0];
	if (password != server->getPassword())
	{
		errMessage = ERR_PASSWDMISMATCH(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}
	return (true);
}

void pass(Server *server, const int clientFd,s_ircCommand cmdInfo)
{
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();
	std::string errMessage = "";

	// 1. 既にパスワード認証が成功している場合、エラーを返す
	if (client.getConnexionPassword())
	{
		errMessage = ERR_ALREADYREGISTERED(clientNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	std::vector<std::string> words = splitMessage(cmdInfo.message);
	std::string password = "";

	// 2. 入力内容の妥当性チェック
	if (!checkArguments(server, clientFd, words, password))
		return ;

	// 3. パスワード認証成功
	client.setConnexionPassword();
	client.incrementNmInfo();

	// 4. 成功通知の送信
	std::string notice = RPL_PASS();
	addToClientSendBuf(server, clientFd, notice);
	// std::cout << INDIGO "[Server] Password authentication success" END << std::endl;
}

/*
Numeric replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)
	ERR_PASSWDMISMATCH (464)
*/