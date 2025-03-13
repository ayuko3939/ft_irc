/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/13 17:34:02 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Irc.hpp"
# include "Server.hpp"

void nick(Server *server, int clientFd,s_ircCommand cmdInfo);
void user(Server *server, int clientFd,s_ircCommand cmdInfo);
void pass(Server *server, int clientFd,s_ircCommand cmdInfo);

#endif