/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/27 21:55:49 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// // PARTコマンドの引数数チェック (<channel> [<reason>])
// static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
// {
// 	// 少なくともチャンネル名が必要。理由は任意なので、1個または2個でなければエラー
// 	if (words.size() < 1 || words.size() > 2)
// 	{
// 		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(PART_USAGE));
// 		return false;
// 	}
// 	return true;
// }

// static std::string getChannelNameFromWords(std::string &word)
// {
// 	if (word[0] == '#')
// 		return (word.substr(1));
// 	return (word);
// }

// // PARTコマンドの処理
// // コマンド形式: PART <channel> [<reason>]
// void part(Server *server, int const clientFd, s_ircCommand cmdInfo)
// {
// 	// 1. 入力パラメータを空白で分割
// 	std::vector<std::string> words = splitMessage(cmdInfo.message);
// 	if (!checkArguments(server, clientFd, words))
// 		return;

// 	// 2. 発行者のクライアント情報を取得
// 	Client &client = retrieveClient(server, clientFd);
// 	std::string clientNick = client.getNickname();

// 	// 3. パラメータから対象のチャンネル名と（あれば）理由を取得
// 	std::string channelName = getChannelNameFromWords(words[0]);
// 	std::string reason = "";
// 	if (words.size() == 2)
// 		reason = words[1];

// 	// 4. チャンネルが存在するか確認
// 	if (!server->isChannelExist(channelName))
// 	{
// 		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(clientNick, channelName));
// 		return;
// 	}

// 	// 5. チャンネルオブジェクトの取得
// 	std::map<std::string, Channel> &channels = server->getChannelList();
// 	Channel &channel = channels.find(channelName)->second;

// 	// 6. クライアントがそのチャンネルに参加しているか確認
// 	if (!channel.isClientInChannel(clientFd))
// 	{
// 		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(clientNick, channelName));
// 		return;
// 	}

// 	// // 7. チャンネル内の唯一のオペレーターである場合、PARTを拒否
// 	// if (channel.isOperator(clientFd))
// 	// {
// 	// 	std::vector<int> opList = channel.getOperatorList();
// 	// 	if (opList.size() == 1)
// 	// 	{
// 	// 		std::string msg = ":" + clientNick + " :You are the only operator in " + channelName + ", you cannot part.\r\n";
// 	// 		addToClientSendBuf(server, clientFd, msg);
// 	// 		return;
// 	// 	}
// 	// }

// 	// 8. チャンネル内全メンバーに通知
// 	addToClientSendBuf(server, clientFd, RPL_PART(clientNick, client.getUserName(), channelName, reason));
// 	std::map<const int, Client> &clientList = channel.getClientList();
// 	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
// 	{
// 		addToClientSendBuf(server, it->second.getClientFd(), RPL_PART(clientNick, client.getUserName(), channelName, reason));
// 	}

// 	// 9. クライアントをチャンネルから削除する
// 	channel.removeClient(clientFd);
// }




// コマンドメッセージから退出理由を抽出して返す。
std::string getReason(std::string msg_to_parse)
{
	// 1. 空の理由文字列を初期化する。
	std::string reason;

	reason.clear(); // by default let's say there is no reason = "#hey"
	
	// 2. メッセージ内に「:」が存在すれば、その位置以降を退出理由として抽出する。
	if (msg_to_parse.find(":") != std::string::npos) // If there is a ":", there is a reason => "#hey :bonjour"
		reason.append(msg_to_parse, msg_to_parse.find(":") + 1, std::string::npos);
	// 3. 抽出した退出理由を返す。
	return (reason); // expected outcome : "bonjour"
}

// 文字列に少なくとも1文字の英字が含まれているか判定する。
static bool containsAtLeastOneAlphaChar(std::string str)
{
	// 1. 文字列内の各文字について、英字であればtrueを返す。
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			return (true);
	}
	// 2. 英字が見つからなければfalseを返す。
	return (false);
}

// 指定されたチャンネル内の全メンバーに対して、PART通知メッセージをブロードキャストする。
static void broadcastToAllChannelMembers(Server *server, Channel &channel, std::string user, std::string nick, std::string reason)
{
	// 1. チャンネルのクライアントリストを取得し、各メンバーに対してループ処理を行う。
	std::map<int, Client>::iterator member = channel.getClientList().begin();

	while (member != channel.getClientList().end())
	{
		// 2. 各メンバーに、PART通知メッセージ（送信者のユーザ情報、チャンネル名、退出理由）を送信する。
		addToClientSendBuf(server, member->second.getClientFd(), RPL_PART(nick, user, channel.getName(), reason));
		member++;
	}
}


void part(Server *server, int const clientFd, s_ircCommand cmd_infos)
{
	// 1. 送信元クライアント情報を取得し、ニックネームを得る。
	Client client = retrieveClient(server, clientFd);
	std::string nick = client.getNickname();
	std::string channel;
	
	// 2. コマンドメッセージから退出理由を抽出する。
	std::string reason = getReason(cmd_infos.message);
	// 3. もしメッセージが空なら、パラメータ不足のエラーを返す。
	if (cmd_infos.message.empty() == true)
	{
		addToClientSendBuf(server, clientFd, ERR_NEEDMOREPARAMS(nick, cmd_infos.name));
		return;
	}
	// 4. メッセージから退出理由部分を削除して、残りの部分をチャンネル名抽出に使用する。
	cmd_infos.message.erase(cmd_infos.message.find(reason), reason.length()); // "#chan reason" becomes "#chan "

	// 5. メッセージに有効な文字（英字）が含まれている間、各チャンネルについて処理を行う。
	while (containsAtLeastOneAlphaChar(cmd_infos.message) == true)
	{
		// 5-1. メッセージからチャンネル名を抽出する。
		channel.clear();
		channel = getChannelName(cmd_infos.message);
		// 5-2. 抽出したチャンネル名部分をメッセージから削除する。
		cmd_infos.message.erase(cmd_infos.message.find(channel), channel.length()); 

		// 5-3. サーバー内のチャンネル一覧から、抽出したチャンネルが存在するか検索する。
		std::map<std::string, Channel>& channels = server->getChannelList();
		std::map<std::string, Channel>::iterator it = channels.find(channel);
		// 5-4. チャンネルが存在しなければ、ERR_NOSUCHCHANNELエラーを返す。
		if (it == channels.end())
		{
			addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(nick, channel));
			continue;
		}
		// 5-5. チャンネルは存在するが、クライアントがそのチャンネルに参加していなければ、ERR_NOTONCHANNELエラーを返す。
		else if (it != channels.end() && it->second.isClientInChannel(clientFd) == false)
		{
			addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(nick, channel));
			continue;
		}
		// 5-6. すべて問題なければ、クライアントをチャンネルから削除し、退出通知を送信する。
		else
		{
			it->second.getClientList().erase(clientFd);
			addToClientSendBuf(server, clientFd, RPL_PART(nick, client.getUserName(), channel, reason));
			// 5-7. チャンネル内の他のメンバーにも退出通知をブロードキャストする。
			broadcastToAllChannelMembers(server, it->second, client.getUserName(), nick, reason);
		}
	}
}
