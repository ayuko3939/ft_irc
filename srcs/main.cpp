/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:07:05 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/17 15:48:30 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

# include <iostream>

// bool g_ServerShutdown = false;

// static void signalHandler(int signal)
// {
// 	(void)signal;
// 	g_ServerShutdown = true;
// }

// void signalHandler(int signal)
// {
// 	(void)signal;
// 	std::cout << INDIGO "[Server] Signal received. " END << std::endl;
// }

int main(int ac, char **av)
{
	// 引数チェック（ircserv + ポート番号 + パスワード）
	if (ac != 3)
	{
		std::cout << RED "Usage: ./ircserv <port> <password>" END << std::endl;
		return (EXIT_FAILURE);
	}

	time_t rawtime;
	struct tm *timeinfo;

	// 現在時刻の取得し、ローカル時間に変換
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	// シグナルハンドラの設定（Ctrl + C , Ctrl + \ , killコマンド , pipe切断）
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	signal(SIGTERM, Server::signalHandler);
	signal(SIGPIPE, SIG_IGN);
	
	// サーバーの初期化
	Server ircServer(av[1], av[2], timeinfo);

	try
	{
		// オペレーターリストを読み込む
		ircServer.readConfigFile();
		// サーバーの情報を取得
		ircServer.getServerInfo();
		// サーバーを起動
		ircServer.launchServer();
		// サーバーのメインループ
		ircServer.manageServerLoop();
	}
	catch(char const*mes)
	{
		std::cerr << RED << mes << END << std::endl;
		return (EXIT_FAILURE);
	}
	catch(const std::string &mes)
	{
		std::cerr << RED << mes << END << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
