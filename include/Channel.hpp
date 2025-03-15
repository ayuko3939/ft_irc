/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/15 12:12:06 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Irc.hpp"
# include "Client.hpp"

# include <iostream>
# include <string>
# include <map>


class Channel
{
  private:
	std::string						_name;
	std::string						_topic;
	std::string						_mode;
	std::string						_password;
	int						        _maxConnections; // チャンネルに参加できる最大クライアント数
	std::map<const int, Client>	_clientList;
	std::vector<int>		_kickedUsers;
	std::vector<int>		_operatorList;

  public:
	Channel(const std::string &name);
	~Channel();

    // Setters
	// void setName(const std::string &name);
	// void setTopic(const std::string &topic);
	// void setMode(const std::string &mode);
	// void setPassword(const std::string &password);
	// void setMaxConnections(int maxConnections);
	// void setClientList(const std::map<const int, Client> &clientList);
	// void setKickedUsers(const std::vector<int> &kickedUsers);
	// void setOperatorList(const std::vector<int> &operatorList);

	// Getters
	// const std::string &getName() const;
	// const std::string &getTopic() const;
	const std::string &getMode() const;
	// const std::string &getPassword() const;
	int getMaxConnections() const;
	std::map<const int, Client> &getClientList();
	// std::vector<int> &getKickedUsers();
	// std::vector<int> &getOperatorList();
	
	bool isClientInChannel(const int clientFd);
	void addClientToChannel(Client &client);
};

#endif