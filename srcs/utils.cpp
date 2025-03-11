/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:58:58 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/11 22:27:41 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Irc.hpp"
# include "Server.hpp"

// 文字列の先頭と末尾の空白（スペース、タブ）を除去する
std::string trim(const std::string &s)
{
    std::string::size_type start = s.find_first_not_of(" \t");
    if (start == std::string::npos)
        return "";
    std::string::size_type end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

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


Client &getClient(Server *server, int clientFd)
{
	// クライアントリストからクライアント情報を探す
	std::map<const int, Client>& clientList = server->getClientList();
	std::map<const int, Client>::iterator it = clientList.find(clientFd);

	Client &client = it->second;
	return (client);
}

void addToClientSendBuf(Server *server, int clientFd, std::string message)
{
	Client &client = getClient(server, clientFd);
	client.setSendBuf(client.getSendBuf() + message);
}

void sendClientRegistrationMsg(Server *server, int clientFd,std::map<const int, Client>::iterator &it)
{
	addToClientSendBuf(server, clientFd, RPL_WELCOME(it->second.getNickname(), it->second.getNickname()));
	addToClientSendBuf(server, clientFd, RPL_YOURHOST(it->second.getNickname(), "ft_irc", "1.0"));
	addToClientSendBuf(server, clientFd, RPL_CREATED(it->second.getNickname(), server->getDateTime()));
	// addToClientSendBuf(server, clientFd, RPL_MYINFO(it->second.getNickname(), "ft_irc", "1.0"));
	addToClientSendBuf(server, clientFd, RPL_ISUPPORT(it->second.getNickname(), "CHANNELLEN=32 NICKLEN=9 TOPICLEN=307"));
}

std::string getChannelName(std::string &msgToParse)
{
	std::string channelName;
	size_t i = 0;
	
	while (msgToParse[i] && (!isalpha(msgToParse[i])) && (!isdigit(msgToParse[i])) &&
		  (msgToParse[i] != '-') && (msgToParse[i] != '_'))
		i++;

		//  ★★★★
	
}

// チャンネルのシンボルを取得(@: secret, *: private, =: public)
std::string getSymbol(Channel &channel)
{
	std::string symbol;

	if (channel.getMode().find('s') != std::string::npos)
		symbol = "@";
	else if (channel.getMode().find('p') != std::string::npos)
		symbol = "*";
	else
		symbol = "=";
	return (symbol);
}
