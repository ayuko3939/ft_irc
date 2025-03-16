/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 19:19:53 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Irc.hpp"
# include "Server.hpp"

// Command functions
void nick(Server *server, const int clientFd, s_ircCommand cmdInfo);
void join(Server *server, const int clientFd, s_ircCommand cmdInfo);
void pass(Server *server, const int clientFd, s_ircCommand cmdInfo);
// void privmsg(Server *server, const int clientFd, s_ircCommand cmdInfo);
void topic(Server *server, const int clientFd, s_ircCommand cmdInfo);
void user(Server *server, const int clientFd, s_ircCommand cmdInfo);


// Command utils
std::vector<std::string> splitMessage(std::string message);
std::vector<std::string> splitByComma(const std::string &str);

#endif