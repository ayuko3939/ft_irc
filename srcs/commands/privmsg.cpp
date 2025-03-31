/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/29 19:20:51 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void getArgument(std::string &argument, std::string &target, std::string &message)
{
	size_t pos = argument.find(' ');
	if (pos == std::string::npos)
	{
		target = "";
		message = "";
	}
	else
	{
		target = argument.substr(0, pos);
		message = argument.substr(pos + 1);
		if (message[0] == ':')
			message = message.substr(1);
	}
}

static bool checkArguments(Server *server, int clientFd, std::string &target, std::string &message)
{
	// <target> <message>
	if (target.empty() || message.empty())
	{
		addToClientSendBuf(server, clientFd, ERR_NORECIPIENT(target, "PRIVMSG"));
		return (false);
	}
	if (message.size() > 400)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(PRIVMSG_REQUIREMENTS));
		return (false);
	}
	return (true);
}

static void broadcastMSG(Server *server, Channel &channel, Client &client, std::string &target, std::string &message)
{
	// チャンネルメンバー全員に
	std::map<const int, Client> &clientList = channel.getClientList();

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		if (it->second.getNickname() != client.getNickname())
			addToClientSendBuf(server, it->second.getClientFd(), RPL_PRIVMSG(IRC_PREFIX(client.getNickname(), client.getUserName()), '#' + target, message));
	}
}

void privmsg(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	std::string target;
	std::string message;
	bool		isChannel = false;

	// 1. ターゲットとメッセージを取得（最初のスペースで区切り、以降はまとめて１つのテキストとする）
	getArgument(cmdInfo.message, target, message);

	// 2. 入力パラメータをチェック
	if (!checkArguments(server, clientFd, target, message))
		return;
	
	// 3. ターゲットがチャンネルならば、#を削除してフラグを立てる
	if (target[0] == '#')
	{
		isChannel = true;
		target = target.substr(1);
	}

	// 4. 送信元クライアント情報の取得
	Client &client = retrieveClient(server, clientFd);
	std::string senderNick = client.getNickname();

	// 5-1. ターゲットがチャンネルの場合
	if (isChannel)
	{
		// 6. ターゲットが存在するか確認
		if (!server->isChannelExist(target))
		{
			addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(senderNick, target));
			return;
		}

		std::map<std::string, Channel> &channels = server->getChannelList();
		Channel &channel = channels.find(target)->second;

		// 7. 送信者がそのチャンネルに参加しているか確認
		if (!channel.isClientInChannel(clientFd))
		{
			addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(senderNick, target));
			return;
		}

		// 8. チャンネル内の全メンバー（送信元以外）に対して、メッセージをブロードキャスト
		broadcastMSG(server, channel, client, target, message);
	}
	// 5-2. ターゲットがユーザーの場合
	else
	{
		// 6. ターゲットが存在するか確認
		if (!server->isClientExist(target))
		{
			addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(senderNick, target));
			return;
		}
		
		// 7. ターゲットにメッセージを送信
		int targetFd = server->getClientFdFromNick(target);
		addToClientSendBuf(server, targetFd, RPL_PRIVMSG(IRC_PREFIX(senderNick, client.getUserName()), target, message));
	}
}

// PRIVMSG <target> <text to be sent>
// void privmsg(Server *server, int const clientFd, s_ircCommand cmdInfo)
// {
//     // 1. 送信元クライアントの情報を取得
//     Client &client = retrieveClient(server, clientFd);
//     std::string senderNick = client.getNickname();

//     // 2. 入力パラメータを先頭のスペースで区切り、最初のトークンと残りの部分に分割する
//     std::istringstream iss(cmdInfo.message);
//     std::string target;
//     if (!(iss >> target)) {
//         // ターゲットが指定されていない場合
//         addToClientSendBuf(server, clientFd, ERR_NORECIPIENT(senderNick, "PRIVMSG"));
//         return;
//     }
//     // 残りの部分を1行として取得
//     std::string messageText;
//     std::getline(iss, messageText);
//     messageText = trim(messageText);
    
//     // 3. 送信テキストが空の場合はエラー
//     if (messageText.empty()) {
//         addToClientSendBuf(server, clientFd, ERR_NOTEXTTOSEND(senderNick));
//         return;
//     }
    
//     // 4. ターゲットがチャネルかユーザーかを判定する
//     if (!target.empty() && target[0] == '#')
//     {
//         // ターゲットがチャネルの場合
//         if (!server->isChannelExist(target))
//         {
//             addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(senderNick, target));
//             return;
//         }
        
//         // サーバーのチャネルリストから対象チャネルを取得
//         std::map<std::string, Channel> &channels = server->getChannelList();
//         Channel &channel = channels.find(target)->second;
        
//         // 送信元がチャネルに参加しているか確認
//         if (!channel.isClientInChannel(clientFd))
//         {
//             addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(senderNick, target));
//             return;
//         }
        
//         // 5. チャネル内の全メンバー（送信元以外）にメッセージをブロードキャスト
//         std::map<const int, Client> &clientList = channel.getClientList();
//         for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
//         {
//             if (it->first != clientFd)
//             {
//                 // RPL_PRIVMSG は「:sender PRIVMSG target :message\r\n」の形式を生成するマクロと想定
//                 std::string notify = RPL_PRIVMSG(senderNick, target, messageText);
//                 addToClientSendBuf(server, it->second.getClientFd(), notify);
//             }
//         }
//     }
//     else
//     {
//         // ターゲットがユーザーの場合
//         if (!server->isClientExist(target))
//         {
//             addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(senderNick, target));
//             return;
//         }
//         int targetFd = server->getClientFdFromNick(target);
//         std::string notify = RPL_PRIVMSG(senderNick, target, messageText);
//         addToClientSendBuf(server, targetFd, notify);
//     }
// }

