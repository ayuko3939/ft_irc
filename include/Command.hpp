/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:30:39 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/18 22:25:25 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Irc.hpp"
# include "Server.hpp"

// Command functions
void invite(Server *server, const int clientFd, s_ircCommand cmdInfo);	// ユーザーをチャンネルに招待
void join(Server *server, const int clientFd, s_ircCommand cmdInfo);	// チャンネルに参加、作成
void kick(Server *server, const int clientFd, s_ircCommand cmdInfo);	// チャンネルからユーザーをキック
void mode(Server *server, const int clientFd, s_ircCommand cmdInfo);	// チャンネルモードの変更
void nick(Server *server, const int clientFd, s_ircCommand cmdInfo);	// ニックネームの変更
void part(Server *server, const int clientFd, s_ircCommand cmdInfo);	// チャンネルから退出
void pass(Server *server, const int clientFd, s_ircCommand cmdInfo);	// サーバーパスワードの認証（初回のみ）
void privmsg(Server *server, const int clientFd, s_ircCommand cmdInfo);	// メッセージ送信
void quit(Server *server, const int clientFd, s_ircCommand cmdInfo);	// クライアントの切断
void topic(Server *server, const int clientFd, s_ircCommand cmdInfo);	// トピックの変更
void user(Server *server, const int clientFd, s_ircCommand cmdInfo);	// ユーザー情報の登録（変更不可）

// mode option functions
void inviteOnlyMode(Server *server, Channel &channel, Client &client, bool sign);
void channelKeyMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs);
void userLimitMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs);
void operatorMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs);
void topicProtectMode(Server *server, Channel &channel, Client &client, bool sign);

// Command utils
std::vector<std::string> splitMessage(std::string message);
std::vector<std::string> splitByComma(const std::string &str);

#endif