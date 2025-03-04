/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:11 by hasega            #+#    #+#             */
/*   Updated: 2025/03/04 19:08:12 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientFd) :
_clientFd(clientFd),
_nickname(""),
_oldNickname(""),
_userName(""),
_realName(""),
_mode(""),
_readBuffer(""),
_sendBuffer(""),
_toDeconnect(false),
_registrationDone(false),
_welcomeSent(false)
{
}

Client::~Client()
{
}


// void Client::setClientFd(int clientFd) { _clientFd = clientFd; }
// void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
// void Client::setOldNickname(const std::string &oldNickname) { _oldNickname = oldNickname; }
// void Client::setUserName(const std::string &userName) { _userName = userName; }
// void Client::setRealName(const std::string &realName) { _realName = realName; }
// void Client::setMode(const std::string &mode) { _mode = mode; }
// void Client::setReadBuffer(const std::string &readBuffer) { _readBuffer = readBuffer; }
// void Client::setSendBuffer(const std::string &sendBuffer) { _sendBuffer = sendBuffer; }
// void Client::setToDeconnect(bool flag) { _toDeconnect = flag; }
// void Client::setRegistrationDone(bool flag) { _registrationDone = flag; }
// void Client::setWelcomeSent(bool flag) { _welcomeSent = flag; }

// int Client::getClientFd() const { return (_clientFd); }
// const std::string &Client::getNickname() const { return (_nickname); }
// const std::string &Client::getOldNickname() const { return (_oldNickname); }
// const std::string &Client::getUserName() const { return (_userName); }
// const std::string &Client::getRealName() const { return (_realName); }
// const std::string &Client::getMode() const { return (_mode); }
// const std::string &Client::getReadBuffer() const { return (_readBuffer); }
// const std::string &Client::getSendBuffer() const { return (_sendBuffer); }
// bool Client::getToDeconnect() const { return (_toDeconnect); }
// bool Client::isRegistrationDone() const { return (_registrationDone); }
// bool Client::isWelcomeSent() const { return (_welcomeSent); }

