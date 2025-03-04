/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 20:15:11 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Irc.hpp"

class Server
{
  private:
    int								_serverSockFd;
    std::map<const int, Client>		_clientList;
    std::map<std::string, Channel>	_channelList;
    std::string						_port;
    std::string						_Password;
    std::string						_dateTime;
    std::string						_motd;
    struct addrinfo					_addrInfo;
    struct addrinfo*				_serverInfo;

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
    // void setMotd(const std::string &motd);
    // void setAddrInfo(const struct addrinfo &addrInfo);
    // void setServerInfo(struct addrinfo* serverInfo);

    // Getters
    // int getServerSockFd() const;
    // const std::map<const int, Client>& getClientList() const;
    // const std::map<std::string, Channel>& getChannelList() const;
    // const std::string &getPort() const;
    // const std::string &getPassword() const;
    // const std::string &getDateTime() const;
    // const std::string &getMotd() const;
    // const struct addrinfo &getAddrInfo() const;
    // struct addrinfo* getServerInfo() const;

    // Other member function prototypes (implementation to be provided later)
    void launchServer();
    void manageServerLoop();
};

#endif