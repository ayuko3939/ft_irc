/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 12:58:58 by yohasega          #+#    #+#             */
/*   Updated: 2025/04/09 17:56:01 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Irc.hpp"
# include "Server.hpp"

void addToClientSendBuf(Server *server, int clientFd, std::string message)
{
	Client &client = retrieveClient(server, clientFd);
	std::string newMessage = client.getSendBuf() + message;
	client.setSendBuf(newMessage);
}

Client &retrieveClient(Server *server, int clientFd)
{
	// クライアントリストからクライアント情報を探す
	std::map<const int, Client>& clientList = server->getClientList();
	std::map<const int, Client>::iterator it = clientList.find(clientFd);

	Client &client = it->second;
	return (client);
}

std::string getChannelName(std::string &msgToParse)
{
	std::string channelName;
	size_t i = 0;
	
	// チャンネル名の先頭を探す（アルファベット、数字、ハイフン、アンダースコア以外の文字は無視）
	while (msgToParse[i] && (!isalpha(msgToParse[i])) && (!isdigit(msgToParse[i])) &&
		  (msgToParse[i] != '-') && (msgToParse[i] != '_'))
		i++;

	// チャンネル名を取得（アルファベット、数字、ハイフン、アンダースコア以外の文字が出るまで）
	while (msgToParse[i] && (isalpha(msgToParse[i]) || isdigit(msgToParse[i]) ||
		(msgToParse[i] == '-') || (msgToParse[i] == '_')))
		channelName += msgToParse[i++];
	
	return (channelName);
}

// 文字列の先頭と末尾の空白（スペース、タブ）を除去する
std::string trim(const std::string &s)
{
	std::string::size_type start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";
	std::string::size_type end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

int getCommandType(std::string &cmd)
{
	int	type = 0;
	std::string	cmdList[NUM_OF_CMD] = {
		"CAP", "INVITE", "JOIN", "KICK", "MODE", "NICK",
		"PART", "PASS", "PING", "PRIVMSG", "QUIT", "TOPIC", "USER"
	};

	// コマンドリストからコマンドを検索
	while (type < NUM_OF_CMD)
	{
		if (cmd == cmdList[type])
			break;
		type++;
	}
	return (type + 1);
}
