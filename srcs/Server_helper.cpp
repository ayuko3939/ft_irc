/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_helper.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:43:54 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/08 18:05:11 by yohasega         ###   ########.fr       */
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
	
	_clientList.erase(clientFd);
	close(clientFd);
	pollFds.erase(it);
	
	std::cout << INDIGO SERVER_NUMBER_OF_CLIENTS << pollFds.size() - 1 << END << std::endl;
}


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
