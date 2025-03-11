/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_helper.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:43:54 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/11 21:04:48 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setServerPollFd(std::vector<pollfd> &pollFds)
{
	pollfd	serverPollFd;

	serverPollFd.fd = _serverSockFd;
	serverPollFd.events = POLLIN; // POLLIN: データ読み込み可能

	pollFds.push_back(serverPollFd);
}

/*******************************************************************/
/*                       Client Management                         */
/*******************************************************************/

void Server::addClient(int clientFd, std::vector<pollfd> &tmpPollFds)
{
	pollfd	clientPollFd;
	Client	newClient(clientFd);

	// クライアントが送受信できるように設定し、tmpPollFdsに格納
	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN | POLLOUT; // 送受信
	tmpPollFds.push_back(clientPollFd);

	// クライアントリストに追加
	_clientList.insert(std::pair<int, Client>(clientFd, newClient));
	std::cout << INDIGO SERVER_ADD_CLIENT << clientFd << END << std::endl;
}

void Server::deleteClient(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	std::cout << INDIGO SERVER_DISCONNECT_CLIENT << clientFd << END << std::endl;
	
	// クライアントリストから削除し、ソケットを閉じる
	_clientList.erase(clientFd);
	close(clientFd);
	pollFds.erase(it);
	
	std::cout << INDIGO SERVER_NUMBER_OF_CLIENTS << pollFds.size() - 1 << END << std::endl;
}


/*******************************************************************/
/*                       Handle Connection                         */
/*******************************************************************/

int Server::handleNewConnection(std::vector<pollfd> &pollFds, std::vector<pollfd> &tmpPollFds)
{
	sockaddr_in		clientAddr;
	socklen_t		clientAddrLen = sizeof(sockaddr_in);

	// クライアントからの接続を受け入れる、失敗したらエラー出力（throwせずに処理を続行(continue)）
	int	clientFd = accept(_serverSockFd, (sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd < 0)
	{
		std::cerr << ORANGE ERROR_SERVER_ACCEPT END << std::endl;
		return (EXIT_FAILURE);
	}

	// サーバーが満員なら、エラー出力・クライアントにエラーを送信してソケットを閉じる
	if (pollFds.size() >= MAX_CLIENTS)
	{
		std::cerr << ORANGE ERROR_SERVER_FULL END << std::endl;
		send(clientFd, ERROR_SERVER_FULL, strlen(ERROR_SERVER_FULL) + 1, 0);
		close(clientFd);
	}
	// 空きがあれば、クライアントを追加する
	else
		addClient(clientFd, tmpPollFds);

	return (EXIT_SUCCESS);
}

int Server::handleClientData(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it)
{
	// 1. クライアント情報の取得
	Client *client = getClient(this, it->fd);
	if (!client)
	{
		std::cerr << ORANGE ERROR_CLIENT_NOT_FOUND << it->fd << END << std::endl;
		return (EXIT_FAILURE);
	}
	
	// 2. 受信用バッファの用意（メッセージの最大長さを指定）
	std::string message;
	message.resize(BUF_SIZE_MSG);

	// 3. recv() を使ってデータ受信
	size_t readSize = recv(it->fd, &message[0], BUF_SIZE_MSG, 0);
	
	// 4-a. エラー発生時の処理
	if (readSize < 0)
	{
		std::cerr << ORANGE ERROR_SERVER_RECV END << std::endl;
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
	// 4-b. クライアントが切断された場合の処理
	else if (readSize == 0)
	{
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
	// 4-c. データ受信成功時の処理
	else
	{
		// 受信したデータをバッファに格納（実際のメッセージの長さにリサイズ）
		message.resize(readSize);
		std::cout << "[Client] " << message << std::endl; // ===== ★後で表示を整える★ =====
		
		// クライアントのバッファに受信したメッセージを追記で格納
		client->setReadBuf(client->getReadBuf() + message);
		
		// 受信したデータに"\r\n"（IRCの改行）が含まれる含む場合、コマンドとして処理する
		if (client->getReadBuf().find("\r\n") != std::string::npos)
		{
			try
			{
				parseMessage(it->fd, client->getReadBuf());

				client->resetReadBuf();
			}
			catch(char const *mes)
			{
				// std::cerr << RED << mes << END << std::endl;
				
				if (client->isRegistrationDone())
					client->setToDeconnect();
				return (EXIT_FAILURE); // ★★★
			}
		}
	}
	return (EXIT_SUCCESS);
}


int Server::handlePollout(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	Client *client = getClient(this, clientFd);

	// クライアントが見つからなかった場合、エラー文を出力して何もしないで処理終了
	if (!client)
	{
		std::cerr << ORANGE ERROR_CLIENT_NOT_FOUND << clientFd << END << std::endl;
	}
	else
	{
		// クライアントのメッセージを送信して、バッファをクリアする
		sendServerReply(clientFd, client->getSendBuf());
		client->getSendBuf().clear();

		// クライアントが切断フラグを持っている場合は削除してbreak
		if (client->getToDeconnect())
		{
			deleteClient(pollFds, it, clientFd);
			return (1);
		}
	}
	return (EXIT_SUCCESS);
}

int Server::handlePollerr(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it)
{
	std::cerr << ORANGE ERROR_SERVER_POLL << it->fd << END << std::endl;
	
	// サーバーにエラーが発生した場合、強制終了
	if (it->fd == _serverSockFd)
	{
		throw (ERROR_SERVER_LISTEN);
	}
	// クライアントにエラーが発生した場合、クライアントを削除
	{
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
}

/*******************************************************************/
/*                             Parser          	                   */
/*******************************************************************/

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

	if (cmdInfo.name == "NICK")
		// nick(); // ★★★
	else if (cmdInfo.name == "USER")
		// user(); // ★★★
	else if (cmdInfo.name == "PASS")
	{
		// pass(); // ★★★
		if (pass() == 0)
			it->second.setConnexionPassword();
	}
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
			// errorMsg(); // ★★★
	}
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
					sendClientRegistrationMsg(clientFd, it);
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
