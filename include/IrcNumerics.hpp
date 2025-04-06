/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:07:12 by ohasega           #+#    #+#             */
/*   Updated: 2025/04/06 19:36:57 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCNUMERICS_HPP
# define IRCNUMERICS_HPP

// Length of parameters
# define USERNAMELEN 20
# define REALNAMELEN 20
# define NICKLEN 10
# define CHANNELLEN 20
# define KEYLEN 10
# define TOPICLEN 50
# define KICKLEN 400
# define PARTLEN 400
# define MSGLEN 400
# define QUITLEN 400

// Error message
# define ERR_INVALID_PARM		"Error: Invalid parameters.\r\n"
# define ERR_PASS_AUTH_YET		"Error: Password authentication is not complete. Please \"PASS\".\r\n"

// Command usage and requirements
# define NICK_USAGE			    "Usage: NICK <nickname>\r\n"
# define NICK_REQUIREMENTS  	"[!] nickname requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
# define USER_USAGE		    	"Usage: USER <username> 0 * <realname>\r\n"
# define USER_REQUIREMENTS  	"[!] username requirements: len(20), char(a-z, A-Z, 0-9)\r\n    realname requirements: len(20), char(a-z, A-Z, 0-9, ' ')\r\n"
# define PASS_USAGE		    	"Usage: PASS <password>\r\n"
# define JOIN_USAGE		    	"Usage: JOIN <channel> [key]\r\n"
# define JOIN_REQUIREMENTS  	"[!] channel requirements: len(20), char(a-z, A-Z, 0-9)\r\n    key requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
# define TOPIC_USAGE	    	"Usage: TOPIC <channel> [<topic>]   If <topic> is not given, display topic\r\n"
# define TOPIC_REQUIREMENTS 	"[!] topic requirements: len(50), char(a-z, A-Z, 0-9)\r\n"
# define INVITE_USAGE	    	"Usage: INVITE <nickname> <channel>\r\n"
# define KICK_USAGE		    	"Usage: KICK <channel> <nickname> [<comment>]\r\n"
# define KICK_REQUIREMENTS  	"[!] comment requirements: len(50)\r\n"
# define MODE_USAGE		    	"MODE <channel> [<modestring> [<mode arguments>]]   If <modestring> is not given, display mode\r\n"
# define MODE_USAGE_K_O_L   	"Usage: MODE <channel> +k <password>  |  +o <nickname>  |  -o <nickname>  |  +l <limit>\r\n"
# define MODE_REQUIREMENTS  	"[!] modestring requirements: option sign( + , - ) + frag( i, t, k, o, l )\r\n"
# define MODE_REQ_K_PASS    	"[!] password requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
# define MODE_REQ_L_LIMIT   	"[!] limit requirements: int(1-4)\r\n"
# define PRIVMSG_USAGE	    	"Usage: PRIVMSG <nickname/channel> <message>\r\n"
# define PRIVMSG_REQUIREMENTS	"[!] message requirements: len(400)\r\n"
# define PART_USAGE			    "Usage: PART <channel> [<reason>]\r\n"
# define PART_REQUIREMENTS	    "[!] reason requirements: len(50)\r\n"

// Success message
# define IRC_PREFIX(nickname, username) (":" + nickname + "!" + username + "@localhost")
# define RPL_PASS "Password accepted\r\n"
# define RPL_USER(IRC_PREFIX, user, real) (IRC_PREFIX + " :Your username is " + user + ", realname is " + real + "\r\n")
# define RPL_NICK(IRC_PREFIX, newNick) (IRC_PREFIX + " NICK " + newNick + "\r\n")
# define RPL_JOIN(IRC_PREFIX, channel) (IRC_PREFIX + " JOIN :#" + channel + "\r\n")
# define RPL_TOPICSET(IRC_PREFIX, channel, topic) (IRC_PREFIX + " TOPIC #" + channel + " " + topic + "\r\n")
# define RPL_INVITE(IRC_PREFIX, target, channelName) (IRC_PREFIX + " INVITE " + target + " #" + channelName + "\r\n")
# define RPL_KICK(IRC_PREFIX, channel, target, comment) (IRC_PREFIX + " KICK #" + channel + " " + target + " " + comment + "\r\n")
# define RPL_MODE(IRC_PREFIX, channel, modestring) (IRC_PREFIX + " MODE #" + channel + " " + modestring + "\r\n")
# define RPL_PRIVMSG(IRC_PREFIX, target, message) (IRC_PREFIX + " PRIVMSG " + target + " " + message + "\r\n")
# define RPL_PART(IRC_PREFIX, channel, reason) (IRC_PREFIX + " PART #" + channel + " " + reason + "\r\n")
# define RPL_QUIT(IRC_PREFIX, reason) (IRC_PREFIX + " :QUIT:" + reason + "\r\n")

# define DEFAULT_KICK_COMMENT	":Kicked by channel operator"
# define DELIMITER_LINE "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\r\n"


// (001) RPL_WELCOME : クライアント登録直後のウェルカムメッセージ
# define RPL_WELCOME(client, nick) (":ircserv 001 " + client + " :Welcome to the Internet Relay Network, " + nick + "\r\n")
// (002) RPL_YOURHOST : 接続中のサーバ名とバージョン情報
# define RPL_YOURHOST(client, server, version) (":ircserv 002 " + client + " :Your host is " + server + ", running version " + version + "\r\n")
// (003) RPL_CREATED : サーバの作成日時情報
# define RPL_CREATED(client, datetime) (":ircserv 003 " + client + " :This server was created " + datetime + "\r\n")
// (004) RPL_MYINFO : サーバ名とバージョン情報のみ
# define RPL_MYINFO(client, server, version) (":ircserv 004 " + client + " " + server + " " + version + "\r\n")

// // (001) RPL_WELCOME : クライアントへ送信するウェルカムメッセージ
// # define RPL_WELCOME(client, networkname, nick, user, host) (":localhost 001 " + client + " :Welcome to the " + networkname + " Network, " + nick + "[!" + user + "@" + host + "]\r\n")

// // (002) RPL_YOURHOST : 接続先サーバの名前とバージョン情報
// # define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + ", running version " + version + "\r\n")

// // (003) RPL_CREATED : サーバの作成日時情報
// # define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")

// // (004) RPL_MYINFO : サーバ名、バージョン、利用可能ユーザ・チャネルモード情報
// # define RPL_MYINFO(client, servername, version, userModes, channelModes, channelModesParam) (":localhost 004 " + client + " " + servername + " " + version + " " + userModes + " " + channelModes + " " + channelModesParam + "\r\n")

// // (005) RPL_ISUPPORT : サーバがサポートしている機能の一覧
// # define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens + " :are supported by this server\r\n")

// // (010) RPL_BOUNCE : 別サーバへリダイレクトする際の情報
// # define RPL_BOUNCE(client, hostname, port, info) (":localhost 010 " + client + " " + hostname + " " + port + " :" + info + "\r\n")

// // (212) RPL_STATSCOMMANDS : 各コマンドの使用統計情報
// # define RPL_STATSCOMMANDS(client, command, count, byteCount, remoteCount) (":localhost 212 " + client + " " + command + " " + count + " " + byteCount + " " + remoteCount + "\r\n")

// // (219) RPL_ENDOFSTATS : 統計情報応答の終了
// # define RPL_ENDOFSTATS(client, statsLetter) (":localhost 219 " + client + " " + statsLetter + " :End of /STATS report\r\n")

// // (221) RPL_UMODEIS : 現在のユーザモード情報
// # define RPL_UMODEIS(client, userModes) (":localhost 221 " + client + " " + userModes + "\r\n")

// // (242) RPL_STATSUPTIME : サーバの稼働時間情報
// # define RPL_STATSUPTIME(client, days, hours, minutes, seconds) (":localhost 242 " + client + " :Server Up " + days + " days " + hours + ":" + minutes + ":" + seconds + "\r\n")

// // (251) RPL_LUSERCLIENT : 総ユーザ数、非表示ユーザ数、接続サーバ数の情報
// # define RPL_LUSERCLIENT(client, u, i, s) (":localhost 251 " + client + " :There are " + u + " users and " + i + " invisible on " + s + " servers\r\n")

// // (252) RPL_LUSEROP : オペレーター数の情報
// # define RPL_LUSEROP(client, ops) (":localhost 252 " + client + " " + ops + " :operator(s) online\r\n")

// // (253) RPL_LUSERUNKNOWN : 不明な接続状態の数
// # define RPL_LUSERUNKNOWN(client, connections) (":localhost 253 " + client + " " + connections + " :unknown connection(s)\r\n")

// // (254) RPL_LUSERCHANNELS : 存在するチャネル数の情報
// # define RPL_LUSERCHANNELS(client, channels) (":localhost 254 " + client + " " + channels + " :channels formed\r\n")

// // (255) RPL_LUSERME : 自サーバに接続中のクライアント数情報
// # define RPL_LUSERME(client, c, s) (":localhost 255 " + client + " :I have " + c + " clients and " + s + " servers\r\n")

// // (256) RPL_ADMINME : 管理情報提供対象のサーバ名（オプション）
// # define RPL_ADMINME(client, server) (":localhost 256 " + client + " " + server + " :Administrative info\r\n")

// // (257) RPL_ADMINLOC1 : サーバ所在地などの管理情報（1行目）
// # define RPL_ADMINLOC1(client, info) (":localhost 257 " + client + " :" + info + "\r\n")

// // (258) RPL_ADMINLOC2 : サーバ運営者に関する管理情報（2行目）
// # define RPL_ADMINLOC2(client, info) (":localhost 258 " + client + " :" + info + "\r\n")

// // (259) RPL_ADMINEMAIL : 管理者連絡用メールアドレス
// # define RPL_ADMINEMAIL(client, info) (":localhost 259 " + client + " :" + info + "\r\n")

// // (263) RPL_TRYAGAIN : 再試行を促すエラーメッセージ
// # define RPL_TRYAGAIN(client, command) (":localhost 263 " + client + " " + command + " :Please wait a while and try again.\r\n")

// // (265) RPL_LOCALUSERS : ローカル接続ユーザ数と最大接続数の情報
// # define RPL_LOCALUSERS(client, u, m) (":localhost 265 " + client + " " + u + " " + m + " :Current local users " + u + ", max " + m + "\r\n")

// // (266) RPL_GLOBALUSERS : グローバルな接続ユーザ数と最大接続数の情報
// # define RPL_GLOBALUSERS(client, u, m) (":localhost 266 " + client + " " + u + " " + m + " :Current global users " + u + ", max " + m + "\r\n")

// // (276) RPL_WHOISCERTFP : WHOISで表示するクライアント証明書フィンガープリント情報
// # define RPL_WHOISCERTFP(client, nick, fingerprint) (":localhost 276 " + client + " " + nick + " :has client certificate fingerprint " + fingerprint + "\r\n")

// // (300) RPL_NONE : ダミー（未定義）数値
// # define RPL_NONE(client) (":localhost 300 " + client + "\r\n")

// // (301) RPL_AWAY : 対象ニックの不在状態と不在メッセージ
// # define RPL_AWAY(client, nick, message) (":localhost 301 " + client + " " + nick + " :" + message + "\r\n")

// // (302) RPL_USERHOST : ユーザホスト情報のリスト
// # define RPL_USERHOST(client, replies) (":localhost 302 " + client + " :" + replies + "\r\n")

// // (305) RPL_UNAWAY : 不在状態解除の通知
// # define RPL_UNAWAY(client) (":localhost 305 " + client + " :You are no longer marked as being away\r\n")

// // (306) RPL_NOWAWAY : 不在状態設定の通知
// # define RPL_NOWAWAY(client) (":localhost 306 " + client + " :You have been marked as being away\r\n")

// // (307) RPL_WHOISREGNICK : WHOISで認証済みであることの通知
// # define RPL_WHOISREGNICK(client, nick) (":localhost 307 " + client + " " + nick + " :has identified for this nick\r\n")

// // (311) RPL_WHOISUSER : WHOISでのユーザ基本情報（ニック、ユーザ名、ホスト、実名）
// # define RPL_WHOISUSER(client, nick, username, host, realname) (":localhost 311 " + client + " " + nick + " " + username + " " + host + " * :" + realname + "\r\n")

// // (312) RPL_WHOISSERVER : WHOISでの接続サーバ情報
// # define RPL_WHOISSERVER(client, nick, server, serverInfo) (":localhost 312 " + client + " " + nick + " " + server + " :" + serverInfo + "\r\n")

// // (313) RPL_WHOISOPERATOR : WHOISでオペレーターであることの通知
// # define RPL_WHOISOPERATOR(client, nick) (":localhost 313 " + client + " " + nick + " :is an IRC operator\r\n")

// // (314) RPL_WHOWASUSER : WHOWASでの過去ユーザ情報
// # define RPL_WHOWASUSER(client, nick, username, host, realname) (":localhost 314 " + client + " " + nick + " " + username + " " + host + " * :" + realname + "\r\n")

// // (315) RPL_ENDOFWHO : WHO応答終了の通知
// # define RPL_ENDOFWHO(client, mask) (":localhost 315 " + client + " " + mask + " :End of WHO list\r\n")

// // (317) RPL_WHOISIDLE : WHOISでのアイドル時間および接続時刻情報
// # define RPL_WHOISIDLE(client, nick, secs, signon) (":localhost 317 " + client + " " + nick + " " + secs + " " + signon + " :seconds idle, signon time\r\n")

// // (318) RPL_ENDOFWHOIS : WHOIS応答終了の通知
// # define RPL_ENDOFWHOIS(client, nick) (":localhost 318 " + client + " " + nick + " :End of /WHOIS list\r\n")

// // (319) RPL_WHOISCHANNELS : WHOISでの所属チャネル一覧
// # define RPL_WHOISCHANNELS(client, nick, channels) (":localhost 319 " + client + " " + nick + " :" + channels + "\r\n")

// // (320) RPL_WHOISSPECIAL : WHOISでの追加情報（自由記述）
// # define RPL_WHOISSPECIAL(client, nick) (":localhost 320 " + client + " " + nick + " :blah blah blah\r\n")

// // (321) RPL_LISTSTART : チャネルリストの開始通知
// # define RPL_LISTSTART(client) (":localhost 321 " + client + " Channel :Users  Name\r\n")

// // (322) RPL_LIST : チャネル情報（チャネル名、参加者数、トピック）
// # define RPL_LIST(client, channel, clientCount, topic) (":localhost 322 " + client + " " + channel + " " + clientCount + " :" + topic + "\r\n")

// // (323) RPL_LISTEND : チャネルリストの終了通知
// # define RPL_LISTEND(client) (":localhost 323 " + client + " :End of /LIST\r\n")

// (324) RPL_CHANNELMODEIS : チャネルの現在のモード情報
# define RPL_CHANNELMODEIS(client, channel, modestring, modeArgs) (":localhost 324 " + client + " " + channel + " " + modestring + " " + modeArgs + "\r\n")

// // (329) RPL_CREATIONTIME : チャネル作成時刻情報
// # define RPL_CREATIONTIME(client, channel, creationtime) (":localhost 329 " + client + " " + channel + " " + creationtime + "\r\n")

// // (330) RPL_WHOISACCOUNT : WHOISでのログイン（アカウント）情報
// # define RPL_WHOISACCOUNT(client, nick, account) (":localhost 330 " + client + " " + nick + " " + account + " :is logged in as\r\n")

// (331) RPL_NOTOPIC : チャネルのトピック未設定状態
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")

// (332) RPL_TOPIC : チャネルの現在のトピック情報
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")

// // (333) RPL_TOPICWHOTIME : トピック設定者と設定時刻情報
// # define RPL_TOPICWHOTIME(client, channel, nick, setat) (":localhost 333 " + client + " " + channel + " " + nick + " " + setat + "\r\n")

// // (336) RPL_INVITELIST : 招待リストに登録されているチャネル情報
// # define RPL_INVITELIST(client, channel) (":localhost 336 " + client + " " + channel + "\r\n")

// // (337) RPL_ENDOFINVITELIST : 招待リストの終了通知
// # define RPL_ENDOFINVITELIST(client) (":localhost 337 " + client + " :End of /INVITE list\r\n")

// (341) RPL_INVITING : 招待リストに登録されているチャネル情報
# define RPL_INVITING(IRC_PREFIX, client, nick, channel) (IRC_PREFIX + " 341 " + client + " " + nick + " #" + channel + "\r\n")

// // (346) RPL_INVEXLIST : チャネル招待除外リストのエントリ情報
// # define RPL_INVEXLIST(client, channel, mask) (":localhost 346 " + client + " " + channel + " " + mask + "\r\n")

// // (347) RPL_ENDOFINVEXLIST : 招待除外リストの終了通知
// # define RPL_ENDOFINVEXLIST(client, channel) (":localhost 347 " + client + " " + channel + " :End of Channel Invite Exception List\r\n")

// // (348) RPL_EXCEPTLIST : チャネル例外リストのエントリ情報
// # define RPL_EXCEPTLIST(client, channel, mask) (":localhost 348 " + client + " " + channel + " " + mask + "\r\n")

// // (349) RPL_ENDOFEXCEPTLIST : 例外リストの終了通知
// # define RPL_ENDOFEXCEPTLIST(client, channel) (":localhost 349 " + client + " " + channel + " :End of channel exception list\r\n")

// // (351) RPL_VERSION : サーバのバージョン情報とコメント
// # define RPL_VERSION(client, version, server, comments) (":localhost 351 " + client + " " + version + " " + server + " :" + comments + "\r\n")

// // (352) RPL_WHOREPLY : WHO応答の詳細情報（チャネル、ユーザ情報等）
// # define RPL_WHOREPLY(client, channel, username, host, server, nick, flags, hopcount, realname) (":localhost 352 " + client + " " + channel + " " + username + " " + host + " " + server + " " + nick + " " + flags + " :" + hopcount + " " + realname + "\r\n")

// (353) RPL_NAMREPLY : NAMES応答（チャネル参加者一覧）
# define RPL_NAMREPLY(client, symbol, channel, nicks) (":localhost 353 " + client + " " + symbol + " " + channel + " :" + nicks + "\r\n")

// // (364) RPL_LINKS : サーバ間リンク情報
// # define RPL_LINKS(client, server, hopcount, serverInfo) (":localhost 364 " + client + " * " + server + " :" + hopcount + " " + serverInfo + "\r\n")

// // (365) RPL_ENDOFLINKS : リンクリストの終了通知
// # define RPL_ENDOFLINKS(client) (":localhost 365 " + client + " * :End of /LINKS list\r\n")

// (366) RPL_ENDOFNAMES : NAMES応答の終了通知
# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of /NAMES list\r\n")

// // (367) RPL_BANLIST : チャネルBANリストのエントリ情報（オプション付き）
// # define RPL_BANLIST(client, channel, mask, who, setts) (":localhost 367 " + client + " " + channel + " " + mask + " " + who + " " + setts + "\r\n")

// // (368) RPL_ENDOFBANLIST : BANリストの終了通知
// # define RPL_ENDOFBANLIST(client, channel) (":localhost 368 " + client + " " + channel + " :End of channel ban list\r\n")

// // (369) RPL_ENDOFWHOWAS : WHOWAS応答の終了通知
// # define RPL_ENDOFWHOWAS(client, nick) (":localhost 369 " + client + " " + nick + " :End of WHOWAS\r\n")

// // (371) RPL_INFO : サーバに関する情報（INFOコマンド）
// # define RPL_INFO(client, string) (":localhost 371 " + client + " :" + string + "\r\n")

// // (372) RPL_MOTD : MOTDの各行情報
// # define RPL_MOTD(client, line) (":localhost 372 " + client + " :" + line + "\r\n")

// // (374) RPL_ENDOFINFO : INFO応答の終了通知
// # define RPL_ENDOFINFO(client) (":localhost 374 " + client + " :End of INFO list\r\n")

// // (375) RPL_MOTDSTART : MOTD開始の通知
// # define RPL_MOTDSTART(client, server) (":localhost 375 " + client + " :- " + server + " Message of the day - \r\n")

// // (376) RPL_ENDOFMOTD : MOTD終了の通知
// # define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of /MOTD command.\r\n")

// // (378) RPL_WHOISHOST : WHOISでの接続元ホスト情報
// # define RPL_WHOISHOST(client, nick) (":localhost 378 " + client + " " + nick + " :is connecting from *@localhost 127.0.0.1\r\n")

// // (379) RPL_WHOISMODES : WHOISでのユーザモード情報
// # define RPL_WHOISMODES(client, nick, modes) (":localhost 379 " + client + " " + nick + " :is using modes " + modes + "\r\n")

// // (381) RPL_YOUREOPER : オペレーター認証成功の通知
// # define RPL_YOUREOPER(client) (":localhost 381 " + client + " :You are now an IRC operator\r\n")

// // (382) RPL_REHASHING : REHASHコマンド実行中の通知
// # define RPL_REHASHING(client, configFile) (":localhost 382 " + client + " " + configFile + " :Rehashing\r\n")

// // (391) RPL_TIME : サーバの時刻情報
// # define RPL_TIME(client, server, timestamp, tsoffset, timeText) (":localhost 391 " + client + " " + server + " " + timestamp + " " + tsoffset + " :" + timeText + "\r\n")




// // (400) ERR_UNKNOWNERROR : 不明なエラー（一般的なエラー）
// # define ERR_UNKNOWNERROR(client, command, subcommand, info) (":localhost 400 " + client + " " + command + " " + subcommand + " :" + info + "\r\n")

// (401) ERR_NOSUCHNICK : 存在しないニックまたはチャネル
# define ERR_NOSUCHNICK(client, nickname) (":localhost 401 " + client + " " + nickname + " :No such nick/channel\r\n")

// // (402) ERR_NOSUCHSERVER : 存在しないサーバ名
// # define ERR_NOSUCHSERVER(client, servername) (":localhost 402 " + client + " " + servername + " :No such server\r\n")

// (403) ERR_NOSUCHCHANNEL : 存在しないチャネル名
# define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " #" + channel + " :No such channel\r\n")

// // (404) ERR_CANNOTSENDTOCHAN : チャネルへの送信ができない
// # define ERR_CANNOTSENDTOCHAN(client, channel) (":localhost 404 " + client + " " + channel + " :Cannot send to channel\r\n")

// (405) ERR_TOOMANYCHANNELS : チャネル参加数上限に達している
# define ERR_TOOMANYCHANNELS(client, channel) (":localhost 405 " + client + " " + channel + " :You have joined too many channels\r\n")

// // (406) ERR_WASNOSUCHNICK : WHOWASで存在しなかったニック
// # define ERR_WASNOSUCHNICK(client, nickname) (":localhost 406 " + client + " " + nickname + " :There was no such nickname\r\n")

// // (409) ERR_NOORIGIN : オリジンが指定されていない
// # define ERR_NOORIGIN(client) (":localhost 409 " + client + " :No origin specified\r\n")

// (411) ERR_NORECIPIENT : 宛先が指定されていない（コマンド名付き）
# define ERR_NORECIPIENT(client, command) (":localhost 411 " + client + " :No recipient given (" + command + ")\r\n")

// (412) ERR_NOTEXTTOSEND : 送信するテキストが指定されていない
# define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client + " :No text to send\r\n")

// // (417) ERR_INPUTTOOLONG : 入力行の長さが制限を超えている
// # define ERR_INPUTTOOLONG(client) (":localhost 417 " + client + " :Input line was too long\r\n")

// (421) ERR_UNKNOWNCOMMAND : 未知のコマンド
# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")

// // (422) ERR_NOMOTD : MOTDファイルが存在しない
// # define ERR_NOMOTD(client) (":localhost 422 " + client + " :MOTD File is missing\r\n")

// (431) ERR_NONICKNAMEGIVEN : ニックネームが指定されていない
# define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :No nickname given\r\n")

// (432) ERR_ERRONEUSNICKNAME : 不正なニックネーム
# define ERR_ERRONEUSNICKNAME(client, nick) (":localhost 432 " + client + " " + nick + " :Erroneus nickname\r\n")

// (433) ERR_NICKNAMEINUSE : 既に使用中のニックネーム
# define ERR_NICKNAMEINUSE(client, nick) (":localhost 433 " + client + " " + nick + " :Nickname is already in use\r\n")

// // (436) ERR_NICKCOLLISION : ニックネーム衝突（他サーバとの競合）
// # define ERR_NICKCOLLISION(client, nick, user, host) (":localhost 436 " + client + " " + nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")

// (437) ERR_ERRONEUSTARGET : 不正なターゲット*
# define ERR_ERRONEUSTARGET(client, target, info) (":localhost 437 " + client + " " + target + " :Erroneous target" + info + "\r\n")

// (441) ERR_USERNOTINCHANNEL : 指定されたチャネルに所属していない
# define ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")

// (442) ERR_NOTONCHANNEL : チャネルに参加していない
# define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " #" + channel + " :You're not on that channel\r\n")

// (443) ERR_USERONCHANNEL : 既にチャネルに参加している
# define ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " " + nick + " #" + channel + " :is already on channel\r\n")

// (451) ERR_NOTREGISTERED : クライアントが未登録である
# define ERR_NOTREGISTERED ":localhost 451  :You have not registered\r\n"

// (461) ERR_NEEDMOREPARAMS : パラメータが不足している
# define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")

// (462) ERR_ALREADYREGISTERED : 再登録は許可されていない
# define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister\r\n")

// (464) ERR_PASSWDMISMATCH : パスワードが一致しない
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")

// // (465) ERR_YOUREBANNEDCREEP : サーバからBANされているため接続できない
// # define ERR_YOUREBANNEDCREEP(client) (":localhost 465 " + client + " :You are banned from this server.\r\n")

// (471) ERR_CHANNELISFULL : チャネルの参加人数上限に達している
# define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n")

// (472) ERR_UNKNOWNMODE : 未知のモード文字が指定された
# define ERR_UNKNOWNMODE(client, modechar) (":localhost 472 " + client + " " + modechar + " :is unknown mode char to me\r\n")

// (473) ERR_INVITEONLYCHAN : 招待専用チャネルであるため参加できない
# define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " #" + channel + " :Cannot join channel (+i)\r\n")

// // (474) ERR_BANNEDFROMCHAN : BANによりチャネル参加が拒否されている
// # define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")

// (475) ERR_BADCHANNELKEY : チャネルキーが不正または不足している
# define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

// // (476) ERR_BADCHANMASK : 不正なチャネル名が指定された
// # define ERR_BADCHANMASK(channel) (":localhost 476 " + channel + " :Bad Channel Mask\r\n")

// // (481) ERR_NOPRIVILEGES : IRCオペレーターとしての権限が不足している
// # define ERR_NOPRIVILEGES(client) (":localhost 481 " + client + " :Permission Denied- You're not an IRC operator\r\n")

// (482) ERR_CHANOPRIVSNEEDED : チャネル管理権限が不足している
# define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " #" + channel + " :You're not channel operator\r\n")

// // (483) ERR_CANTKILLSERVER : サーバを強制終了することはできない
// # define ERR_CANTKILLSERVER(client) (":localhost 483 " + client + " :You cant kill a server!\r\n")

// // (491) ERR_NOOPERHOST : 接続元ホストによりオペレーター認証が拒否された
// # define ERR_NOOPERHOST(client) (":localhost 491 " + client + " :No O-lines for your host\r\n")

// // (501) ERR_UMODEUNKNOWNFLAG : 未知のユーザモードフラグが指定された
// # define ERR_UMODEUNKNOWNFLAG(client) (":localhost 501 " + client + " :Unknown MODE flag\r\n")

// // (502) ERR_USERSDONTMATCH : 他ユーザのモードを変更できない
// # define ERR_USERSDONTMATCH(client) (":localhost 502 " + client + " :Cant change mode for other users\r\n")

// // (524) ERR_HELPNOTFOUND : 指定されたトピックのヘルプ情報が見つからない
// # define ERR_HELPNOTFOUND(client, subject) (":localhost 524 " + client + " " + subject + " :No help available on this topic\r\n")

// (525) ERR_INVALIDKEY : チャネルキーが不正な形式である
# define ERR_INVALIDKEY(client, targetChan) (":localhost 525 " + client + " " + targetChan + " :Key is not well-formed\r\n")

// // (670) RPL_STARTTLS : STARTTLSコマンド成功（TLSハンドシェイク開始可能）
// # define RPL_STARTTLS(client) (":localhost 670 " + client + " :STARTTLS successful, proceed with TLS handshake\r\n")

// // (671) RPL_WHOISSECURE : WHOISで安全な接続状態であることの通知
// # define RPL_WHOISSECURE(client, nick) (":localhost 671 " + client + " " + nick + " :is using a secure connection\r\n")

// // (691) ERR_STARTTLS : STARTTLSコマンド失敗の通知
// # define ERR_STARTTLS(client) (":localhost 691 " + client + " :STARTTLS failed (Wrong moon phase)\r\n")

// // (696) ERR_INVALIDMODEPARAM : モードパラメータが不正である
// # define ERR_INVALIDMODEPARAM(client, target, modeChar, parameter, description) (":localhost 696 " + client + " " + target + " " + modeChar + " " + parameter + " :" + description + "\r\n")

// // (704) RPL_HELPSTART : ヘルプ応答の開始行
// # define RPL_HELPSTART(client, subject, firstLine) (":localhost 704 " + client + " " + subject + " :" + firstLine + "\r\n")

// // (705) RPL_HELPTXT : ヘルプテキストの各行
// # define RPL_HELPTXT(client, subject, helpLine) (":localhost 705 " + client + " " + subject + " :" + helpLine + "\r\n")

// // (706) RPL_ENDOFHELP : ヘルプ応答の終了行
// # define RPL_ENDOFHELP(client, subject, lastLine) (":localhost 706 " + client + " " + subject + " :" + lastLine + "\r\n")

// // (723) ERR_NOPRIVS : 指定されたオペレーター権限が不足している
// # define ERR_NOPRIVS(client, priv) (":localhost 723 " + client + " " + priv + " :Insufficient oper privileges.\r\n")

// // (900) RPL_LOGGEDIN : ログイン成功の通知（ニック、ユーザ、ホスト、アカウント情報付き）
// # define RPL_LOGGEDIN(client, nick, user, host, account, username) (":localhost 900 " + client + " " + nick + "!" + user + "@" + host + " " + account + " :You are now logged in as " + username + "\r\n")

// // (901) RPL_LOGGEDOUT : ログアウト成功の通知
// # define RPL_LOGGEDOUT(client, nick, user, host) (":localhost 901 " + client + " " + nick + "!" + user + "@" + host + " :You are now logged out\r\n")

// // (902) ERR_NICKLOCKED : ロックされたニックは使用できない
// # define ERR_NICKLOCKED(client) (":localhost 902 " + client + " :You must use a nick assigned to you\r\n")

// // (903) RPL_SASLSUCCESS : SASL認証成功の通知
// # define RPL_SASLSUCCESS(client) (":localhost 903 " + client + " :SASL authentication successful\r\n")

// // (904) ERR_SASLFAIL : SASL認証失敗の通知
// # define ERR_SASLFAIL(client) (":localhost 904 " + client + " :SASL authentication failed\r\n")

// // (905) ERR_SASLTOOLONG : SASLメッセージが長すぎる
// # define ERR_SASLTOOLONG(client) (":localhost 905 " + client + " :SASL message too long\r\n")

// // (906) ERR_SASLABORTED : SASL認証が中断された
// # define ERR_SASLABORTED(client) (":localhost 906 " + client + " :SASL authentication aborted\r\n")

// // (907) ERR_SASLALREADY : 既にSASL認証済みである
// # define ERR_SASLALREADY(client) (":localhost 907 " + client + " :You have already authenticated using SASL\r\n")

// // (908) RPL_SASLMECHS : 利用可能SASLメカニズム一覧
// # define RPL_SASLMECHS(client, mechanisms) (":localhost 908 " + client + " " + mechanisms + " :are available SASL mechanisms\r\n")

#endif