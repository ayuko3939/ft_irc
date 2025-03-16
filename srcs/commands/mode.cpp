/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/16 23:30:04 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// コマンド形式: MODE <channel> <modestring> [<mode arguments>...]
static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words)
{
    // MODEコマンドでは、最低でもチャンネル名とモード文字列の2個の引数が必要
    if (words.size() < 2)
    {
        addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(MODE_USAGE));
        return (false);
    }
    return (true);
}

static bool isValid(Server *server, int const clientFd, std::string channelName, std::string modeString)
{
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();

	if (channelName.empty() || modeString.empty())
	{
		addToClientSendBuf(server, clientFd, ERR_PARM_EMPTY);
		return (false);
	}
	// チャンネルの存在確認
	if (!server->isChannelExist(channelName))
	{
		addToClientSendBuf(server, clientFd, ERR_NOSUCHCHANNEL(issuerNick, channelName));
		return (false);
	}
	// 先頭が '+' または '-' であること
	if (modeString[0] != '+' && modeString[0] != '-')
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(" MODE: Modestring must start with '+' or '-'"));
		return (false);
	}
	// モード文字列が2文字以上であること
	if (modeString.size() < 2)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(" MODE: Modestring must have at least 2 characters"));
		return (false);
	}
	// モード文字列が正しい文字列であること
	std::string modeChars = modeString.substr(1);
	if (modeChars.find_first_not_of("itkol") != std::string::npos)
	{
		addToClientSendBuf(server, clientFd, ERR_INVALID_PARM + std::string(" MODE: Invalid mode character"));
		return (false);
	}
	return (true);
}

// MODEコマンドで、各オプションの具体的な処理は別に実装するため
// ここでは、モード変更の適用を呼び出すプロトタイプ関数を利用する（oオプションは未実装）
void processModeChanges(Server *server, Channel &channel, Client &client, const std::string &modeStr, const std::vector<std::string> &modeArgs);

// コマンド形式: MODE <channel> <modestring> [<mode arguments>...]
void mode(Server *server, int const clientFd, s_ircCommand cmdInfo)
{
	// 1. 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return;

	// 2. パラメータからチャンネル名とモード文字列を取得
	std::string channelName = words[0];
	std::string modeString = words[1];

	// 3. パラメータの妥当性確認
	if (!isValid(server, clientFd, channelName, modeString))
		return;

	// 4. 発行者のクライアント情報を取得
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();

	std::map<std::string, Channel> &channels = server->getChannelList();
	std::map<std::string, Channel>::iterator chanIt = channels.find(channelName);
	Channel &channel = chanIt->second;

	// 6. 発行者がそのチャンネルに参加しているか確認
	if (!channel.isClientInChannel(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_NOTONCHANNEL(issuerNick, channelName));
		return;
	}

	// 7. 発行者がチャンネルオペレーターであるか確認
	if (!channel.isOperator(clientFd))
	{
		addToClientSendBuf(server, clientFd, ERR_CHANOPRIVSNEEDED(issuerNick, channelName));
		return;
	}

	// 8. 追加パラメータ（mode arguments）の抽出（存在する場合）
	std::vector<std::string> modeArgs;
	if (words.size() > 2)
	{
		for (size_t i = 2; i < words.size(); i++)
		{
			modeArgs.push_back(words[i]);
		}
	}

	// 9. MODEコマンドの処理
	// 今回は「o」オプション以外の処理のみ対応するので、processModeChanges関数内で
	// 各オプション（例: i, t, k, l）を個別に処理し、"o"オプションはスキップするように実装する
	processModeChanges(server, channel, client, modeString, modeArgs);

	// 10. 変更されたモード情報をチャンネル内全メンバーに通知する
	// ※ 具体的なブロードキャスト処理は、各オプションの実装後に追加する可能性がある
}
