/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:11 by hasega            #+#    #+#             */
/*   Updated: 2025/03/08 22:27:50 by yohasega         ###   ########.fr       */
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
    std::string		_readBuf; // 受信バッファ
    std::string		_sendBuf; // 送信バッファ
    bool		    	_toDeconnect; // 切断フラグ
    bool		    	_registrationDone; // 登録完了フラグ
    bool		    	_welcomeSent; // ウェルカムメッセージ送信済みフラグ

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
    void setReadBuf(const std::string &readBuf);
    // void setSendBuf(const std::string &sendBuf);
    // void setToDeconnect(bool flag);
    void setRegistrationDone(bool flag);
    // void setWelcomeSent(bool flag);
    void resetReadBuf();

    // Getters
    // int getClientFd() const;
    // const std::string &getNickname() const;
    // const std::string &getOldNickname() const;
    // const std::string &getUserName() const;
    // const std::string &getRealName() const;
    // const std::string &getMode() const;
    const std::string &getReadBuf() const;
    // const std::string &getSendBuf() const;
    // bool getToDeconnect() const;
    bool isRegistrationDone() const;
    // bool isWelcomeSent() const;
};

#endif