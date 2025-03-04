/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 23:26:03 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password, struct tm *timeinfo) :
_serverSockFd(-1),
_clientList(),
_channelList(),
_port(port),
_password(password),
_serverInfo(NULL)
{
    std::cout << INDIGO "Server running..." END << std::endl;
    
    // 時間を文字列として取得（2042-4-2 4:2:42）
    char buf[80];
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    _dateTime = std::string(buf);

    // サーバーの初期設定（アドレス情報を設定）
    memset(&_addrInfo, 0, sizeof(_addrInfo));
    _addrInfo.ai_family = AF_INET;       // IPv4
    _addrInfo.ai_socktype = SOCK_STREAM; // TCP
    _addrInfo.ai_flags = AI_PASSIVE;     // ローカルホストで動作
}

Server::~Server()
{
}


// Setters
// void Server::setServerSockFd(int serverSockFd) { _serverSockFd = serverSockFd; }
// void Server::setClientList(const std::map<const int, Client> &clientList) { _clientList = clientList; }
// void Server::setChannelList(const std::map<std::string, Channel> &channelList) { _channelList = channelList; }
// void Server::setPort(const std::string &port) { _port = port; }
// void Server::setPassword(const std::string &password) { _password = password; }
// void Server::setDateTime(const std::string &dateTime) { _dateTime = dateTime; }
// void Server::setAddrInfo(const struct addrinfo &addrInfo) { _addrInfo = addrInfo; }
// void Server::setServerInfo(struct addrinfo* serverInfo) { _serverInfo = serverInfo; } 

// Getter
// int Server::getServerSockFd() const { return (_serverSockFd); }
// const std::map<const int, Client>& Server::getClientList() const { return (_clientList); }
// const std::map<std::string, Channel>& Server::getChannelList() const { return (_channelList); }
// const std::string &Server::getPort() const { return (_port); }
// const std::string &Server::getPassword() const { return (_password); }
// const std::string &Server::getDateTime() const { return (_dateTime); }
// const struct addrinfo &Server::getAddrInfo() const { return (_addrInfo); }
// struct addrinfo* Server::getServerInfo() const { return (_serverInfo); }


// Other member functions (prototypes only)

void Server::initServer()
{
    
}

// void Server::manageServerLoop()
// {
//     // TODO: Implementation to be provided later.
// }
