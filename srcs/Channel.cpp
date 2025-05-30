/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/04/06 19:28:15 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name):
_name(name),
_topic(""),
_password(""),
_capacity(0),
_clientList(),
_invitedList(),
_operatorList()
{
	_mode.invite = false;
	_mode.topic = true;
	_mode.key = false;
	_mode.limit = false;
}

Channel::~Channel() {}

void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setPassword(const std::string &password) { _password = password; }
void Channel::setCapacity(int capacity) { _capacity = capacity; }

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
	// std::cout << GREEN "[setMode] mode: " << mode << ", sign: " << sign << END << std::endl;
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

bool Channel::isInvited(const int clientFd)
{
	for (std::vector<int>::iterator it = _invitedList.begin(); it != _invitedList.end(); ++it)
	{
		if (*it == clientFd)
			return (true);
	}
	return (false);
}

bool Channel::isOperator(const int clientFd)
{
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it)
	{
		if (*it == clientFd)
			return (true);
	}
	return (false);
}

void Channel::addClientToChannel(Client &client) { _clientList.insert(std::make_pair(client.getClientFd(), client)); }
void Channel::addOperatorList(int clientFd) { _operatorList.push_back(clientFd); }
void Channel::addInvitedList(int clientFd){ _invitedList.push_back(clientFd); }

void Channel::removeClient(int clientFd)
{
	_clientList.erase(clientFd);
	// std::cout << "debug : remove client " << clientFd << " from #" << getName() << std::endl;
	
	if (isOperator(clientFd))
		removeOperator(clientFd);
	if (isInvited(clientFd))
		removeInvited(clientFd);
}

void Channel::removeInvited(int clientFd)
{
	for (std::vector<int>::iterator it = _invitedList.begin(); it != _invitedList.end(); ++it)
	{
		if (*it == clientFd)
		{
			_invitedList.erase(it);
			// std::cout << "debug : remove invited too" << std::endl;
			return ;
		}
	}
	// std::cout << "debug : remove invited not found" << std::endl;
}

void Channel::removeOperator(int clientFd)
{
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it)
	{
		if (*it == clientFd)
		{
			_operatorList.erase(it);
			// std::cout << "debug : remove operator too" << std::endl;
			return ;
		}
	}
	// std::cout << "debug : remove operator not found" << std::endl;
}

std::string Channel::getClientListString()
{
	std::string clientListString = "";
	std::map<const int, Client>::iterator it = _clientList.begin();
	
	while (it != _clientList.end())
	{
		// チャンネルオペレーターの場合、@をつける
		if (isOperator(it->first))
			clientListString += "@";
		clientListString += it->second.getNickname();
		++it;
		if (it != _clientList.end())
			clientListString += " ";
	}
	return (clientListString);
}
