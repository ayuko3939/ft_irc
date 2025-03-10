/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:11 by hasega            #+#    #+#             */
/*   Updated: 2025/03/10 21:25:21 by yohasega         ###   ########.fr       */
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
_readBuf(""),
_sendBuf(""),
_connexion_password(false),
_nmInfo(0),
_hasAllInfo(false),
_toDeconnect(false),
_registrationDone(false),
_welcomeSent(false)
{
}

Client::~Client()
{
}


// void Client::setClientFd(int clientFd) { _clientFd = clientFd; }
void Client::setUserName(const std::string &userName) { _userName = userName; }
// void Client::setRealName(const std::string &realName) { _realName = realName; }
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
// void Client::setOldNickname(const std::string &oldNickname) { _oldNickname = oldNickname; }
// void Client::setMode(const std::string &mode) { _mode = mode; }
void Client::setReadBuf(const std::string &readBuf) { _readBuf = readBuf; }
// void Client::setSendBuf(const std::string &sendBuf) { _sendBuf = sendBuf; }
void Client::setConnexionPassword() { _connexion_password = true; }
// void Client::incrementNmInfo() { _nmInfo++; }
void Client::setHasAllInfo() { _hasAllInfo = true; }
// void Client::setToDeconnect(bool flag) { _toDeconnect = flag; }
void Client::setRegistrationDone() { _registrationDone = true; }
// void Client::setWelcomeSent(bool flag) { _welcomeSent = flag; }
void Client::resetReadBuf() { _readBuf.clear(); }

int Client::getClientFd() { return (_clientFd); }
// std::string &Client::getUserName() { return (_userName); }
// std::string &Client::getRealName() { return (_realName); }
std::string &Client::getNickname() { return (_nickname); }
// std::string &Client::getOldNickname() { return (_oldNickname); }
// std::string &Client::getMode() { return (_mode); }
std::string &Client::getReadBuf() { return (_readBuf); }
std::string &Client::getSendBuf() { return (_sendBuf); }
// bool Client::getConnexionPassword() { return (_connexion_password); }
int Client::getNmInfo() { return (_nmInfo); }
bool Client::getHasAllInfo() { return (_hasAllInfo); }
bool Client::getToDeconnect() { return (_toDeconnect); }
bool Client::isRegistrationDone() { return (_registrationDone); }
// bool Client::isWelcomeSent() { return (_welcomeSent); }

bool Client::isValid()
{
	return (_clientFd != -1 &&
			_nickname != "" && _userName != "" && _realName != "" &&
			_connexion_password == true &&
			_nmInfo >= 3);
}
