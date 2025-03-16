/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/16 14:47:16 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name):
_name(name),
_topic(""),
_password(""),
_capacity(-1),
_clientList(),
_kickedUsers(),
_operatorList()
{
	setMode("+t");
}

Channel::~Channel() {}


// void Channel::setName(const std::string &name) { _name = name; }
// void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setPassword(const std::string &password) { _password = password; }
// void Channel::setCapacity(int capacity) { _capacity = capacity; }
// void Channel::setClientList(const Client &client) { _clientList.insert(std::make_pair(client.getClientFd(), client)); }
// void Channel::setKickedUsers(int clientFd) { _kickedUsers.push_back(clientFd); }
void Channel::setOperatorList(int clientFd) { _operatorList.push_back(clientFd); }

void Channel::setMode(const std::string &mode)
{
	std::string::const_iterator it = mode.begin();
	bool flag;

	// チャンネルモードの先頭文字が+か-かでフラグを立てる
	switch (*it)
	{
		case '-':
			flag = false;
			break;
		case '+':
			flag = true;
			break;
		default:
			std::cerr << "invalid option" << std::endl;
			return ;
	}
	++it;
	
	// チャンネルモードの文字列を解析してフラグを立てる
	for (; it != mode.end(); ++it)
	{
		switch (*it)
		{
			case 'i':
				_mode.i = flag;
				break;
			case 't':
				_mode.t = flag;
				break;
			case 'k':
				_mode.k = flag;
				break;
			case 'o':
				break;
			case 'l':
				_mode.l = flag;
				break;
			default:
				std::cerr << "invalid option" << std::endl;
				return ;
		}
	}
}

// const std::string &Channel::getName() const { return (_name);}
// const std::string &Channel::getTopic() const { return (_topic);}
const s_mode &Channel::getMode() const { return (_mode);}
const std::string &Channel::getPassword() const { return (_password);}
int Channel::getCapacity() const { return (_capacity);}
std::map<const int, Client> &Channel::getClientList() { return (_clientList);}
// int Channel::getKickedUsers() { return (_kickedUsers);}
std::vector<int> Channel::getOperatorList() { return (_operatorList);}


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
