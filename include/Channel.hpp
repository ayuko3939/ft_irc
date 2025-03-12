/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:04 by hasega            #+#    #+#             */
/*   Updated: 2025/03/12 16:09:32 by yohasega         ###   ########.fr       */
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
	std::map<std::string, Client>	_clientList;
	std::vector<std::string>		_kickedUsers;
	std::vector<std::string>		_operatorList;

  public:
	Channel(const std::string &name);
	~Channel();

    // Setters
	// void setName(const std::string &name);
	// void setTopic(const std::string &topic);
	// void setMode(const std::string &mode);
	// void setPassword(const std::string &password);
	// void setMaxConnections(int maxConnections);
	// void setClientList(const std::map<std::string, Client> &clientList);
	// void setKickedUsers(const std::vector<std::string> &kickedUsers);
	// void setOperatorList(const std::vector<std::string> &operatorList);

	// Getters
	// const std::string &getName() const;
	// const std::string &getTopic() const;
	const std::string &getMode() const;
	// const std::string &getPassword() const;
	int getMaxConnections() const;
	std::map<std::string, Client> &getClientList();
	// std::vector<std::string> &getKickedUsers();
	// std::vector<std::string> &getOperatorList();
	
	bool isClientInChannel(std::string &clientName);
	void addClientToChannel(Client &client);
};

#endif