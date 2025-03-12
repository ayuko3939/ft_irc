/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/12 17:44:50 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

int pass(Server *server, int clientFd,s_ircCommand cmdInfo)
{
	(void)server;
	(void)clientFd;
	(void)cmdInfo;

	std::cout << "=== PASS command test! ===" << std::endl;
	return (EXIT_SUCCESS);
}
