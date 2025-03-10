/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/10 21:36:27 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*******************************************************************/
/*                    Constructor & Destructor                     */
/*******************************************************************/

Server::Server(std::string port, std::string password, struct tm *timeinfo) :
_serverSockFd(-1),
_port(port),
_password(password),
_operatorList(),
// _clientList(),
// _channelList(),
_serverInfo(NULL)
{
    std::cout << INDIGO "Server running..." END << std::endl;
    
    // 時間を文字列として取得（2042-4-2 4:2:42）
    char buf[80];
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    _dateTime = std::string(buf);

    // サーバーの初期設定（アドレス情報を設定）
    memset(&_addrInfo, 0, sizeof(_addrInfo));
    _addrInfo.ai_family = AF_INET;       // IPv4
    _addrInfo.ai_socktype = SOCK_STREAM; // TCP
    _addrInfo.ai_flags = AI_PASSIVE;     // ローカルホストで動作
}

Server::~Server()
{
}


/*******************************************************************/
/*                             Accessor                            */
/*******************************************************************/

// Setters
// void Server::setServerSockFd(int serverSockFd) { _serverSockFd = serverSockFd; }
// void Server::setPort(const std::string &port) { _port = port; }
// void Server::setPassword(const std::string &password) { _password = password; }
// void Server::setDateTime(const std::string &dateTime) { _dateTime = dateTime; }
// void Server::setOperatorList(std::vector<serverOperator> &operatorList) { _operatorList = operatorList; }
// void Server::setClientList(const std::map<const int, Client> &clientList) { _clientList = clientList; }
// void Server::setChannelList(const std::map<std::string, Channel> &channelList) { _channelList = channelList; }
// void Server::setAddrInfo(const struct addrinfo &addrInfo) { _addrInfo = addrInfo; }
// void Server::setServerInfo(struct addrinfo* serverInfo) { _serverInfo = serverInfo; } 

// Getter
// int Server::getServerSockFd() { return (_serverSockFd); }
// std::string &Server::getPort() { return (_port); }
// std::string &Server::getPassword() { return (_password); }
// std::string &Server::getDateTime() { return (_dateTime); }
// std::vector<serverOperator> &Server::getOperatorList() { return (_operatorList); }
std::map<const int, Client>& Server::getClientList() { return (_clientList); }
// std::map<std::string, Channel>& Server::getChannelList() { return (_channelList); }
// struct addrinfo &Server::getAddrInfo() { return (_addrInfo); }
// struct addrinfo* Server::getServerInfo() { return (_serverInfo); }


/*******************************************************************/
/*                       Server Functions                          */
/*******************************************************************/

void Server::readConfigFile()
{
	// ファイルが開けられるか
	std::ifstream configFile(CONFG_FILE);
	if (!configFile.is_open())
		throw (ERROR_OPEN_FILE);

	std::string		line;

	// １行目はヘッダーなのでスキップ
	std::getline(configFile, line);

	// ２行目以降をオペレーターリストに格納する
    while (std::getline(configFile, line))
	{
        std::istringstream			iss(line);
		std::string					str;
		std::vector<std::string>	elements;
        serverOperator				op;

		// " "区切りで１つずつベクトル格納する、要素が３つなければエラー
		while (!(iss >> str))
			elements.push_back(str);
		
		if (elements.size() != 3)
			throw (ERROR_FILE_CONTENTS);

		// 各要素をオペレーターの情報として格納し、リストに追加する
		op.name = elements[0];
		op.host = elements[1];
		op.password = elements[2];

        _operatorList.push_back(op);
	}

	// もしファイルが空ならエラー
	if (_operatorList.empty())
		throw (ERROR_FILE_EMPTY);

	configFile.close();
}

void Server::getServerInfo()
{
	int status;

	// サーバが利用するネットワークアドレス情報を取得
	if ((status = getaddrinfo(NULL, _port.c_str(), &_addrInfo, &_serverInfo)) != 0)
		throw ("Error: " + std::string(gai_strerror(status)));
}

void Server::launchServer()
{
	// ソケットを作成
	if ((_serverSockFd = socket(_serverInfo->ai_family, _serverInfo->ai_socktype, _serverInfo->ai_protocol)) < 0)
		throw (ERROR_SERVER_SOCKET);
	
	// ソケットオプションを設定
	int socketOpt = 1;
	if (setsockopt(_serverSockFd, SOL_SOCKET, SO_REUSEADDR, &socketOpt, sizeof(socketOpt)) < 0)
		throw (ERROR_SERVER_SETSOCKETOPT);

	// ソケットにアドレスをバインド
	if (bind(_serverSockFd, _serverInfo->ai_addr, _serverInfo->ai_addrlen) < 0)
		throw (ERROR_SERVER_BIND + _port);

	// 待ち受け状態にする
	if (listen(_serverSockFd, 10) < 0)
		throw (ERROR_SERVER_LISTEN);

	// 取得済みのアドレス情報は、ソケットのバインド後は不要となるため、メモリを解放
	freeaddrinfo(_serverInfo);
}


/*******************************************************************/
/*                        Server Main Loop                         */
/*******************************************************************/

int Server::handleClientData(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it)
{
	// 1. クライアント情報の取得
	Client *client = getClient(this, it->fd);
	if (!client)
	{
		std::cerr << ORANGE ERROR_CLIENT_NOT_FOUND << it->fd << END << std::endl;
		return (EXIT_FAILURE);
	}
	
	// 2. 受信用バッファの用意（メッセージの最大長さを指定）
	std::string message;
	message.resize(BUF_SIZE_MSG);

	// 3. recv() を使ってデータ受信
	size_t readSize = recv(it->fd, &message[0], BUF_SIZE_MSG, 0);
	
	// 4-a. エラー発生時の処理
	if (readSize < 0)
	{
		std::cerr << ORANGE ERROR_SERVER_RECV END << std::endl;
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
	// 4-b. クライアントが切断された場合の処理
	else if (readSize == 0)
	{
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
	// 4-c. データ受信成功時の処理
	else
	{
		// 受信したデータをバッファに格納（実際のメッセージの長さにリサイズ）
		message.resize(readSize);
		std::cout << "[Client] " << message << std::endl; // ===== ★後で表示を整える★ =====
		
		// クライアントのバッファに受信したメッセージを追記で格納
		client->setReadBuf(client->getReadBuf() + message);
		
		// 受信したデータに"\r\n"（IRCの改行）が含まれる含む場合、コマンドとして処理する
		if (client->getReadBuf().find("\r\n") != std::string::npos)
		{
			try
			{
				parseMessage(it->fd, client->getReadBuf());

				client->resetReadBuf();
			}
			catch(char const *mes)
			{
				// std::cerr << RED << mes << END << std::endl;

				return (EXIT_FAILURE); // ★★★
			}
		}
	}
	return (EXIT_SUCCESS);
}


int Server::handlePollout(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	Client *client = getClient(this, clientFd);

	// クライアントが見つからなかった場合、エラー文を出力して何もしないで処理終了
	if (!client)
	{
		std::cerr << ORANGE ERROR_CLIENT_NOT_FOUND << clientFd << END << std::endl;
	}
	else
	{
		// クライアントのメッセージを送信して、バッファをクリアする
		sendServerReply(clientFd, client->getSendBuf());
		client->getSendBuf().clear();

		// クライアントが切断フラグを持っている場合は削除してbreak
		if (client->getToDeconnect())
		{
			deleteClient(pollFds, it, clientFd);
			return (1);
		}
	}
	return (EXIT_SUCCESS);
}

int Server::handlePollerr(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it)
{
	std::cerr << ORANGE ERROR_SERVER_POLL << it->fd << END << std::endl;
	
	// サーバーにエラーが発生した場合、強制終了
	if (it->fd == _serverSockFd)
	{
		throw (ERROR_SERVER_LISTEN);
	}
	// クライアントにエラーが発生した場合、クライアントを削除
	{
		deleteClient(pollFds, it, it->fd);
		return (EXIT_FAILURE);
	}
}

void Server::manageServerLoop()
{
	std::vector<pollfd>		pollFds;
	std::vector<pollfd>		tmpPollFds;

	// サーバーのソケットを監視対象に追加
	setServerPollFd(pollFds);
	
	// サーバーがシャットダウンされるまでループ
	while (g_ServerShutdown == false)
	{
		tmpPollFds.clear();

		// pollFdsの接続に変化があるまで待機（-1:タイムアウトなし）
		if (poll((pollfd *)&pollFds[0], pollFds.size(), -1) <= 0)
		{
			// シグナル（Ctrl + C）が発生した場合、ループを抜ける
			if (errno == EINTR)
				break;
			throw ("Error: poll"); // ERROR_POLL
		}
		
		// pollFdsの中身を状態を順番に確認し、処理を行う
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
						continue;
				}
				// 既存クライアントからのデータ受信
				else
				{
					if (handleClientData(pollFds, it))
						break;
				}
			}
			// POLLOUT: データ書き込み可能（クライアントにデータを送信）
			else if (it->revents & POLLOUT)
			{
				// クライアントにデータを送信する
				if (handlePollout(pollFds, it, it->fd))
					break;
			}
			// POLLERR: エラー（クライアントが切断された）
			else if (it->revents & POLLERR)
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

/*******************************************************************/
/*                       Channel Management                        */
/*******************************************************************/

void Server::addChannel(std::string &channelName)
{
	// すでにチャンネルが存在する場合、エラー文を出力して何もしないで処理終了
	std::map<std::string, Channel>::iterator it = _channelList.find(channelName);
	if (it != _channelList.end())
	{
		std::cerr << ORANGE << ERROR_CHANNEL_EXIST << channelName << END << std::endl;
		return ;
	}
	
	// チャンネルを作成し、チャンネルリストに追加
	Channel channel(channelName);
	_channelList.insert(std::make_pair(channelName, channel));
}

void Server::addClientToChannel(std::string &channelName, Client &client)
{
	// チャンネルが存在しない場合、エラー文を出力して何もしないで処理終了
	std::map<std::string, Channel>::iterator it = _channelList.find(channelName);
	if (it == _channelList.end())
	{
		std::cerr << ORANGE ERROR_CHANNEL_NOT_FOUND << channelName << END << std::endl;
		return ;
	}
	
	// クライアントがすでにチャンネルに参加している場合、エラー文を出力して何もしないで処理終了
	if (it->second.isClientInChannel(client.getNickname()))
	{
		std::cerr << ORANGE "Error: Client is already in the channel" END << std::endl;
		return ;
	}

	// チャンネルにクライアントを追加
	it->second.addClientToChannel(client);
}

bool Server::isChannelExist(std::string &channelName)
{
	// チャンネルが存在しない場合、エラー文を出力してfalseを返す
	std::map<std::string, Channel>::iterator it = _channelList.find(channelName);
	if (it == _channelList.end())
	{
		std::cerr << ORANGE ERROR_CHANNEL_NOT_FOUND << channelName << END << std::endl;
		return (false);
	}
	return (true);
}
