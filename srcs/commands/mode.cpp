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

// MODE <channel> [<modestring> [<mode arguments>...]]
static bool checkAndGetChannel(Server *server, int const clientFd, std::vector<std::string> &words, 
	std::string &channelName)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";
	int wordsSize = words.size();

	// 引数がない場合はエラー
	if (wordsSize == 0 || words[0].empty())
	{
		errMessage = ERR_NEEDMOREPARAMS(nickname, "MODE");
		errMessage += MODE_USAGE;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// チャンネルが存在するか確認
	channelName = getChannelNameFromWord(words[0]);
	if (!server->isChannelExist(channelName))
	{
		// チャンネルではなくクライアントが指定されている場合
		if (server->isClientExist(channelName))
			return (false);

		errMessage = ERR_NOSUCHCHANNEL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// クライアントがそのチャンネルに参加しているか確認
	if (!channel.isClientInChannel(clientFd))
	{
		errMessage = ERR_NOTONCHANNEL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

// MODE <channel> [<modestring> [<mode arguments>...]]
static bool checkModeArguments(Server *server, int const clientFd, 
	Channel &channel, std::string &modeString, std::string &modeArgs)
{
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";


	// モード文字列が正しいか確認(+i, -i, +t, -t, +k, -k, +o, -o, +l, -l)
	if (modeString.empty() || modeString.size() != 2 ||
		((modeString[0] != '+' && modeString[0] != '-')) ||
		((modeString[1] != 'i' && modeString[1] != 't' && modeString[1] != 'k' && modeString[1] != 'o' && modeString[1] != 'l')))
	{
		errMessage = ERR_UNKNOWNMODE(nickname, modeString);
		errMessage += MODE_REQUIREMENTS;
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// <mode arguments> が必要なモード文字列の場合、引数があるか確認
	// +k <password>  ,  +l <limit>  ,  +o <nickname>  は引数が必要
	if (modeString == "+k" || modeString == "+o" || modeString == "-o" || modeString == "+l")
	{
		if (modeArgs.empty())
		{
			errMessage = ERR_NEEDMOREPARAMS(nickname, "MODE");
			errMessage += MODE_USAGE_K_O_L;
			addToClientSendBuf(server, clientFd, errMessage);
			return (false);
		}
	}
	// それ以外のモード文字列は引数不要
	else
	{
		if (!modeArgs.empty())
		{
			errMessage = ERR_INVALID_PARM;
			errMessage += MODE_USAGE;
			addToClientSendBuf(server, clientFd, errMessage);
			return (false);
		}
	}

	// クライアントがそのチャンネルのオペレーターであるか確認
	if (!channel.isOperator(clientFd))
	{
		errMessage = ERR_CHANOPRIVSNEEDED(nickname, channel.getName());
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

static std::string getChannelModeString(Channel &channel)
{
	std::string modeString = "+";
	if (channel.getMode("i"))
		modeString += "i";
	if (channel.getMode("t"))
		modeString += "t";
	if (channel.getMode("k"))
		modeString += "k";
	if (channel.getMode("o"))
		modeString += "o";
	if (channel.getMode("l"))
		modeString += "l";
	if (modeString == "+")
		modeString = "all off";
	return (modeString);
}

static void infoChannelMode(Server *server, int const clientFd, Channel &channel, std::string channelName)
{
	std::string modeInfo = "";
	std::string modeString = getChannelModeString(channel);

	modeInfo = RPL_CHANNELMODEIS(server->getNickname(clientFd), channelName, modeString, channel.getPassword());
	modeInfo += "========== " + channelName + "'s mode ==========\r\n";
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
			break;
	}
}

// コマンド形式: MODE <channel> <modestring> [<mode arguments>...]
void mode(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	std::string channelName = "";

	// 1. 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 2. 1つ目の引数をチャンネル名として取得
	if (!checkAndGetChannel(server, clientFd, words, channelName))
		return;

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;
	
	// 3. 「MODE #channelname」の場合、チャンネルのモード情報をクライアントに通知
	if (words.size() == 1)
	{
		// チャンネルのモード情報をクライアントに通知
		infoChannelMode(server, clientFd, channel, channelName);
		return;
	}

	// 4. 追加パラメータ（mode arguments）を取得
	std::string modeString = words[1];
	std::string modeArgs = "";
	if (words.size() > 2)
		modeArgs = words[2];

	// 5. モード文字列と引数の妥当性をチェック
	if (!checkModeArguments(server, clientFd, channel, modeString, modeArgs))
		return;

	// 6. MODEコマンドの処理
	Client &client = retrieveClient(server, clientFd);
	processModeChanges(server, client, channel, modeString, modeArgs);
}

/*
Numeric Replies:
 User mode
// 	ERR_NOSUCHNICK (401)		// <channelName> が存在しないニックネームの場合
// 	ERR_USERSDONTMATCH (502)	// <channelName> がコマンド送信者自身と異なる場合
// 	RPL_UMODEIS (221)			// <modestring> が指定されない場合は、現在のユーザモードを示す
// 	ERR_UMODEUNKNOWNFLAG (501)	// サーバがサポートしていないモードが含まれる場合は、実装されているモードは適用
 Channel mode
	ERR_NOSUCHCHANNEL (403)		// <channelName> が存在しないチャネルの場合
	RPL_CHANNELMODEIS (324)		// <modestring> が指定されない場合は、現在のチャネルモードを示す
// 	RPL_CREATIONTIME (329)		// 必要に応じて
	RR_CHANOPRIVSNEEDED (482)	// <modestring> が指定された場合、権限がない場合
// 	RPL_BANLIST (367)			// 例: BAN リスト (“+b”)
// 	RPL_ENDOFBANLIST (368)
// 	RPL_EXCEPTLIST (348)		// 例: EXCEPT リスト (“+e”)
// 	RPL_ENDOFEXCEPTLIST (349)
// 	RPL_INVITELIST (336)		// 例: INVITE リスト (“+I”)
// 	RPL_ENDOFINVITELIST (337)
*/