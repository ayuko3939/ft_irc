/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/29 16:48:01 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name):
_name(name),
_topic(""),
_password(""),
_capacity(MAX_CLIENTS),
_clientList(),
_kickedUsers(),
_operatorList()
{
	_mode.invite = false;
	_mode.topic = true;
	_mode.key = false;
	_mode.limit = true;
}

Channel::~Channel() {}


// void Channel::setName(const std::string &name) { _name = name; }
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setPassword(const std::string &password) { _password = password; }
void Channel::setCapacity(int capacity) { _capacity = capacity; }
// void Channel::setClientList(const Client &client) { _clientList.insert(std::make_pair(client.getClientFd(), client)); }
// void Channel::setKickedUsers(int clientFd) { _kickedUsers.push_back(clientFd); }
void Channel::setOperatorList(int clientFd) { _operatorList.push_back(clientFd); }

// void Channel::setMultMode(const std::string &mode)
// {
// 	std::string::const_iterator it = mode.begin();
// 	bool flag = true;

// 	// チャンネルモードの先頭文字が+か-かでフラグを立てる
// 	if (*it != '+' && *it != '-')
// 	{
// 		std::cerr << "invalid option" << std::endl;
// 		return ;
// 	}
	
// 	// チャンネルモードの文字列を解析してフラグを立てる
// 	for (; it != mode.end(); ++it)
// 	{
// 		switch (*it)
// 		{
// 			case '-':
// 				flag = false;
// 				break;
// 			case '+':
// 				flag = true;
// 				break;

// 			case 'i':
// 				_mode.invite = flag;
// 				break;
// 			case 't':
// 				_mode.topic = flag;
// 				break;
// 			case 'k':
// 				_mode.key = flag;
// 				break;
// 			case 'o':
// 				break;
// 			case 'l':
// 				_mode.limit = flag;
// 				break;
// 			default:
// 				std::cerr << "invalid option" << std::endl;
// 				return ;
// 		}
// 	}
// }

void Channel::setMode(bool sign, const char mode)
{
	std::cout << "[setMode] mode: " << mode << ", sign: " << sign << std::endl;
	switch (mode)
	{
		case 'i':
			_mode.invite = sign;
			break;
		case 't':
			_mode.topic = sign;
			break;
		case 'k':
			_mode.key = sign;
			break;
		case 'l':
			_mode.limit = sign;
			break;
		default:
			std::cerr << "invalid option" << std::endl;
			return ;
	}
}

const std::string &Channel::getName() const { return (_name);}
const std::string &Channel::getTopic() const { return (_topic);}
const std::string &Channel::getPassword() const { return (_password);}
size_t Channel::getCapacity() const { return (_capacity);}
std::map<const int, Client> &Channel::getClientList() { return (_clientList);}
// int Channel::getKickedUsers() { return (_kickedUsers);}
std::vector<int> Channel::getOperatorList() { return (_operatorList);}

bool Channel::getMode(const std::string mode) const
{
	if (mode == "i" || mode == "invite")
		return (_mode.invite);
	else if (mode == "t" || mode == "topic")
		return (_mode.topic);
	else if (mode == "k" || mode == "key")
		return (_mode.key);
	else if (mode == "l" || mode == "limit")
		return (_mode.limit);
	else
		return (false);
}

bool Channel::isClientInChannel(const int clientFd)
{
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return (false);
	return (true);
}

bool Channel::isOperator(int clientFd)
{
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it)
	{
		if (*it == clientFd)
			return (true);
	}
	return (false);
}

void Channel::addClientToChannel(Client &client)
{
	_clientList.insert(std::make_pair(client.getClientFd(), client));
}

void Channel::removeClient(int clientFd)
{
	_clientList.erase(clientFd);

	if (isOperator(clientFd))
		removeOperator(clientFd);
}

void Channel::removeOperator(int clientFd)
{
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it)
	{
		if (*it == clientFd)
		{
			_operatorList.erase(it);
			return ;
		}
	}
}
