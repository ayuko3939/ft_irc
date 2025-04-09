/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/04/09 18:04:36 by yohasega         ###   ########.fr       */
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
    void sendServerReply(int clientFd, std::string &message);
    
    void parseExecCommand(int clientFd, std::string &message);
    void execCommand(int clientFd, s_ircCommand	&cmdInfo);
    void fillClientInfo(Client *client, int clientFd, s_ircCommand cmdInfo);
    
  public:
	  Server(std::string port, std::string password, struct tm *timeinfo);
	  ~Server();

    // Getters
    std::string &getPassword();
    std::string &getDateTime();
    std::map<const int, Client>& getClientList();
    std::map<std::string, Channel>& getChannelList();
    int getClientFdByNick(std::string &nick);
    Client *getClient(int clientFd);

    // Other functions
    static void signalHandler(int signum);
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
void addToClientSendBuf(Server *server, int clientFd, std::string message);
Client &retrieveClient(Server *server, int clientSockFd);
std::string getChannelName(std::string &msgToParse);
std::string trim(const std::string &s);
int getCommandType(std::string &cmd);

#endif