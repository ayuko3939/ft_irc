/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 20:15:20 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password, struct tm *timeinfo) :
_serverSockFd(0),
_port(port),
_Password(password),
_dateTime(""),
_motd(""),
_serverInfo(nullptr)
{
}

Server::~Server()
{
}


// Setters
// void Server::setServerSockFd(int serverSockFd) { _serverSockFd = serverSockFd; }
// void Server::setClientList(const std::map<const int, Client> &clientList) { _clientList = clientList; }
// void Server::setChannelList(const std::map<std::string, Channel> &channelList) { _channelList = channelList; }
// void Server::setPort(const std::string &port) { _port = port; }
// void Server::setPassword(const std::string &password) { _Password = password; }
// void Server::setDateTime(const std::string &dateTime) { _dateTime = dateTime; }
// void Server::setMotd(const std::string &motd) { _motd = motd; }
// void Server::setAddrInfo(const struct addrinfo &addrInfo) { _addrInfo = addrInfo; }
// void Server::setServerInfo(struct addrinfo* serverInfo) { _serverInfo = serverInfo; } 

// Getter
// int Server::getServerSockFd() const { return (_serverSockFd); }
// const std::map<const int, Client>& Server::getClientList() const { return (_clientList); }
// const std::map<std::string, Channel>& Server::getChannelList() const { return (_channelList); }
// const std::string &Server::getPort() const { return (_port); }
// const std::string &Server::getPassword() const { return (_Password); }
// const std::string &Server::getDateTime() const { return (_dateTime); }
// const std::string &Server::getMotd() const { return (_motd); }
// const struct addrinfo &Server::getAddrInfo() const { return (_addrInfo); }
// struct addrinfo* Server::getServerInfo() const { return (_serverInfo); }


// Other member functions (prototypes only)

void Server::launchServer()
{
    // TODO: Implementation to be provided later.
}

void Server::manageServerLoop()
{
    // TODO: Implementation to be provided later.
}
