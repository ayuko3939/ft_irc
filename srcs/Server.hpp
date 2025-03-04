/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 23:01:07 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Server
{
  private:
    int							              	_serverSockFd;
    std::map<const int, Client>		  _clientList;
    std::map<std::string, Channel>	_channelList;
    std::string						          _port;
    std::string					          	_password;
    std::string					          	_dateTime;
    struct addrinfo				        	_addrInfo; // アドレス情報
    struct addrinfo*			        	_serverInfo; // サーバー情報

  public:
	  Server(std::string port, std::string password, struct tm *timeinfo);
	  ~Server();

    // Setters
    // void setServerSockFd(int serverSockFd);
    // void setClientList(const std::map<const int, Client> &clientList);
    // void setChannelList(const std::map<std::string, Channel> &channelList);
    // void setPort(const std::string &port);
    // void setPassword(const std::string &password);
    // void setDateTime(const std::string &dateTime);
    // void setAddrInfo(const struct addrinfo &addrInfo);
    // void setServerInfo(struct addrinfo* serverInfo);

    // Getters
    // int getServerSockFd() const;
    // const std::map<const int, Client>& getClientList() const;
    // const std::map<std::string, Channel>& getChannelList() const;
    // const std::string &getPort() const;
    // const std::string &getPassword() const;
    // const std::string &getDateTime() const;
    // const struct addrinfo &getAddrInfo() const;
    // struct addrinfo* getServerInfo() const;

    // Other member function prototypes (implementation to be provided later)
    void initServer();
    // void manageServerLoop();
};

#endif