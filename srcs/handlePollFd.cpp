/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePollFd.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:43:54 by yohasega          #+#    #+#             */
/*   Updated: 2025/04/09 18:01:04 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

// 新規クライアントの接続を処理する
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

	// 非ブロッキングモードに設定（サーバーが応答しない状態になるのを防ぎ、複数の接続が同時に処理される）
	// レビュー用
	// if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	// 	throw (ERROR_SERVER_SETSOCKETOPT);
	// 理想の実装
	int flags = fcntl(clientFd, F_GETFL, 0);
	if (flags == -1)
		throw (ERROR_SERVER_SETSOCKETOPT);
	if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw (ERROR_SERVER_SETSOCKETOPT);

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

// 既存クライアントからのデータ受信を処理する
int Server::handleClientData(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it)
{
	// 1. クライアント情報の取得
	Client *client = getClient(it->fd);
	if (!client)
	{
		std::cerr << ORANGE ERROR_CLIENT_NOT_FOUND << it->fd << END << std::endl;
		return (EXIT_FAILURE);
	}
	
	// 2. 受信用バッファの用意（メッセージの最大長さを指定）
	std::string message;
	message.resize(BUF_SIZE_MSG);

	// 3. recv() を使ってデータ受信
	int readSize = recv(it->fd, &message[0], BUF_SIZE_MSG, 0);
	
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

		if (message.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
			std::cout << YELLOW "[Client] " << it->fd << " < " << message << END << std::endl;
		
		// クライアントのバッファに受信したメッセージを追記で格納
		client->setReadBuf(client->getReadBuf() + message);
		
		// 受信したデータに"\r\n"（IRCの改行）が含まれる場合、コマンドとして処理する
		if (client->getReadBuf().find("\r\n") != std::string::npos)
		{
			try
			{
				parseExecCommand(it->fd, client->getReadBuf());

				client->resetReadBuf();
			}
			catch(char const *mes)
			{
				std::cerr << RED << mes << END << std::endl;
				if (client->isRegistrationDone())
					client->setToDeconnect();
				return (EXIT_FAILURE);
			}
		}
	}
	return (EXIT_SUCCESS);
}

// クライアントにデータを送信する処理
int Server::handlePollout(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	Client *client = getClient(clientFd);

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

// クライアントにエラーが発生した場合の処理
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
