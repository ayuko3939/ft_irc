/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/13 17:52:32 by yohasega         ###   ########.fr       */
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
    void execCommand(int clientFd, std::string &cmd);
    void fillClientInfo(int clientFd, std::string &cmd);

    void addChannel(std::string &channelName);
    void addClientToChannel(std::string &channelName, Client &client);
    bool isChannelExist(std::string &channelName);
    
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
    // int getServerSockFd();
    // std::string &getPort();
    std::string &getPassword();
    std::string &getDateTime();
    // std::vector<serverOperator> &getOperatorList();
    std::map<const int, Client>& getClientList();
    // std::map<std::string, Channel>& getChannelList();
    // struct addrinfo &getAddrInfo();
    // struct addrinfo* getServerInfo();

    // Other functions
    void readConfigFile();
    void getServerInfo();
    void launchServer();
    void manageServerLoop();
};

// utils
std::string trim(const std::string &s);
Client *getClient(Server *server, int clientSockFd);
Client &retrieveClient(Server *server, int clientSockFd);
void splitMessage(std::string &message, std::vector<std::string> &cmds);
void sendServerReply(int clientFd, std::string &message);
void addToClientSendBuf(Server *server, int clientFd, std::string message);
void sendClientRegistrationMsg(Server *server, int clientFd,std::map<const int, Client>::iterator &it);
std::string getChannelMemberList(std::string client, Channel &channel);
std::string getChannelName(std::string &msgToParse);
std::string getSymbol(Channel &channel);
bool getCommandType(std::string &cmd);

#endif