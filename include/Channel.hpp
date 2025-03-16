/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/16 14:46:45 by yohasega         ###   ########.fr       */
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
	bool	i;
	bool	t;
	bool	k;
	bool	l;
};

class Channel
{
  private:
	std::string						_name;
	std::string						_topic;
	s_mode							_mode;
	std::string						_password;
	int						        _capacity; // チャンネルに参加できる最大クライアント数
	std::map<const int, Client>		_clientList;
	std::vector<int>				_kickedUsers;
	std::vector<int>				_operatorList;

  public:
	Channel(const std::string &name);
	~Channel();

    // Setters
	// void setName(const std::string &name);
	// void setTopic(const std::string &topic);
	void setMode(const std::string &mode);
	void setPassword(const std::string &password);
	// void setCapacity(int capacity);
	// void setClientList(Client &client);
	// void setKickedUsers(int clientFd);
	void setOperatorList(int clientFd);

	// Getters
	// const std::string &getName() const;
	// const std::string &getTopic() const;
	const s_mode &getMode() const;
	const std::string &getPassword() const;
	int getCapacity() const;
	std::map<const int, Client> &getClientList();
	// std::vector<int> &getKickedUsers();
	std::vector<int> getOperatorList();
	
	bool isClientInChannel(const int clientFd);
	void addClientToChannel(Client &client);
};

#endif