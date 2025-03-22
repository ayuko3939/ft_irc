/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:43:54 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/22 18:42:44 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

void Server::manageServerLoop()
{
	std::vector<pollfd>		pollFds;
	std::vector<pollfd>		tmpPollFds;
	int						eventCount;

	// サーバーのソケットを監視対象に追加
	setServerPollFd(pollFds);
	
	// サーバーがシャットダウンされるまでループ
	while (Server::_signal == false)
	{
		tmpPollFds.clear();

		// pollFdsの接続に変化があるまで待機（-1:タイムアウトなし）
		if ((eventCount = poll(pollFds.data(), pollFds.size(), -1)) <= 0 && Server::_signal == false)
			throw ("Error: poll"); // ERROR_POLL
		
		// pollFdsの中身を状態を順番に確認し、処理を行う
		// std::cout << "debug: poll loop" << std::endl;
		std::vector<pollfd>::iterator it = pollFds.begin();
		while (it != pollFds.end())
		{
			// POLLIN: データ読み込み可能（新規接続 or 既存クライアントからのデータ受信）
			if (it->revents & POLLIN)
			{
				// クライアントからの新規接続
				if (it->fd == _serverSockFd)
				{
					if (handleNewConnection(pollFds, tmpPollFds))
						it--;
				}
				// 既存クライアントからのデータ受信
				else
				{
					std::cout << "debug: handleClientData!" << std::endl;
					if (handleClientData(pollFds, it))
						break;
				}
			}
			// POLLOUT: データ書き込み可能（クライアントにデータを送信）
			if (it->revents & POLLOUT)
			{
				// クライアントにデータを送信する
				if (handlePollout(pollFds, it, it->fd))
					break;
			}
			// POLLERR: エラー（クライアントが切断された）
			if (it->revents & POLLERR)
			{
				// エラーが発生した
				if (handlePollerr(pollFds, it))
					break;
			}

			++it;
		}

		// 新しいクライアントがいれば、pollFdsに追加する
		// [!] pollFdsに直接入れるとイテレーターが狂うので、
		// 一旦tmpに入れて後でまとめてpollFdsに格納する
		if (!tmpPollFds.empty())
			pollFds.insert(pollFds.end(), tmpPollFds.begin(), tmpPollFds.end());
	}
}
