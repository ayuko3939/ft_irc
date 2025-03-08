/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:58:58 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/08 18:05:32 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Irc.hpp"
# include "Server.hpp"

Client *getClient(Server *server, int clientFd)
{
	std::map<const int, Client>& clientList = server->getClientList();
	std::map<const int, Client>::iterator it = clientList.find(clientFd);

	// クライアントが見つからなかった場合、エラー文を出力してNULLを返す
	if (it == clientList.end())
		return (NULL);
	return (&it->second);
}
