/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:58:58 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/10 19:45:53 by yohasega         ###   ########.fr       */
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

void splitMessage(std::string &message, std::vector<std::string> &cmds)
{
	// 改行コードを"\r\n"から"\n"に変換
	std::string			replaced;
	std::string::size_type pos = 0;

	while ((pos = message.find("\r\n", pos)) != std::string::npos)
	{
		message.replace(pos, 2, "\n");
		pos += 1;
	}

	// メッセージを１行ずつ分割してコマンドリストに格納
	std::istringstream	iss(message);
	std::string			line;

	while (std::getline(iss, line, '\n'))
		cmds.push_back(line);
}

void sendServerReply(int clientFd, std::string &message)
{
	std::istringstream iss(message);
	std::string line;

	// クライアントにデータを送信
	send(clientFd, message.c_str(), message.size(), 0);
	
	// 送信したデータを表示
	while (getline(iss, line))
	{
		  // ===== ★後で表示を整える★ =====
		std::cout << "[Server] "
				  << clientFd << " >" << line << std::endl;
	}
}
