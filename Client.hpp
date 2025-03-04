/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:11 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 19:15:46 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

class Client
{
  private:
    int           _clientFd;
    std::string		_nickname;
    std::string		_oldNickname;
    std::string		_userName;
    std::string		_realName;
    std::string		_mode;
    std::string		_readBuffer;
    std::string		_sendBuffer;
    bool		    	_toDeconnect;
    bool		    	_registrationDone;
    bool		    	_welcomeSent;

  public:
    Client(int clientFd);
    ~Client();

    // Setters
    // void setClientFd(int clientFd);
    // void setNickname(const std::string &nickname);
    // void setOldNickname(const std::string &oldNickname);
    // void setUserName(const std::string &userName);
    // void setRealName(const std::string &realName);
    // void setMode(const std::string &mode);
    // void setReadBuffer(const std::string &readBuffer);
    // void setSendBuffer(const std::string &sendBuffer);
    // void setToDeconnect(bool flag);
    // void setRegistrationDone(bool flag);
    // void setWelcomeSent(bool flag);

    // Getters
    // int getClientFd() const;
    // const std::string &getNickname() const;
    // const std::string &getOldNickname() const;
    // const std::string &getUserName() const;
    // const std::string &getRealName() const;
    // const std::string &getMode() const;
    // const std::string &getReadBuffer() const;
    // const std::string &getSendBuffer() const;
    // bool getToDeconnect() const;
    // bool isRegistrationDone() const;
    // bool isWelcomeSent() const;
};

#endif