/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/15 12:11:19 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name):
_name(name),
_topic(""),
_mode(""),
_password(""),
_maxConnections(0),
_clientList(),
_kickedUsers(),
_operatorList()
{
}

Channel::~Channel() {}


// void Channel::setName(const std::string &name) { _name = name; }
// void Channel::setTopic(const std::string &topic) { _topic = topic; }
// void Channel::setMode(const std::string &mode) { _mode = mode; }
// void Channel::setPassword(const std::string &password) { _password = password; }
// void Channel::setMaxConnections(int maxConnections) { _maxConnections = maxConnections; }
// void Channel::setClientList(const std::map<std::string, Client> &clientList) { _clientList = clientList; }
// void Channel::setKickedUsers(const std::vector<std::string> &kickedUsers) { _kickedUsers = kickedUsers; }
// void Channel::setOperatorList(const std::vector<std::string> &operatorList) { _operatorList = operatorList; }

// const std::string &Channel::getName() const { return (_name);}
// const std::string &Channel::getTopic() const { return (_topic);}
const std::string &Channel::getMode() const { return (_mode);}
// const std::string &Channel::getPassword() const { return (_password);}
int Channel::getMaxConnections() const { return (_maxConnections);}
std::map<const int, Client> &Channel::getClientList() { return (_clientList);}
// std::vector<std::string> &Channel::getKickedUsers() { return (_kickedUsers);}
// std::vector<std::string> &Channel::getOperatorList() { return (_operatorList);}


bool Channel::isClientInChannel(const int clientFd)
{
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return (false);
	return (true);
}

void Channel::addClientToChannel(Client &client)
{
	_clientList.insert(std::make_pair(client.getClientFd(), client));
}
