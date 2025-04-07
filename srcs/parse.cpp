/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:43:40 by yohasega          #+#    #+#             */
/*   Updated: 2025/04/04 21:43:33 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"
#include "Command.hpp"

static void splitCommandLine(std::string &message, std::vector<std::string> &cmds)
{
	// 改行コードを"\r\n"から"\n"に変換
	std::string::size_type pos = 0;
	while ((pos = message.find("\r\n", pos)) != std::string::npos)
	{
		message.replace(pos, 2, "\n");
		pos += 1;
	}

	// メッセージを１行ずつ分割してコマンドリストに格納
	std::istringstream	iss(message);
	std::string			line;

	while (std::getline(iss, line, '\n'))
		cmds.push_back(line);
}

// 処理例
//     ":nick!user@host PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG" をコマンド名として取得
// 　→ ":nick!user@host " をプレフィックスとして取得
// 　→ "#channel :Hello there\n" をメッセージとして取得
// 　→ "PRIVMSG" を大文字に変換
	
static int parseCommand(std::string &cmdLine, s_ircCommand &cmdInfo)
{
	// １．コマンドが空の場合、エラーを返す
	cmdLine = trim(cmdLine);
	if (cmdLine.empty())
	{
		std::cerr << ORANGE ERROR_SERVER_EMPTY_CMD END << std::endl;
		return (EXIT_FAILURE);
	}

	// ２．コマンドの先頭が":"（プレフィックス）の場合、コマンドの先頭までを取り除く
	std::string	copy = cmdLine;
	if (copy[0] == ':')
	{
		if (cmdLine.find(' ') != std::string::npos)
			copy.erase(0, copy.find(' ') + 1);
	}
	
	// ３．コマンド名を取得（スペースがない場合、全体がコマンド名、ある場合は先頭からスペースまで）
	std::string::size_type firstSpace = copy.find(' ');
	if (firstSpace == std::string::npos)
		cmdInfo.name.assign(copy);
	else
		cmdInfo.name.assign(copy, 0, firstSpace);
	
	// ４．プレフィックスを取得（コマンド名が始まる位置まで）
	std::string::size_type commandPos = cmdLine.find(cmdInfo.name);
	if (commandPos != std::string::npos)
		cmdInfo.prefix.assign(cmdLine, 0, commandPos);
	else
		cmdInfo.prefix = "";

	// ５．メッセージ部分を取得（コマンド名の後ろから最後まで）
	std::string::size_type msgPos = commandPos + cmdInfo.name.size();
	if (msgPos < cmdLine.size())
	{
		cmdInfo.message = cmdLine.substr(msgPos);
		cmdInfo.message = trim(cmdInfo.message);
	}
	else
		cmdInfo.message = "";
	
	// ６．コマンド名を大文字に変換
	for (std::string::size_type i = 0; i < cmdInfo.name.size(); ++i)
		cmdInfo.name[i] = std::toupper(cmdInfo.name[i]);
	
	// DEBUG
	// std::cout << GREEN;
	// std::cout << "prefix: " << cmdInfo.prefix << std::endl;
	// std::cout << "name: " << cmdInfo.name << std::endl;
	// std::cout << "message: " << cmdInfo.message << std::endl;
	// std::cout << END;

	return (EXIT_SUCCESS);
}

static void sendClientRegistrationMsg(Server *server, int clientFd, Client *client)
{
	addToClientSendBuf(server, clientFd, DELIMITER_LINE);
	addToClientSendBuf(server, clientFd, RPL_WELCOME(client->getNickname(), client->getNickname()));
	addToClientSendBuf(server, clientFd, RPL_YOURHOST(client->getNickname(), "ircserv", "1.0"));
	addToClientSendBuf(server, clientFd, RPL_CREATED(client->getNickname(), server->getDateTime()));
	addToClientSendBuf(server, clientFd, RPL_MYINFO(client->getNickname(), "ircserv", "1.0"));
	addToClientSendBuf(server, clientFd, DELIMITER_LINE);
}

void Server::fillClientInfo(Client *client, int clientFd, s_ircCommand cmdInfo)
{
	// irssiからの"CAP LS"リクエストを無視
	if (cmdInfo.name == "CAP" && cmdInfo.message.find("LS") != std::string::npos)
	{
		// CAP LS に対して空の応答を返す（または必要な機能を指定する）
		addToClientSendBuf(this, clientFd, "CAP * LS :\r\n");
		return;
	}

	// パスワード認証が完了していない場合
	if (!client->getConnexionPassword())
	{
		if (cmdInfo.name != "PASS")
			addToClientSendBuf(this, clientFd, ERR_PASS_AUTH_YET);
		else
			pass(this, clientFd, cmdInfo);
		return ;
	}
	else
	{
		// クライアント情報が全て揃っていない場合、クライアント情報を取得する
		if (cmdInfo.name != "NICK" && cmdInfo.name != "USER")
			addToClientSendBuf(this, clientFd, ERR_NOTREGISTERED);
		else
		{
			if (cmdInfo.name == "NICK")
				nick(this, clientFd, cmdInfo);
			else if (cmdInfo.name == "USER")
				user(this, clientFd, cmdInfo);
		}

		// クライアント情報が全て揃った場合、登録処理を行う
		if (client->getNmInfo() == 3 && client->isRegistrationDone() == false)
		{
			sendClientRegistrationMsg(this, clientFd, client);
			client->setRegistrationDone();
		}
		return ;
	}
}

void Server::execCommand(int clientFd, s_ircCommand	&cmdInfo)
{
	Client	*client = getClient(clientFd);

	// 登録処理が完了していない場合
	if (!client->isRegistrationDone())
	{
		fillClientInfo(client, clientFd, cmdInfo);
		return ;
	}

	// コマンドリストからコマンドを検索
	int	type = getCommandType(cmdInfo.name);
	// std::cout << GREEN "[execCommand] type: " << type << END << std::endl;

	// コマンドに応じた処理を実行
	switch (type)
	{
		case 1: break; // CAP
		case 2: invite(this, clientFd, cmdInfo); break;
		case 3: join(this, clientFd, cmdInfo); break;
		case 4: kick(this, clientFd, cmdInfo); break;
		case 5: mode(this, clientFd, cmdInfo); break;
		case 6: nick(this, clientFd, cmdInfo); break;
		case 7: part(this, clientFd, cmdInfo); break;
		case 8: pass(this, clientFd, cmdInfo); break;
		case 9: break; //PING
		case 10: privmsg(this, clientFd, cmdInfo); break;
		case 11: quit(this, clientFd, cmdInfo); break;
		case 12: topic(this, clientFd, cmdInfo); break;
		case 13: user(this, clientFd, cmdInfo); break;

		// コマンドが見つからない場合、エラー文を出力して何もしないで処理終了
		default:
			addToClientSendBuf(this, clientFd, ERR_UNKNOWNCOMMAND(client->getNickname(), cmdInfo.name));
	}
}

void Server::parseExecCommand(int clientFd, std::string &message)
{
	std::vector<std::string>	cmds;
	
	// 改行毎にメッセージを分割してコマンドリストに格納
	splitCommandLine(message, cmds);
	
	// コマンドリストを順番に処理
	std::vector<std::string>::iterator cmdIt = cmds.begin();
	for ( ; cmdIt != cmds.end(); ++cmdIt)
	{
        s_ircCommand cmdInfo;

        // コマンド解析
        if (parseCommand(*cmdIt, cmdInfo) == EXIT_FAILURE)
            continue;
		// コマンド実行
		execCommand(clientFd, cmdInfo);
	}
}
