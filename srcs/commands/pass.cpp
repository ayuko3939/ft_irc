/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/04/04 21:42:58 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, const int clientFd, std::vector<std::string> &words)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数が1つであることを確認
	if (words.size() != 1 || words[0].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "PASS");
		errMessage += PASS_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// パスワードが一致しない場合
	if (words[0] != server->getPassword())
	{
		errMessage = ERR_PASSWDMISMATCH(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
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

	// 2. 入力内容の妥当性チェック
	if (!checkArguments(server, clientFd, words))
		return ;

	// 3. パスワード認証成功
	client.setConnexionPassword();
	client.incrementNmInfo();

	// 4. 成功通知の送信
	std::string notice = RPL_PASS();
	addToClientSendBuf(server, clientFd, notice);
}

/*
Numeric replies:
	ERR_NEEDMOREPARAMS (461)
	ERR_ALREADYREGISTERED (462)
	ERR_PASSWDMISMATCH (464)
*/