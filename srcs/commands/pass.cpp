/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 15:54:57 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, const int clientFd, std::vector<std::string> &words)
{
	// 引数が足りなければエラーを返す
	if (words.size() < 1)
	{
		addToClientSendBuf(server, clientFd, ERR_NOT_ENOUGH_ARGS + std::string(PASS_USAGE));
		return (false);
	}
	// 引数が多すぎればエラーを返す
	else if (words.size() > 1)
	{
		addToClientSendBuf(server, clientFd, ERR_TOO_MANY_ARGS + std::string(PASS_USAGE));
		return (false);
	}
	return (true);
}

void pass(Server *server, const int clientFd,s_ircCommand cmdInfo)
{
	// クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);

	// 1. 既にパスワード認証が成功している場合、エラーを返す
	if (client.getConnexionPassword())
	{
		addToClientSendBuf(server, clientFd, ERR_ALREADYDONE(client.getNickname()));
		return ;
	}

	// 2. ユーザー入力をスペースで分割し、引数の数が正しいかチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	if (!checkArguments(server, clientFd, words))
		return ;

	// 3. 入力内容の妥当性チェック（ニックネームの文字数、文字種）
	std::string pssword = words[0];
	if (pssword != server->getPassword())
	{
		addToClientSendBuf(server, clientFd, ERR_PASSWDMISMATCH(client.getNickname()));
		return ;
	}

	// 4. パスワード認証成功
	client.setConnexionPassword();
	client.incrementNmInfo();

	// 5. 成功通知の送信
	addToClientSendBuf(server, clientFd, RPL_PASS(client.getNickname()));
}
