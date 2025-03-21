/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/17 15:32:45 by yohasega         ###   ########.fr       */
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
    static bool											_signal;
  
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
    void fillClientInfo(Client *client, int clientFd, s_ircCommand cmdInfo);
    
  public:
	  Server(std::string port, std::string password, struct tm *timeinfo);
	  ~Server();

    static void signalHandler(int signum);

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
    std::map<std::string, Channel>& getChannelList();
    // struct addrinfo &getAddrInfo();
    // struct addrinfo* getServerInfo();
    int getClientFdFromNick(std::string &nick);


    // Other functions
    void readConfigFile();
    void getServerInfo();
    void launchServer();
    void manageServerLoop();

    void addChannel(std::string &channelName);
    void addClientToChannel(std::string &channelName, Client &client);
    bool isChannelExist(std::string &channelName);
    bool isClientExist(int clientFd);
    bool isClientExist(std::string &nick);
    std::string getNickname(int clientFd);
};

// utils
std::string trim(const std::string &s);
Client *getClient(Server *server, int clientSockFd);
Client &retrieveClient(Server *server, int clientSockFd);
void splitMessage(std::string &message, std::vector<std::string> &cmds);
void sendServerReply(int clientFd, std::string &message);
void addToClientSendBuf(Server *server, int clientFd, std::string message);
void sendClientRegistrationMsg(Server *server, int clientFd, Client *client);
std::string getChannelMemberList(std::string client, Channel &channel);
std::string getChannelName(std::string &msgToParse);
// std::string getSymbol(Channel &channel);
int getCommandType(std::string &cmd);

#endif