/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:07:12 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/29 19:34:51 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCNUMERICS_HPP
# define IRCNUMERICS_HPP

// エラーメッセージ
# define ERR_INVALID_PARM		"Error: Invalid parameters.\r\n"
# define ERR_ALREADYDONE(client) (":" + client + " :You are already done\r\n")
# define ERR_PASS_AUTH_YET		"Error: Password authentication is not complete. Please \"PASS\".\r\n"
# define ERR_REGISTRATION_YET	"Error: Registration is not complete. Please \"NICK\" and \"USER\".\r\n"
# define ERR_CMD_NOT_FOUND		"Error: Command not found.\r\n"
# define ERR_PARM_EMPTY			"Error: Parameter is empty.\r\n"

// コマンド要件
# define NICK_USAGE			"Usage: NICK <nickname>\r\n"
# define NICK_REQUIREMENTS	"[!] nickname requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
# define USER_USAGE			"Usage: USER <username> 0 * <realname>\r\n"
# define USER_REQUIREMENTS	"[!] username requirements: len(20), char(a-z, A-Z, 0-9)\r\n    realname requirements: len(2-20), char(a-z, A-Z, ' ')\r\n"
# define PASS_USAGE			"Usage: PASS <password>\r\n"
# define PASS_REQUIREMENTS	"[!] \r\n"
# define JOIN_USAGE			"Usage: JOIN <channel> [key]  or  JOIN <channel>{,<channel>} [<key>{,<key>}]\r\n"
# define JOIN_REQUIREMENTS	"[!] channel/key requirements: len(20), char(a-z, A-Z, 0-9)\r\n"
# define TOPIC_USAGE		"Usage: TOPIC <channel> [<topic>]   If <topic> is not given, display topic\r\n"
# define TOPIC_REQUIREMENTS	"[!] topic requirements: len(50), char(a-z, A-Z, 0-9)\r\n"
# define INVITE_USAGE		"Usage: INVITE <nickname> <channel>\r\n"
# define KICK_USAGE			"Usage: KICK <channel> <nickname> [<comment>]\r\n"
# define KICK_REQUIREMENTS	"[!] comment requirements: len(30)\r\n"
# define MODE_USAGE			"MODE <channel> [modestring> [<mode arguments>]]\r\n"
# define MODE_USAGE_K_O_L	"Usage: MODE <channel> +k <password>  |  +o <nickname>  |  -o <nickname>  |  +l <limit>\r\n"
# define MODE_REQUIREMENTS	"[!] modestring requirements: option sign( + , - ) + frag( i, t, k, o, l )\r\n"
# define MODE_REQ_K_PASS	"[!] password requirements: len(1-20), char(a-z, A-Z, 0-9)\r\n"
# define MODE_REQ_L_LIMIT(max)	("[!] limit requirements: (1-" + max + ")\r\n")
# define PRIVMSG_USAGE		"Usage: PRIVMSG <nickname/channel> <message>\r\n"
# define PRIVMSG_REQUIREMENTS	"[!] message requirements: len(400)\r\n"
# define PART_USAGE			"Usage: PART <channel> [<reason>]\r\n"
# define PART_REQUIREMENTS	"[!] reason requirements: len(30)\r\n"
# define QUIT_USAGE			"Usage: QUIT [<reason>]\r\n"
# define QUIT_REQUIREMENTS	"[!] reason requirements: len(30)\r\n"

# define IRC_PREFIX(nickname, username) (":" + nickname + "!" + username + "@localhost")
// 成功通知
# define RPL_NICK(client, newNick) (":" + client + " :Your nickname is " + newNick + "\r\n")
# define RPL_USER(client, user, real) (":" + client + " :Your username is " + user + ", realname is " + real + "\r\n")
# define RPL_PASS() ("Password accepted\r\n")
# define RPL_JOIN(IRC_PREFIX, channel) (IRC_PREFIX + " :joined #" + channel + "\r\n")
# define RPL_TOPI(IRC_PREFIX, channel, topic) (IRC_PREFIX + " : #" + channel + " topic has been changed to: " + topic + "\r\n")
# define RPL_INVITE(IRC_PREFIX, guest, channelName) (IRC_PREFIX + " :invited " + guest + " to " + channelName + "\r\n")
# define RPL_KICK(IRC_PREFIX, channel, target, comment) (IRC_PREFIX + " :kicked " + target + " from " + channel + " (" + comment + ")\r\n")
// # define RPL_MODE(IRC_PREFIX, channel, modestring) (IRC_PREFIX + " :channel mode " + modestring + " set to #" + channel + "\r\n")
# define RPL_PRIVMSG(IRC_PREFIX, target, message) (IRC_PREFIX + " :PRIVMSG " + target + " :" + message + "\r\n")
# define RPL_PART(IRC_PREFIX, channel, reason) (IRC_PREFIX + " :PART #" + channel + " (" + reason + ")\r\n")
# define RPL_QUIT(IRC_PREFIX, reason) (IRC_PREFIX + " :Quit (" + reason + ")\r\n")
# define SEY_BYE(IRC_PREFIX) (IRC_PREFIX + " : .*:+* Thanks for using IRC server. Goodbye! *+:*.")

# define DELIMITER_LINE "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\r\n"


// (001) RPL_WELCOME : クライアント登録直後の歓迎メッセージ
# define RPL_WELCOME(client, nick) (":ircserv 001 " + client + " :Welcome to the Internet Relay Network, " + nick + "\r\n")
// (002) RPL_YOURHOST : 接続中のサーバ名とバージョン情報
# define RPL_YOURHOST(client, server, version) (":ircserv 002 " + client + " :Your host is " + server + ", running version " + version + "\r\n")
// (003) RPL_CREATED : サーバ生成日時の通知
# define RPL_CREATED(client, datetime) (":ircserv 003 " + client + " :This server was created " + datetime + "\r\n")
// (004) RPL_MYINFO : サーバ名とバージョン情報のみ
# define RPL_MYINFO(client, server, version) (":ircserv 004 " + client + " " + server + " " + version + "\r\n")
// // (005) RPL_ISUPPORT : サポートトークンの通知
// #define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " :Supported tokens: " + tokens + "\r\n")

// // (010) RPL_BOUNCE : 別サーバへのリダイレクト
// #define RPL_BOUNCE(client, host, port) (":localhost 010 " + client + " :" + host + " " + port + "\r\n")

// // (212) RPL_STATSCOMMANDS : コマンド使用回数（簡素版：countのみ）
// #define RPL_STATSCOMMANDS(client, command, count) (":localhost 212 " + client + " :" + command + " " + count + "\r\n")

// // (219) RPL_ENDOFSTATS : STATS終了通知
// #define RPL_ENDOFSTATS(client, letter) (":localhost 219 " + client + " :" + letter + " :End of /STATS\r\n")

// // (221) RPL_UMODEIS : ユーザモード情報
// #define RPL_UMODEIS(client, modes) (":localhost 221 " + client + " :" + modes + "\r\n")

// // (242) RPL_STATSUPTIME : サーバ稼働時間（1つの文字列）
// #define RPL_STATSUPTIME(client, uptime) (":localhost 242 " + client + " :Server Up " + uptime + "\r\n")

// // (251) RPL_LUSERCLIENT : ユーザ数・サーバ数（簡素化：ユーザ数とサーバ数）
// #define RPL_LUSERCLIENT(client, users, servers) (":localhost 251 " + client + " :There are " + users + " users on " + servers + " servers\r\n")

// // (252) RPL_LUSEROP : オペレータ数
// #define RPL_LUSEROP(client, ops) (":localhost 252 " + client + " :" + ops + " :operators online\r\n")

// // (253) RPL_LUSERUNKNOWN : 未確認接続数
// #define RPL_LUSERUNKNOWN(client, unknown) (":localhost 253 " + client + " :" + unknown + " :unknown connection(s)\r\n")

// // (254) RPL_LUSERCHANNELS : チャネル数
// #define RPL_LUSERCHANNELS(client, channels) (":localhost 254 " + client + " :" + channels + " :channels formed\r\n")

// // (255) RPL_LUSERME : 自サーバのクライアント数とサーバ数（例示通り）
// // ※ このメッセージは元々固定文言のみのため、client 追加後は以下のようになります。
// #define RPL_LUSERME(client, clients, servers) (":localhost 255 " + client + " :I have " + clients + " clients and " + servers + " servers\r\n")

// // (256) RPL_ADMINME : 管理情報（infoのみ）
// #define RPL_ADMINME(client, info) (":localhost 256 " + client + " :" + info + "\r\n")

// // (257) RPL_ADMINLOC1 : サーバ所在地情報
// #define RPL_ADMINLOC1(client, info) (":localhost 257 " + client + " :" + info + "\r\n")

// // (258) RPL_ADMINLOC2 : 運営者情報
// #define RPL_ADMINLOC2(client, info) (":localhost 258 " + client + " :" + info + "\r\n")

// // (259) RPL_ADMINEMAIL : 管理者メールアドレス
// #define RPL_ADMINEMAIL(client, email) (":localhost 259 " + client + " :" + email + "\r\n")

// // (263) RPL_TRYAGAIN : 再試行促し
// #define RPL_TRYAGAIN(client, command) (":localhost 263 " + client + " :Please try again later (" + command + ")\r\n")

// // (265) RPL_LOCALUSERS : 直接接続ユーザ数と最大値
// #define RPL_LOCALUSERS(client, cur, max) (":localhost 265 " + client + " :Local users " + cur + ", max " + max + "\r\n")

// // (266) RPL_GLOBALUSERS : 全体接続ユーザ数と最大値
// #define RPL_GLOBALUSERS(client, cur, max) (":localhost 266 " + client + " :Global users " + cur + ", max " + max + "\r\n")

// // (276) RPL_WHOISCERTFP : 証明書フィンガープリント
// #define RPL_WHOISCERTFP(client, nick, fp) (":localhost 276 " + client + " :" + nick + " has fp " + fp + "\r\n")

// // (300) RPL_NONE : ダミーメッセージ
// #define RPL_NONE(client) (":localhost 300 " + client + " :\r\n")

// // (301) RPL_AWAY : away状態通知
// #define RPL_AWAY(client, nick, msg) (":localhost 301 " + client + " :" + nick + " is away: " + msg + "\r\n")

// // (302) RPL_USERHOST : ユーザホスト情報
// #define RPL_USERHOST(client, reply) (":localhost 302 " + client + " :" + reply + "\r\n")

// // (305) RPL_UNAWAY : away解除通知
// #define RPL_UNAWAY(client) (":localhost 305 " + client + " :You are no longer away\r\n")

// // (306) RPL_NOWAWAY : away設定通知
// #define RPL_NOWAWAY(client) (":localhost 306 " + client + " :You are now away\r\n")

// // (307) RPL_WHOISREGNICK : 認証済みニック通知
// #define RPL_WHOISREGNICK(client, nick) (":localhost 307 " + client + " :"+ nick + " is identified\r\n")

// // (311) RPL_WHOISUSER : WHOIS基本情報
// #define RPL_WHOISUSER(client, nick, user, host, realname) (":localhost 311 " + client + " :" + nick + " " + user + " " + host + " * :" + realname + "\r\n")

// // (312) RPL_WHOISSERVER : 接続サーバ名
// #define RPL_WHOISSERVER(client, nick, server) (":localhost 312 " + client + " :" + nick + " is on " + server + "\r\n")

// // (313) RPL_WHOISOPERATOR : オペレータ通知
// #define RPL_WHOISOPERATOR(client, nick) (":localhost 313 " + client + " :" + nick + " is an IRC operator\r\n")

// // (314) RPL_WHOWASUSER : WHOWAS情報
// #define RPL_WHOWASUSER(client, nick, user, host, realname) (":localhost 314 " + client + " :" + nick + " " + user + " " + host + " * :" + realname + "\r\n")

// // (315) RPL_ENDOFWHO : WHO終了
// #define RPL_ENDOFWHO(client, mask) (":localhost 315 " + client + " :End of WHO (" + mask + ")\r\n")

// // (317) RPL_WHOISIDLE : アイデル時間
// #define RPL_WHOISIDLE(client, nick, secs, signon) (":localhost 317 " + client + " :" + nick + " idle " + secs + ", signon " + signon + "\r\n")

// // (318) RPL_ENDOFWHOIS : WHOIS終了
// #define RPL_ENDOFWHOIS(client, nick) (":localhost 318 " + client + " :End of WHOIS for " + nick + "\r\n")

// // (319) RPL_WHOISCHANNELS : 参加チャネル一覧
// #define RPL_WHOISCHANNELS(client, nick, chans) (":localhost 319 " + client + " :" + nick + " is on " + chans + "\r\n")

// // (320) RPL_WHOISSPECIAL : 補足情報
// #define RPL_WHOISSPECIAL(client, nick, info) (":localhost 320 " + client + " :" + nick + " " + info + "\r\n")

// // (321) RPL_LISTSTART : チャネルリスト開始
// #define RPL_LISTSTART(client) (":localhost 321 " + client + " :Channel :Users  Name\r\n")

// // (322) RPL_LIST : チャネル情報
// #define RPL_LIST(client, channel, count, topic) (":localhost 322 " + client + " :" + channel + " " + count + " :" + topic + "\r\n")

// // (323) RPL_LISTEND : チャネルリスト終了
// #define RPL_LISTEND(client) (":localhost 323 " + client + " :End of /LIST\r\n")

// (324) RPL_CHANNELMODEIS : チャネルモード（モード文字列のみ）
#define RPL_CHANNELMODEIS(client, channel, modes) (":localhost 324 " + client + " :" + channel + " modes: " + modes + "\r\n")

// // (329) RPL_CREATIONTIME : チャネル作成時刻
// #define RPL_CREATIONTIME(client, channel, ts) (":localhost 329 " + client + " :" + channel + " created at " + ts + "\r\n")

// // (330) RPL_WHOISACCOUNT : アカウント名
// #define RPL_WHOISACCOUNT(client, nick, account) (":localhost 330 " + client + " :" + nick + " is logged in as " + account + "\r\n")

// (331) RPL_NOTOPIC : トピック未設定
#define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " :" + channel + " has no topic set\r\n")

// (332) RPL_TOPIC : チャネルトピック
#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " :" + channel + " topic: " + topic + "\r\n")

// // (333) RPL_TOPICWHOTIME : トピック設定者と時刻
// #define RPL_TOPICWHOTIME(client, channel, nick, ts) (":localhost 333 " + client + " :" + channel + " topic set by " + nick + " at " + ts + "\r\n")

// // (336) RPL_INVITELIST : 招待されたチャネル
// #define RPL_INVITELIST(client, channel) (":localhost 336 " + client + " :Invite list: " + channel + "\r\n")

// // (337) RPL_ENDOFINVITELIST : 招待リスト終了
// #define RPL_ENDOFINVITELIST(client) (":localhost 337 " + client + " :End of invite list\r\n")

// // (338) RPL_WHOISACTUALLY : 実際の接続情報
// #define RPL_WHOISACTUALLY(client, nick, info) (":localhost 338 " + client + " :" + nick + " " + info + "\r\n")

// // (341) RPL_INVITING : 招待成功通知
// #define RPL_INVITING(client, nick, channel) (":localhost 341 " + client + " :Inviting " + nick + " to " + channel + "\r\n")

// // (346) RPL_INVEXLIST : 招待例外リスト
// #define RPL_INVEXLIST(client, channel, mask) (":localhost 346 " + client + " :" + channel + " exception: " + mask + "\r\n")

// // (347) RPL_ENDOFINVEXLIST : 招待例外リスト終了
// #define RPL_ENDOFINVEXLIST(client, channel) (":localhost 347 " + client + " :End of invite exception list for " + channel + "\r\n")

// // (348) RPL_EXCEPTLIST : 例外リスト
// #define RPL_EXCEPTLIST(client, channel, mask) (":localhost 348 " + client + " :" + channel + " exception: " + mask + "\r\n")

// // (349) RPL_ENDOFEXCEPTLIST : 例外リスト終了
// #define RPL_ENDOFEXCEPTLIST(client, channel) (":localhost 349 " + client + " :End of exception list for " + channel + "\r\n")

// // (351) RPL_VERSION : サーババージョン情報
// #define RPL_VERSION(client, version, server, comments) (":localhost 351 " + client + " :" + version + " " + server + " : " + comments + "\r\n")

// // (352) RPL_WHOREPLY : WHO応答
// #define RPL_WHOREPLY(client, channel, user, host, server, nick, flags, hop, real) (":localhost 352 " + client + " :WHO " + channel + " " + user + " " + host + " " + server + " " + nick + " " + flags + " :" + hop + " " + real + "\r\n")

// // (353) RPL_NAMREPLY : NAMES応答
// #define RPL_NAMREPLY(client, symbol, channel, nicks) (":localhost 353 " + client + " :" + symbol + " " + channel + " : " + nicks + "\r\n")

// // (364) RPL_LINKS : サーバリンク情報
// #define RPL_LINKS(client, server, hop, info) (":localhost 364 " + client + " :* " + server + " :" + hop + " " + info + "\r\n")

// // (365) RPL_ENDOFLINKS : LINKS終了
// #define RPL_ENDOFLINKS(client) (":localhost 365 " + client + " :End of /LINKS\r\n")

// // (366) RPL_ENDOFNAMES : NAMES終了
// #define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " :End of /NAMES for " + channel + "\r\n")

// // (367) RPL_BANLIST : BANリスト
// #define RPL_BANLIST(client, channel, mask) (":localhost 367 " + client + " :BAN " + channel + " " + mask + "\r\n")

// // (368) RPL_ENDOFBANLIST : BANリスト終了
// #define RPL_ENDOFBANLIST(client, channel) (":localhost 368 " + client + " :End of ban list for " + channel + "\r\n")

// // (369) RPL_ENDOFWHOWAS : WHOWAS終了
// #define RPL_ENDOFWHOWAS(client, nick) (":localhost 369 " + client + " :End of WHOWAS for " + nick + "\r\n")

// // (371) RPL_INFO : サーバ情報
// #define RPL_INFO(client, info) (":localhost 371 " + client + " :" + info + "\r\n")

// // (372) RPL_MOTD : MOTDの1行
// #define RPL_MOTD(client, line) (":localhost 372 " + client + " :" + line + "\r\n")

// // (374) RPL_ENDOFINFO : INFO終了
// #define RPL_ENDOFINFO(client) (":localhost 374 " + client + " :End of INFO\r\n")

// // (375) RPL_MOTDSTART : MOTD開始
// #define RPL_MOTDSTART(client, server) (":localhost 375 " + client + " :Message of the day from " + server + "\r\n")

// // (376) RPL_ENDOFMOTD : MOTD終了
// #define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of MOTD\r\n")

// // (378) RPL_WHOISHOST : WHOISHOST情報
// #define RPL_WHOISHOST(client, nick, hostinfo) (":localhost 378 " + client + " :" + nick + " is connecting from " + hostinfo + "\r\n")

// // (379) RPL_WHOISMODES : ユーザモード情報
// #define RPL_WHOISMODES(client, nick, modes) (":localhost 379 " + client + " :" + nick + " is using modes " + modes + "\r\n")

// // (381) RPL_YOUREOPER : オペレータ認証成功
// #define RPL_YOUREOPER(client) (":localhost 381 " + client + " :You are now an IRC operator\r\n")

// // (382) RPL_REHASHING : REHASH実行中
// #define RPL_REHASHING(client, config) (":localhost 382 " + client + " :Rehashing " + config + "\r\n")

// // (391) RPL_TIME : サーバ時刻（人間向けのみ）
// #define RPL_TIME(client, server, time_str) (":localhost 391 " + client + " :Time on " + server + " is " + time_str + "\r\n")

// // (400) ERR_UNKNOWNERROR : 一般的エラー
// #define ERR_UNKNOWNERROR(client, command, info) (":localhost 400 " + client + " :Error " + command + " - " + info + "\r\n")

// (401) ERR_NOSUCHNICK : 存在しないニック
#define ERR_NOSUCHNICK(client, nick) (":localhost 401 " + client + " :No such nick/channel: " + nick + "\r\n")

// // (402) ERR_NOSUCHSERVER : 存在しないサーバ
// #define ERR_NOSUCHSERVER(client, server) (":localhost 402 " + client + " :No such server: " + server + "\r\n")

// (403) ERR_NOSUCHCHANNEL : 存在しないチャネル
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " :No such channel: " + channel + "\r\n")

// // (404) ERR_CANNOTSENDTOCHAN : 送信不可
// #define ERR_CANNOTSENDTOCHAN(client, channel) (":localhost 404 " + client + " :Cannot send to channel: " + channel + "\r\n")

// // (405) ERR_TOOMANYCHANNELS : チャネル参加数オーバー
// #define ERR_TOOMANYCHANNELS(client, channel) (":localhost 405 " + client + " :Too many channels joined: " + channel + "\r\n")

// // (406) ERR_WASNOSUCHNICK : WHOWAS情報なし
// #define ERR_WASNOSUCHNICK(client, nick) (":localhost 406 " + client + " :No such nick: " + nick + "\r\n")

// // (409) ERR_NOORIGIN : オリジン未指定
// #define ERR_NOORIGIN(client) (":localhost 409 " + client + " :No origin specified\r\n")

// (411) ERR_NORECIPIENT : 宛先未指定
#define ERR_NORECIPIENT(client, command) (":localhost 411 " + client + " :No recipient given for " + command + "\r\n")

// // (412) ERR_NOTEXTTOSEND : 送信テキストなし
// #define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client + " :No text to send\r\n")

// // (417) ERR_INPUTTOOLONG : 入力行が長すぎる
// #define ERR_INPUTTOOLONG(client) (":localhost 417 " + client + " :Input line too long\r\n")

// // (421) ERR_UNKNOWNCOMMAND : 未知のコマンド
// #define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " :Unknown command: " + command + "\r\n")

// // (422) ERR_NOMOTD : MOTD未設定
// #define ERR_NOMOTD(client) (":localhost 422 " + client + " :MOTD File is missing\r\n")

// // (431) ERR_NONICKNAMEGIVEN : ニックネーム未指定
// #define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :No nickname given\r\n")

// (432) ERR_ERRONEUSNICKNAME : 不正なニックネーム
#define ERR_ERRONEUSNICKNAME(client, nick) (":localhost 432 " + client + " :Erroneous nickname: " + nick + "\r\n")

// (433) ERR_NICKNAMEINUSE : ニックネーム使用中
#define ERR_NICKNAMEINUSE(client, nick) (":localhost 433 " + client + " :Nickname is already in use: " + nick + "\r\n")
#define ERR_USERNAMEINUSE(client, user) (":localhost 433 " + client + " :Nickname is already in use: " + nick + "\r\n")

// // (436) ERR_NICKCOLLISION : ニックネーム衝突
// #define ERR_NICKCOLLISION(client, nick) (":localhost 436 " + client + " :Nickname collision for " + nick + "\r\n")

// (441) ERR_USERNOTINCHANNEL : チャネル未参加
#define ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 441 " + client + " :User " + nick + " is not in channel " + channel + "\r\n")

// (442) ERR_NOTONCHANNEL : チャネル未参加（自分）
#define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " :You're not on channel: " + channel + "\r\n")

// (443) ERR_USERONCHANNEL : 既にチャネル参加中
#define ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " :User " + nick + " is already on channel " + channel + "\r\n")

// // (451) ERR_NOTREGISTERED : 未登録
// #define ERR_NOTREGISTERED(client) (":localhost 451 " + client + " :You have not registered\r\n")

// (461) ERR_NEEDMOREPARAMS : パラメータ不足
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " :Not enough parameters for " + command + "\r\n")

// (462) ERR_ALREADYREGISTERED : 再登録不可
#define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister\r\n")

// (464) ERR_PASSWDMISMATCH : パスワード不正
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")

// // (465) ERR_YOUREBANNEDCREEP : 接続拒否（BAN）
// #define ERR_YOUREBANNEDCREEP(client) (":localhost 465 " + client + " :You are banned\r\n")

// (471) ERR_CHANNELISFULL : チャネル満員
#define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " :Channel is full: " + channel + "\r\n")

// (472) ERR_UNKNOWNMODE : 不明なモード文字
#define ERR_UNKNOWNMODE(client, mode) (":localhost 472 " + client + " :Unknown mode char: " + mode + "\r\n")

// (473) ERR_INVITEONLYCHAN : 招待制チャネル
#define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " :Invite only channel: " + channel + "\r\n")

// // (474) ERR_BANNEDFROMCHAN : BAN状態
// #define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " :Banned from channel: " + channel + "\r\n")

// (475) ERR_BADCHANNELKEY : チャネルキー不正
#define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " :Bad channel key for " + channel + "\r\n")

// // (476) ERR_BADCHANMASK : 不正なチャネル名
// #define ERR_BADCHANMASK(client, channel) (":localhost 476 " + client + " :Bad channel mask: " + channel + "\r\n")

// // (481) ERR_NOPRIVILEGES : 権限不足
// #define ERR_NOPRIVILEGES(client) (":localhost 481 " + client + " :Permission Denied\r\n")

// (482) ERR_CHANOPRIVSNEEDED : チャンネル権限不足
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " :Not channel operator on " + channel + "\r\n")

// // (483) ERR_CANTKILLSERVER : サーバ殺害不可
// #define ERR_CANTKILLSERVER(client) (":localhost 483 " + client + " :Cannot kill server\r\n")

// // (491) ERR_NOOPERHOST : オペレータ接続不可
// #define ERR_NOOPERHOST(client) (":localhost 491 " + client + " :No oper host\r\n")

// // (501) ERR_UMODEUNKNOWNFLAG : 不明なユーザモード
// #define ERR_UMODEUNKNOWNFLAG(client) (":localhost 501 " + client + " :Unknown mode flag\r\n")

// // (502) ERR_USERSDONTMATCH : 他ユーザのモード変更不可
// #define ERR_USERSDONTMATCH(client) (":localhost 502 " + client + " :Cannot change mode for others\r\n")

// // (524) ERR_HELPNOTFOUND : ヘルプ情報なし
// #define ERR_HELPNOTFOUND(client, subject) (":localhost 524 " + client + " :No help available for " + subject + "\r\n")

// // (525) ERR_INVALIDKEY : キーの形式不正
// #define ERR_INVALIDKEY(client, chan) (":localhost 525 " + client + " :Key not well-formed for " + chan + "\r\n")

// // (670) RPL_STARTTLS : TLS開始成功
// #define RPL_STARTTLS(client) (":localhost 670 " + client + " :STARTTLS successful\r\n")

// // (671) RPL_WHOISSECURE : 安全な接続状態
// #define RPL_WHOISSECURE(client, nick) (":localhost 671 " + client + " :Secure connection for " + nick + "\r\n")

// // (691) ERR_STARTTLS : TLS開始失敗
// #define ERR_STARTTLS(client) (":localhost 691 " + client + " :STARTTLS failed\r\n")

// // (696) ERR_INVALIDMODEPARAM : モードパラメータ不正
// #define ERR_INVALIDMODEPARAM(client, target, mode, param, desc) (":localhost 696 " + client + " :For " + target + " mode " + mode + " " + param + " error: " + desc + "\r\n")

// // (704) RPL_HELPSTART : HELP開始
// #define RPL_HELPSTART(client, subject, title) (":localhost 704 " + client + " :HELP " + subject + " - " + title + "\r\n")

// // (705) RPL_HELPTXT : HELPテキスト
// #define RPL_HELPTXT(client, subject, line) (":localhost 705 " + client + " :HELP " + subject + " " + line + "\r\n")

// // (706) RPL_ENDOFHELP : HELP終了
// #define RPL_ENDOFHELP(client, subject, last) (":localhost 706 " + client + " :End of HELP for " + subject + ": " + last + "\r\n")

// // (723) ERR_NOPRIVS : オペレータ権限不足（priv指定）
// #define ERR_NOPRIVS(client, priv) (":localhost 723 " + client + " :Insufficient oper privileges: " + priv + "\r\n")

// // (900) RPL_LOGGEDIN : ログイン成功（アカウント名とユーザ名）
// #define RPL_LOGGEDIN(client, account, username) (":localhost 900 " + client + " :Logged in as " + account + " (" + username + ")\r\n")

// // (901) RPL_LOGGEDOUT : ログアウト成功
// #define RPL_LOGGEDOUT(client) (":localhost 901 " + client + " :Logged out\r\n")

// // (902) ERR_NICKLOCKED : ロックされたニック
// #define ERR_NICKLOCKED(client) (":localhost 902 " + client + " :Nick locked\r\n")

// // (903) RPL_SASLSUCCESS : SASL認証成功
// #define RPL_SASLSUCCESS(client) (":localhost 903 " + client + " :SASL authentication successful\r\n")

// // (904) ERR_SASLFAIL : SASL認証失敗
// #define ERR_SASLFAIL(client) (":localhost 904 " + client + " :SASL authentication failed\r\n")

// // (905) ERR_SASLTOOLONG : SASLメッセージ長過ぎ
// #define ERR_SASLTOOLONG(client) (":localhost 905 " + client + " :SASL message too long\r\n")

// // (906) ERR_SASLABORTED : SASL中断
// #define ERR_SASLABORTED(client) (":localhost 906 " + client + " :SASL authentication aborted\r\n")

// // (907) ERR_SASLALREADY : 既にSASL済み
// #define ERR_SASLALREADY(client) (":localhost 907 " + client + " :Already authenticated via SASL\r\n")

// // (908) RPL_SASLMECHS : 利用可能SASLメカニズム一覧
// #define RPL_SASLMECHS(client, mechs) (":localhost 908 " + client + " :SASL mechanisms: " + mechs + "\r\n")

#endif