/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/28 18:50:21 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// コマンド形式: MODE <channel> [<modestring> [<mode arguments>...]]
static bool checkArguments(Server *server, int const clientFd, std::vector<std::string> &words)
{
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();
	int wordsSize = words.size();

	// 引数がない場合はエラー
	if (wordsSize == 0 || words[0].empty())
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(MODE_USAGE));
		return (false);
	}

	// チャンネル名のみならモード説明なのでOK
	if (wordsSize == 1)
		return (true);

	// モード文字列が正しいか確認
	std::string modeString = words[1];
	if (modeString.empty() || modeString.size() != 2 ||
		(modeString[0] != '+' && modeString[0] != '-') ||
		(modeString[1] != 'i' && modeString[1] != 't' && modeString[1] != 'k' && modeString[1] != 'o' && modeString[1] != 'l'))
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(MODE_REQUIREMENTS));
		return (false);
	}
	
	// <mode arguments> が必要なモード文字列の場合、引数があるか確認
	// +k <password>  ,  +l <limit>  ,  +o <nickname>  は引数が必要
	if (modeString == "+k" || modeString == "+o" || modeString == "-o" || modeString == "+l")
	{
		if (wordsSize != 3 || words[2].empty())
		{
			addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(MODE_USAGE_K_O_L));
			return (false);
		}
	}
	// それ以外のモード文字列は引数不要
	else
	{
		if (wordsSize != 2)
		{
			addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(MODE_USAGE));
			return (false);
		}
	}
	return (true);
}

static void infoChannelMode(Server *server, int const clientFd, Channel &channel, std::string channelName)
{
	std::string modeInfo = "========== " + channelName + "'s mode ==========\r\n";
	modeInfo += " invite only   : ";
	modeInfo += (channel.getMode("i") ? "on\r\n" : "off\r\n");
	modeInfo += " topic protect : ";
	modeInfo += (channel.getMode("t") ? "on\r\n" : "off\r\n");
	modeInfo += " channel key   : ";
	modeInfo += (channel.getMode("k") ? "on\r\n" : "off\r\n");
	modeInfo += " user limit    : ";
	modeInfo += (channel.getMode("l") ? "on\r\n" : "off\r\n");
	modeInfo += "\r\n";
	if (channel.isOperator(clientFd))
		modeInfo += "You are operator. You can change channel mode.\r\n";
	else
		modeInfo += "You are not operator. if you want to change channel mode, you need to ask operator.\r\n";

	addToClientSendBuf(server, clientFd, modeInfo);
}

static void processModeChanges(Server *server, Client &client, Channel &channel, std::string modeString, std::string modeArgs)
{
	// モード変更の内容を取得
	bool sign = modeString[0] == '+'; // true: +, false: -
	char mode = modeString[1];

	// モード変更の処理
	switch (mode)
	{
		case 'i':
			inviteOnlyMode(server, channel, client, sign);
			break;
		case 't':
			topicProtectMode(server, channel, client, sign);
			break;
		case 'k':
			channelKeyMode(server, channel, client, sign, modeArgs);
			break;
		case 'l':
			userLimitMode(server, channel, client, sign, modeArgs);
			break;
		case 'o':
			operatorMode(server, channel, client, sign, modeArgs);
			break;
	
		default:
			// 未対応のモード文字列の場合はエラー
			addToClientSendBuf(server, client.getClientFd(), ERR_UNKNOWNMODE(client.getNickname(), modeString));
			break;
	}
}

// コマンド形式: MODE <channel> <modestring> [<mode arguments>...]
void mode(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 2. 引数の数と形式が正しいか確認
	if (!checkArguments(server, clientFd, words))
		return;

	// 3. クライアント情報を取得
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();

	// 4. チャンネル名（ニックネーム）を取得
	bool		isChannel = false;
	std::string target = words[0];
	if (target[0] == '#')
	{
		isChannel = true;
		target = target.substr(1);
	}

	// 5. ターゲットが存在するか確認
	// クライアントの場合：存在しなければエラー、存在すれば何もしない　チャンネルの場合：存在しなければエラー、存在すれば処理を続行
	if (!isChannel)
	{
		if (!server->isClientExist(target))
			addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(clientNick, target));
		return ;
	}
	else if (!server->isChannelExist(target))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(clientNick, target));
		return ;
	}

	// 6. クライアントがチャンネルに参加しているか確認
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(target)->second;
	if (!channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(clientNick, target));
		return;
	}

	// 7. 「MODE #channelname」の場合、チャンネルのモード情報をクライアントに通知
	if (words.size() == 1)
	{
		// チャンネルのモード情報をクライアントに通知
		infoChannelMode(server, clientFd, channel, target);
		return;
	}

	// 8. クライアントがチャンネルオペレーターであるか確認
	if (!channel.isOperator(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_CHANOPRIVSNEEDED(clientNick, target));
		return;
	}

	// 9. 追加パラメータ（mode arguments）の抽出（存在する場合）
	std::string modeString = words[1];
	std::string modeArgs = "";
	if (words.size() > 2)
		modeArgs = words[2];

	// 10. MODEコマンドの処理
	processModeChanges(server, client, channel, modeString, modeArgs);
}
