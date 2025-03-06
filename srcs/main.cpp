/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 21:07:05 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/05 23:12:24 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Irc.hpp"

# include <iostream>
# include <cstdlib>

// サーバー停止フラグ
bool serverShutdown = false;

static void signalHandler(int signal)
{
	(void)signal;
	serverShutdown = true;
}

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
	
	// Ctrl + C でサーバーを停止
	signal(SIGINT, signalHandler);
	
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
		manageServerLoop();
	}
	catch(char const *mes)
	{
		std::cout << RED << mes << END << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
