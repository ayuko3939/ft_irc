/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:07:12 by ohasega           #+#    #+#             */
/*   Updated: 2025/03/13 16:32:36 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCNUMERICS_HPP
# define IRCNUMERICS_HPP

// エラーメッセージ
# define ERR_NOT_ENOUGH_ARGS	"Error: not enough arguments. Usage: "
# define ERR_TOO_MANY_ARGS	"Error: too many arguments. Usage: "

// コマンド要件
# define NICK_USAGE			"NICK <nickname>\r\n"
# define NICK_REQUIREMENTS	"[!] nickname requirements: len(1-10), char(a-z, A-Z, 0-9)\r\n"
# define USER_USAGE			"USER <username> <realname>\r\n"
# define USER_REQUIREMENTS	"[!] username requirements: len(1-10), char(a-z, A-Z, 0-9)\r\n    realname requirements: len(1-10), char(a-z, A-Z)\r\n"

// 成功通知
# define RPL_NICK(client, newNick) (":" + client + " :Your nickname is " + newNick + "\r\n")
# define RPL_USER(client, user, real) (":" + client + " :Your username is " + user + ", realname is " + real + "\r\n")

// (001) RPL_WELCOME : クライアント登録直後の歓迎メッセージ
# define RPL_WELCOME(client, nick) (":" + client + " :Welcome to the Internet Relay Network, " + nick + "\r\n")
// (002) RPL_YOURHOST : 接続中のサーバ名とバージョン情報
# define RPL_YOURHOST(client, server, version) (":" + client + " :Your host is " + server + ", running version " + version + "\r\n")
// (003) RPL_CREATED : サーバ生成日時の通知
# define RPL_CREATED(client, datetime) (":" + client + " :This server was created " + datetime + "\r\n")
// (004) RPL_MYINFO : サーバ名とバージョン情報のみ
# define RPL_MYINFO(client, server, version) (":" + client + " " + server + " " + version + "\r\n")
// (005) RPL_ISUPPORT : サポートトークンの通知
# define RPL_ISUPPORT(client, tokens) (":" + client + " " + tokens + " :are supported\r\n")

// // (010) RPL_BOUNCE : 別サーバへのリダイレクト
// # define RPL_BOUNCE(client, host, port) (":" + client + " " + host + " " + port + "\r\n")

// // (212) RPL_STATSCOMMANDS : コマンド使用回数（簡素版：countのみ）
// # define RPL_STATSCOMMANDS(client, command, count) (":" + client + " " + command + " " + count + "\r\n")

// // (219) RPL_ENDOFSTATS : STATS終了通知
// # define RPL_ENDOFSTATS(client, letter) (":" + client + " " + letter + " :End of /STATS\r\n")

// // (221) RPL_UMODEIS : ユーザモード情報
// # define RPL_UMODEIS(client, modes) (":" + client + " " + modes + "\r\n")

// // (242) RPL_STATSUPTIME : サーバ稼働時間（1つの文字列）
// # define RPL_STATSUPTIME(client, uptime) (":" + client + " :Server Up " + uptime + "\r\n")

// // (251) RPL_LUSERCLIENT : ユーザ数・サーバ数（簡素化：ユーザ数とサーバ数）
// # define RPL_LUSERCLIENT(client, users, servers) (":" + client + " :There are " + users + " users on " + servers + " servers\r\n")

// // (252) RPL_LUSEROP : オペレータ数
// # define RPL_LUSEROP(client, ops) (":" + client + " " + ops + " :operators online\r\n")

// // (253) RPL_LUSERUNKNOWN : 未確認接続数
// # define RPL_LUSERUNKNOWN(client, unknown) (":" + client + " " + unknown + " :unknown connection(s)\r\n")

// // (254) RPL_LUSERCHANNELS : チャネル数
// # define RPL_LUSERCHANNELS(client, channels) (":" + client + " " + channels + " :channels formed\r\n")

// // (255) RPL_LUSERME : 自サーバのクライアント数とサーバ数（例示通り）
// # define RPL_LUSERME(clients, servers) (":I have " + clients + " clients and " + servers + " servers\r\n")

// // (256) RPL_ADMINME : 管理情報（infoのみ）
// # define RPL_ADMINME(client, info) (":" + client + " :" + info + "\r\n")

// // (257) RPL_ADMINLOC1 : サーバ所在地情報
// # define RPL_ADMINLOC1(client, info) (":" + client + " :" + info + "\r\n")

// // (258) RPL_ADMINLOC2 : 運営者情報
// # define RPL_ADMINLOC2(client, info) (":" + client + " :" + info + "\r\n")

// // (259) RPL_ADMINEMAIL : 管理者メールアドレス
// # define RPL_ADMINEMAIL(client, email) (":" + client + " :" + email + "\r\n")

// // (263) RPL_TRYAGAIN : 再試行促し
// # define RPL_TRYAGAIN(client, command) (":" + client + " " + command + " :Please try again later\r\n")

// // (265) RPL_LOCALUSERS : 直接接続ユーザ数と最大値
// # define RPL_LOCALUSERS(client, cur, max) (":" + client + " " + cur + " " + max + " :Local users\r\n")

// // (266) RPL_GLOBALUSERS : 全体接続ユーザ数と最大値
// # define RPL_GLOBALUSERS(client, cur, max) (":" + client + " " + cur + " " + max + " :Global users\r\n")

// // (276) RPL_WHOISCERTFP : 証明書フィンガープリント
// # define RPL_WHOISCERTFP(client, nick, fp) (":" + client + " " + nick + " :fp " + fp + "\r\n")

// // (300) RPL_NONE : ダミーメッセージ
// # define RPL_NONE(client) (":" + client + "\r\n")

// // (301) RPL_AWAY : away状態通知
// # define RPL_AWAY(client, nick, msg) (":" + client + " " + nick + " :" + msg + "\r\n")

// // (302) RPL_USERHOST : ユーザホスト情報
// # define RPL_USERHOST(client, reply) (":" + client + " :" + reply + "\r\n")

// // (305) RPL_UNAWAY : away解除通知
// # define RPL_UNAWAY(client) (":" + client + " :You are no longer away\r\n")

// // (306) RPL_NOWAWAY : away設定通知
// # define RPL_NOWAWAY(client) (":" + client + " :You are now away\r\n")

// // (307) RPL_WHOISREGNICK : 認証済みニック通知
// # define RPL_WHOISREGNICK(client, nick) (":" + client + " " + nick + " :identified\r\n")

// // (311) RPL_WHOISUSER : WHOIS基本情報
// # define RPL_WHOISUSER(client, nick, user, host, realname) (":" + client + " " + nick + " " + user + " " + host + " * :" + realname + "\r\n")

// // (312) RPL_WHOISSERVER : 接続サーバ名
// # define RPL_WHOISSERVER(client, nick, server) (":" + client + " " + nick + " " + server + "\r\n")

// // (313) RPL_WHOISOPERATOR : オペレータ通知
// # define RPL_WHOISOPERATOR(client, nick) (":" + client + " " + nick + " :is an operator\r\n")

// // (314) RPL_WHOWASUSER : WHOWAS情報
// # define RPL_WHOWASUSER(client, nick, user, host, realname) (":" + client + " " + nick + " " + user + " " + host + " * :" + realname + "\r\n")

// // (315) RPL_ENDOFWHO : WHO終了
// # define RPL_ENDOFWHO(client, mask) (":" + client + " " + mask + " :End of WHO\r\n")

// // (317) RPL_WHOISIDLE : アイデル時間
// # define RPL_WHOISIDLE(client, nick, secs, signon) (":" + client + " " + nick + " " + secs + " " + signon + "\r\n")

// // (318) RPL_ENDOFWHOIS : WHOIS終了
// # define RPL_ENDOFWHOIS(client, nick) (":" + client + " " + nick + " :End of WHOIS\r\n")

// // (319) RPL_WHOISCHANNELS : 参加チャネル一覧
// # define RPL_WHOISCHANNELS(client, nick, chans) (":" + client + " " + nick + " :" + chans + "\r\n")

// // (320) RPL_WHOISSPECIAL : 補足情報
// # define RPL_WHOISSPECIAL(client, nick, info) (":" + client + " " + nick + " :" + info + "\r\n")

// // (321) RPL_LISTSTART : チャネルリスト開始
// # define RPL_LISTSTART(client) (":" + client + " Channel :Users  Name\r\n")

// // (322) RPL_LIST : チャネル情報
// # define RPL_LIST(client, channel, count, topic) (":" + client + " " + channel + " " + count + " :" + topic + "\r\n")

// // (323) RPL_LISTEND : チャネルリスト終了
// # define RPL_LISTEND(client) (":" + client + " :End of /LIST\r\n")

// // (324) RPL_CHANNELMODEIS : チャネルモード（モード文字列のみ）
// # define RPL_CHANNELMODEIS(client, channel, modes) (":" + client + " " + channel + " " + modes + "\r\n")

// // (329) RPL_CREATIONTIME : チャネル作成時刻
// # define RPL_CREATIONTIME(client, channel, ts) (":" + client + " " + channel + " " + ts + "\r\n")

// // (330) RPL_WHOISACCOUNT : アカウント名
// # define RPL_WHOISACCOUNT(client, nick, account) (":" + client + " " + nick + " " + account + "\r\n")

// // (331) RPL_NOTOPIC : トピック未設定
// # define RPL_NOTOPIC(client, channel) (":" + client + " " + channel + " :No topic is set\r\n")

// // (332) RPL_TOPIC : チャネルトピック
// # define RPL_TOPIC(client, channel, topic) (":" + client + " " + channel + " :" + topic + "\r\n")

// // (333) RPL_TOPICWHOTIME : トピック設定者と時刻
// # define RPL_TOPICWHOTIME(client, channel, nick, ts) (":" + client + " " + channel + " " + nick + " " + ts + "\r\n")

// // (336) RPL_INVITELIST : 招待されたチャネル
// # define RPL_INVITELIST(client, channel) (":" + client + " " + channel + "\r\n")

// // (337) RPL_ENDOFINVITELIST : 招待リスト終了
// # define RPL_ENDOFINVITELIST(client) (":" + client + " :End of invite list\r\n")

// // (338) RPL_WHOISACTUALLY : 実際の接続情報
// # define RPL_WHOISACTUALLY(client, nick, info) (":" + client + " " + nick + " " + info + "\r\n")

// // (341) RPL_INVITING : 招待成功通知
// # define RPL_INVITING(client, nick, channel) (":" + client + " " + nick + " " + channel + "\r\n")

// // (346) RPL_INVEXLIST : 招待例外リスト
// # define RPL_INVEXLIST(client, channel, mask) (":" + client + " " + channel + " " + mask + "\r\n")

// // (347) RPL_ENDOFINVEXLIST : 招待例外リスト終了
// # define RPL_ENDOFINVEXLIST(client, channel) (":" + client + " " + channel + " :End of invite exception list\r\n")

// // (348) RPL_EXCEPTLIST : 例外リスト
// # define RPL_EXCEPTLIST(client, channel, mask) (":" + client + " " + channel + " " + mask + "\r\n")

// // (349) RPL_ENDOFEXCEPTLIST : 例外リスト終了
// # define RPL_ENDOFEXCEPTLIST(client, channel) (":" + client + " " + channel + " :End of exception list\r\n")

// // (351) RPL_VERSION : サーババージョン情報
// # define RPL_VERSION(client, version, server, comments) (":" + client + " " + version + " " + server + " :" + comments + "\r\n")

// // (352) RPL_WHOREPLY : WHO応答
// # define RPL_WHOREPLY(client, channel, user, host, server, nick, flags, hop, real) (":" + client + " " + channel + " " + user + " " + host + " " + server + " " + nick + " " + flags + " :" + hop + " " + real + "\r\n")

// // (353) RPL_NAMREPLY : NAMES応答
// # define RPL_NAMREPLY(client, symbol, channel, nicks) (":" + client + " " + symbol + " " + channel + " :" + nicks + "\r\n")

// // (364) RPL_LINKS : サーバリンク情報
// # define RPL_LINKS(client, server, hop, info) (":" + client + " * " + server + " :" + hop + " " + info + "\r\n")

// // (365) RPL_ENDOFLINKS : LINKS終了
// # define RPL_ENDOFLINKS(client) (":" + client + " * :End of /LINKS\r\n")

// // (366) RPL_ENDOFNAMES : NAMES終了
// # define RPL_ENDOFNAMES(client, channel) (":" + client + " " + channel + " :End of /NAMES\r\n")

// // (367) RPL_BANLIST : BANリスト
// # define RPL_BANLIST(client, channel, mask) (":" + client + " " + channel + " " + mask + "\r\n")

// // (368) RPL_ENDOFBANLIST : BANリスト終了
// # define RPL_ENDOFBANLIST(client, channel) (":" + client + " " + channel + " :End of ban list\r\n")

// // (369) RPL_ENDOFWHOWAS : WHOWAS終了
// # define RPL_ENDOFWHOWAS(client, nick) (":" + client + " " + nick + " :End of WHOWAS\r\n")

// // (371) RPL_INFO : サーバ情報
// # define RPL_INFO(client, info) (":" + client + " :" + info + "\r\n")

// // (372) RPL_MOTD : MOTDの1行
// # define RPL_MOTD(client, line) (":" + client + " :" + line + "\r\n")

// // (374) RPL_ENDOFINFO : INFO終了
// # define RPL_ENDOFINFO(client) (":" + client + " :End of INFO\r\n")

// // (375) RPL_MOTDSTART : MOTD開始
// # define RPL_MOTDSTART(client, server) (":" + client + " :- " + server + " MOTD -\r\n")

// // (376) RPL_ENDOFMOTD : MOTD終了
// # define RPL_ENDOFMOTD(client) (":" + client + " :End of MOTD\r\n")

// // (378) RPL_WHOISHOST : WHOISHOST情報
// # define RPL_WHOISHOST(client, nick, hostinfo) (":" + client + " " + nick + " :" + hostinfo + "\r\n")

// // (379) RPL_WHOISMODES : ユーザモード情報
// # define RPL_WHOISMODES(client, nick, modes) (":" + client + " " + nick + " :modes " + modes + "\r\n")

// // (381) RPL_YOUREOPER : オペレータ認証成功
// # define RPL_YOUREOPER(client) (":" + client + " :You are now an operator\r\n")

// // (382) RPL_REHASHING : REHASH実行中
// # define RPL_REHASHING(client, config) (":" + client + " " + config + " :Rehashing\r\n")

// // (391) RPL_TIME : サーバ時刻（人間向けのみ）
// # define RPL_TIME(client, server, time_str) (":" + client + " " + server + " :" + time_str + "\r\n")

// // (400) ERR_UNKNOWNERROR : 一般的エラー
// # define ERR_UNKNOWNERROR(client, command, info) (":" + client + " " + command + " :" + info + "\r\n")

// // (401) ERR_NOSUCHNICK : 存在しないニック
// # define ERR_NOSUCHNICK(client, nick) (":" + client + " " + nick + " :No such nick/channel\r\n")

// // (402) ERR_NOSUCHSERVER : 存在しないサーバ
// # define ERR_NOSUCHSERVER(client, server) (":" + client + " " + server + " :No such server\r\n")

// // (403) ERR_NOSUCHCHANNEL : 存在しないチャネル
// # define ERR_NOSUCHCHANNEL(client, channel) (":" + client + " " + channel + " :No such channel\r\n")

// // (404) ERR_CANNOTSENDTOCHAN : 送信不可
// # define ERR_CANNOTSENDTOCHAN(client, channel) (":" + client + " " + channel + " :Cannot send to channel\r\n")

// // (405) ERR_TOOMANYCHANNELS : チャネル参加数オーバー
// # define ERR_TOOMANYCHANNELS(client, channel) (":" + client + " " + channel + " :Too many channels joined\r\n")

// // (406) ERR_WASNOSUCHNICK : WHOWAS情報なし
// # define ERR_WASNOSUCHNICK(client, nick) (":" + client + " " + nick + " :No such nick\r\n")

// // (409) ERR_NOORIGIN : オリジン未指定
// # define ERR_NOORIGIN(client) (":" + client + " :No origin specified\r\n")

// // (411) ERR_NORECIPIENT : 宛先未指定
// # define ERR_NORECIPIENT(client, command) (":" + client + " :No recipient given (" + command + ")\r\n")

// // (412) ERR_NOTEXTTOSEND : 送信テキストなし
// # define ERR_NOTEXTTOSEND(client) (":" + client + " :No text to send\r\n")

// // (417) ERR_INPUTTOOLONG : 入力行が長すぎる
// # define ERR_INPUTTOOLONG(client) (":" + client + " :Input line too long\r\n")

// // (421) ERR_UNKNOWNCOMMAND : 未知のコマンド
// # define ERR_UNKNOWNCOMMAND(client, command) (":" + client + " " + command + " :Unknown command\r\n")

// // (422) ERR_NOMOTD : MOTD未設定
// # define ERR_NOMOTD(client) (":" + client + " :MOTD File is missing\r\n")

// // (431) ERR_NONICKNAMEGIVEN : ニックネーム未指定
// # define ERR_NONICKNAMEGIVEN(client) (":" + client + " :No nickname given\r\n")

// (432) ERR_ERRONEUSNICKNAME : 不正なニックネーム
# define ERR_ERRONEUSNICKNAME(client, nick) (":" + client + " " + nick + " :Erroneous nickname\r\n")

// (433) ERR_NICKNAMEINUSE : ニックネーム使用中
# define ERR_NICKNAMEINUSE(client, nick) (":" + client + " " + nick + " :Nickname is already in use\r\n")
# define ERR_USERNAMEINUSE(client, user) (":" + client + " " + user + " :Username is already in use\r\n")

// // (436) ERR_NICKCOLLISION : ニックネーム衝突
// # define ERR_NICKCOLLISION(client, nick) (":" + client + " " + nick + " :Nickname collision\r\n")

// // (441) ERR_USERNOTINCHANNEL : チャネル未参加
// # define ERR_USERNOTINCHANNEL(client, nick, channel) (":" + client + " " + nick + " " + channel + " :Not on that channel\r\n")

// // (442) ERR_NOTONCHANNEL : チャネル未参加（自分）
// # define ERR_NOTONCHANNEL(client, channel) (":" + client + " " + channel + " :You're not on that channel\r\n")

// // (443) ERR_USERONCHANNEL : 既にチャネル参加中
// # define ERR_USERONCHANNEL(client, nick, channel) (":" + client + " " + nick + " " + channel + " :Already on channel\r\n")

// // (451) ERR_NOTREGISTERED : 未登録
// # define ERR_NOTREGISTERED(client) (":" + client + " :You have not registered\r\n")

// // (461) ERR_NEEDMOREPARAMS : パラメータ不足
// # define ERR_NEEDMOREPARAMS(client, command) (":" + client + " " + command + " :Not enough parameters\r\n")

// (462) ERR_ALREADYREGISTERED : 再登録不可
# define ERR_ALREADYREGISTERED(client) (":" + client + " :You may not reregister\r\n")

// // (464) ERR_PASSWDMISMATCH : パスワード不正
// # define ERR_PASSWDMISMATCH(client) (":" + client + " :Password incorrect\r\n")

// // (465) ERR_YOUREBANNEDCREEP : 接続拒否（BAN）
// # define ERR_YOUREBANNEDCREEP(client) (":" + client + " :You are banned\r\n")

// // (471) ERR_CHANNELISFULL : チャネル満員
// # define ERR_CHANNELISFULL(client, channel) (":" + client + " " + channel + " :Channel is full\r\n")

// // (472) ERR_UNKNOWNMODE : 不明なモード文字
// # define ERR_UNKNOWNMODE(client, mode) (":" + client + " " + mode + " :Unknown mode char\r\n")

// // (473) ERR_INVITEONLYCHAN : 招待制チャネル
// # define ERR_INVITEONLYCHAN(client, channel) (":" + client + " " + channel + " :Invite only\r\n")

// // (474) ERR_BANNEDFROMCHAN : BAN状態
// # define ERR_BANNEDFROMCHAN(client, channel) (":" + client + " " + channel + " :Banned from channel\r\n")

// // (475) ERR_BADCHANNELKEY : チャネルキー不正
// # define ERR_BADCHANNELKEY(client, channel) (":" + client + " " + channel + " :Bad channel key\r\n")

// // (476) ERR_BADCHANMASK : 不正なチャネル名
// # define ERR_BADCHANMASK(channel) (":" + channel + " :Bad channel mask\r\n")

// // (481) ERR_NOPRIVILEGES : 権限不足
// # define ERR_NOPRIVILEGES(client) (":" + client + " :Permission Denied\r\n")

// // (482) ERR_CHANOPRIVSNEEDED : チャンネル権限不足
// # define ERR_CHANOPRIVSNEEDED(client, channel) (":" + client + " " + channel + " :Not channel operator\r\n")

// // (483) ERR_CANTKILLSERVER : サーバ殺害不可
// # define ERR_CANTKILLSERVER(client) (":" + client + " :Cannot kill server\r\n")

// // (491) ERR_NOOPERHOST : オペレータ接続不可
// # define ERR_NOOPERHOST(client) (":" + client + " :No oper host\r\n")

// // (501) ERR_UMODEUNKNOWNFLAG : 不明なユーザモード
// # define ERR_UMODEUNKNOWNFLAG(client) (":" + client + " :Unknown mode flag\r\n")

// // (502) ERR_USERSDONTMATCH : 他ユーザのモード変更不可
// # define ERR_USERSDONTMATCH(client) (":" + client + " :Cannot change mode for others\r\n")

// // (524) ERR_HELPNOTFOUND : ヘルプ情報なし
// # define ERR_HELPNOTFOUND(client, subject) (":" + client + " " + subject + " :No help available\r\n")

// // (525) ERR_INVALIDKEY : キーの形式不正
// # define ERR_INVALIDKEY(client, chan) (":" + client + " " + chan + " :Key not well-formed\r\n")

// // (670) RPL_STARTTLS : TLS開始成功
// # define RPL_STARTTLS(client) (":" + client + " :STARTTLS successful\r\n")

// // (671) RPL_WHOISSECURE : 安全な接続状態
// # define RPL_WHOISSECURE(client, nick) (":" + client + " " + nick + " :secure connection\r\n")

// // (691) ERR_STARTTLS : TLS開始失敗
// # define ERR_STARTTLS(client) (":" + client + " :STARTTLS failed\r\n")

// // (696) ERR_INVALIDMODEPARAM : モードパラメータ不正
// # define ERR_INVALIDMODEPARAM(client, target, mode, param, desc) (":" + client + " " + target + " " + mode + " " + param + " :" + desc + "\r\n")

// // (704) RPL_HELPSTART : HELP開始
// # define RPL_HELPSTART(client, subject, title) (":" + client + " " + subject + " :" + title + "\r\n")

// // (705) RPL_HELPTXT : HELPテキスト
// # define RPL_HELPTXT(client, subject, line) (":" + client + " " + subject + " :" + line + "\r\n")

// // (706) RPL_ENDOFHELP : HELP終了
// # define RPL_ENDOFHELP(client, subject, last) (":" + client + " " + subject + " :" + last + "\r\n")

// // (723) ERR_NOPRIVS : オペレータ権限不足（priv指定）
// # define ERR_NOPRIVS(client, priv) (":" + client + " " + priv + " :Insufficient oper privileges\r\n")

// // (900) RPL_LOGGEDIN : ログイン成功（アカウント名とユーザ名）
// # define RPL_LOGGEDIN(client, account, username) (":" + client + " " + account + " :Logged in as " + username + "\r\n")

// // (901) RPL_LOGGEDOUT : ログアウト成功
// # define RPL_LOGGEDOUT(client) (":" + client + " :Logged out\r\n")

// // (902) ERR_NICKLOCKED : ロックされたニック
// # define ERR_NICKLOCKED(client) (":" + client + " :Nick locked\r\n")

// // (903) RPL_SASLSUCCESS : SASL認証成功
// # define RPL_SASLSUCCESS(client) (":" + client + " :SASL success\r\n")

// // (904) ERR_SASLFAIL : SASL認証失敗
// # define ERR_SASLFAIL(client) (":" + client + " :SASL failed\r\n")

// // (905) ERR_SASLTOOLONG : SASLメッセージ長過ぎ
// # define ERR_SASLTOOLONG(client) (":" + client + " :SASL message too long\r\n")

// // (906) ERR_SASLABORTED : SASL中断
// # define ERR_SASLABORTED(client) (":" + client + " :SASL aborted\r\n")

// // (907) ERR_SASLALREADY : 既にSASL済み
// # define ERR_SASLALREADY(client) (":" + client + " :Already authenticated via SASL\r\n")

// // (908) RPL_SASLMECHS : 利用可能SASLメカニズム一覧
// # define RPL_SASLMECHS(client, mechs) (":" + client + " " + mechs + " :SASL mechanisms\r\n")

#endif