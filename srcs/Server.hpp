/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/08 19:17:09 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# include <string>
# include <vector>
# include <netdb.h>

struct serverOperator
{
  std::string name;
  std::string host;
  std::string password;
};

class Server
{
  private:
    int							              	_serverSockFd;
    std::string						          _port;
    std::string					          	_password;
    std::string					          	_dateTime;
    std::vector<serverOperator>     _operatorList;
    std::map<const int, Client>		  _clientList;
    std::map<std::string, Channel>	_channelList;
    struct addrinfo				        	_addrInfo; // アドレス情報
    struct addrinfo*			        	_serverInfo; // サーバー情報

    // manageServerLoop
    void setServerPollFd(std::vector<pollfd> &pollFds);

    int handleNewConnection(std::vector<pollfd> &pollFds, std::vector<pollfd> &tmpPollFds);
    int handleClientData(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it);
    int handlePollout(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientSockFd);
    int handlePollerr(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it);

    void addClient(int clientSockFd, std::vector<pollfd> &tmpPollFds);
    void deleteClient(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientSockFd);
    
    void parseMessage(int clientFd, std::string &message);

  public:
	  Server(std::string port, std::string password, struct tm *timeinfo);
	  ~Server();

    // Setters
    // void setServerSockFd(int serverSockFd);
    // void setPort(const std::string &port);
    // void setPassword(const std::string &password);
    // void setDateTime(const std::string &dateTime);
    // void setOperatorList(std::vector<serverOperator> &operatorList);
    // void setClientList(const std::map<const int, Client> &clientList);
    // void setChannelList(const std::map<std::string, Channel> &channelList);
    // void setAddrInfo(const struct addrinfo &addrInfo);
    // void setServerInfo(struct addrinfo* serverInfo);

    // Getters
    // int getServerSockFd() const;
    // const std::string &getPort() const;
    // const std::string &getPassword() const;
    // const std::string &getDateTime() const;
    // const std::vector<serverOperator> &getOperatorList() const;
    std::map<const int, Client>& getClientList() const;
    // const std::map<std::string, Channel>& getChannelList() const;
    // const struct addrinfo &getAddrInfo() const;
    // struct addrinfo* getServerInfo() const;

    // Other functions
    void readConfigFile();
    void getServerInfo();
    void launchServer();
    void manageServerLoop();
};

// utils
Client *getClient(Server *server, int clientSockFd);

#endif