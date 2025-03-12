/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:43:40 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/12 16:14:38 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"
#include "Command.hpp"

void splitMessage(std::string &message, std::vector<std::string> &cmds)
{
	// 改行コードを"\r\n"から"\n"に変換
	std::string			replaced;
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


int parseCommand(std::string &cmdLine, s_ircCommand &cmdInfo)
{
	// 処理例
	//     ":nick!user@host PRIVMSG #channel :Hello there\n"
	// 　→ "PRIVMSG #channel :Hello there\n"
	// 　→ "PRIVMSG" をコマンド名として取得
	// 　→ ":nick!user@host " をプレフィックスとして取得
	// 　→ "#channel :Hello there\n" をメッセージとして取得
	// 　→ "PRIVMSG" を大文字に変換
	
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
	for (std::string::size_type i = 0; i < cmdInfo.message.size(); ++i)
		cmdInfo.name[i] = std::toupper(cmdInfo.name[i]);
	
	// DEBUG
	std::cout << GREEN;
	std::cout << "prefix: " << cmdInfo.prefix << std::endl;
	std::cout << "name: " << cmdInfo.name << std::endl;
	std::cout << "message: " << cmdInfo.message << std::endl;
	std::cout << END;

	return (EXIT_SUCCESS);
}

void Server::fillClientInfo(std::map<const int, Client> &clientList, int clientFd, std::string &cmd)
{
	std::map<const int, Client>::iterator it = clientList.find(clientFd);
	s_ircCommand			cmdInfo;

	if (parseCommand(cmd, cmdInfo) == false)
		return ;

	// if (cmdInfo.name == "NICK")
	// 	// nick(); // ★★★
	// else if (cmdInfo.name == "USER")
	// 	// user(); // ★★★
	// else if (cmdInfo.name == "PASS")
	// {
	// 	// pass(); // ★★★
		// if (pass() == 0)
			it->second.setConnexionPassword();
	// }
}

void Server::execCommand(int clientFd, std::string &cmd)
{
	std::string	cmdList[NUM_OF_CMD] = {
		"INVITE", "JOIN", "KICK","MODE", "NICK", "PART", "PING",
		"PRIVMSG", "QUIT", "TOPIC", "USER"
	};
	
	Client	*client = getClient(this, clientFd);
	s_ircCommand	cmdInfo;
	int	i = 0;
	
	// コマンド解析
	if (parseCommand(cmd, cmdInfo) == false)
		return ;
	
	// コマンドリストからコマンドを検索
	while (i < NUM_OF_CMD)
	{
		if (cmdInfo.name == cmdList[i])
			break;
		i++;
	}
		
	// コマンドに応じた処理を実行
	switch (i + 1)
	{
		case 1: // invite(this, clientFd, cmdInfo); break; // ★★★
		case 2: // join(this, clientFd, cmdInfo); break; // ★★★
		case 3: // kick(this, clientFd, cmdInfo); break; // ★★★
		case 4: // mode(this, clientFd, cmdInfo); break; // ★★★
		case 5: // nick(this, clientFd, cmdInfo); break; // ★★★
		case 6: // part(this, clientFd, cmdInfo); break; // ★★★
		case 7: // ping(this, clientFd, cmdInfo); break; // ★★★
		case 8: // privmsg(this, clientFd, cmdInfo); break; // ★★★
		case 9: // quit(this, clientFd, cmdInfo); break; // ★★★
		case 10: // topic(this, clientFd, cmdInfo); break; // ★★★
		case 11: // user(this, clientFd, cmdInfo); break; // ★★★

		// コマンドが見つからない場合、エラー文を出力して何もしないで処理終了
		default:
			; // errorMsg(); // ★★★
	}



	(void)client;
}


void Server::parseMessage(int clientFd, std::string &message)
{
	std::vector<std::string>	cmds;
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	
	// 改行毎にメッセージを分割してコマンドリストに格納
	splitMessage(message, cmds);
	
	// コマンドリストを順番に処理
	std::vector<std::string>::iterator cmdIt = cmds.begin();
	for ( ; cmdIt != cmds.end(); ++cmdIt)
	{
		std::string &cmd = *cmdIt;
		
		// 登録が完了できていない場合、クライアント情報を取得する
		if (!it->second.isRegistrationDone())
		{
			// クライアント情報が全て揃っていない場合
			if (it->second.getHasAllInfo() == false)
			{
				fillClientInfo(_clientList, clientFd, cmd); // ★★★
				if (it->second.getNmInfo() == 3)
					it->second.setHasAllInfo();
			}
			else if (it->second.isRegistrationDone() == false)
			{
				if (it->second.isValid())
				{
					sendClientRegistrationMsg(this, clientFd, it);
					it->second.setRegistrationDone();
				}
				else
					throw (ERROR_CLIENT_INVALID_INFO);
			}
		}
		// 登録が完了している場合、コマンドを処理する
		else
			execCommand(clientFd, cmd);
	}
}

