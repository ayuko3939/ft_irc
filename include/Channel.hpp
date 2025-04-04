/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/29 16:45:41 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"
# include "Client.hpp"

# include <iostream>
# include <string>
# include <map>

struct s_mode
{
	bool	invite;	// invite only
	bool	topic;	// topic settable by channel operator only
	bool	key;	// channel requires a key to join
	bool	limit;	// limit number of users
};

class Channel
{
  private:
	std::string						_name;
	std::string						_topic;
	s_mode							_mode;
	std::string						_password;
	size_t							_capacity; // チャンネルに参加できる最大クライアント数
	std::map<const int, Client>		_clientList;
	std::vector<int>				_invitedList;
	std::vector<int>				_operatorList;

  public:
	Channel(const std::string &name);
	~Channel();

    // Setters
	void setTopic(const std::string &topic);
	// void setMultMode(const std::string &mode); // 複数のモードを設定する場合
	void setMode(bool sign, const char mode); // 1つのモードを設定する場合
	void setPassword(const std::string &password);
	void setCapacity(int capacity);
	// Getters
	const std::string &getName() const;
	const std::string &getTopic() const;
	bool getMode(const std::string mode) const;
	const std::string &getPassword() const;
	size_t getCapacity() const;
	std::map<const int, Client> &getClientList();
	
	// check
	bool isClientInChannel(const int clientFd);
	bool isInvited(const int clientFd);
	bool isOperator(const int clientFd);
	// add
	void addClientToChannel(Client &client);
	void addInvitedList(int clientFd);
	void addOperatorList(int clientFd);
	// remove
	void removeClient(int clientFd);
	void removeInvited(int clientFd);
	void removeOperator(int clientFd);
};

#endif