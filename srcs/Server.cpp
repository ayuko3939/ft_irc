/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/03/15 13:47:30 by yohasega         ###   ########.fr       */
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
std::string &Server::getPassword() { return (_password); }
std::string &Server::getDateTime() { return (_dateTime); }
// std::vector<serverOperator> &Server::getOperatorList() { return (_operatorList); }
std::map<const int, Client>& Server::getClientList() { return (_clientList); }
std::map<std::string, Channel>& Server::getChannelList() { return (_channelList); }
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
		while (iss >> str)
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
/*                       Client Management                         */
/*******************************************************************/

void Server::addClient(int clientFd, std::vector<pollfd> &tmpPollFds)
{
	pollfd	clientPollFd;
	Client	newClient(clientFd);

	// クライアントが送受信できるように設定し、tmpPollFdsに格納
	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN | POLLOUT; // 送受信
	tmpPollFds.push_back(clientPollFd);

	// クライアントリストに追加
	_clientList.insert(std::pair<int, Client>(clientFd, newClient));
	std::cout << INDIGO SERVER_ADD_CLIENT << clientFd << END << std::endl;
}

void Server::deleteClient(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	std::cout << INDIGO SERVER_DISCONNECT_CLIENT << clientFd << END << std::endl;
	
	// クライアントリストから削除し、ソケットを閉じる
	_clientList.erase(clientFd);
	close(clientFd);
	pollFds.erase(it);
	
	std::cout << INDIGO SERVER_NUMBER_OF_CLIENTS << pollFds.size() - 1 << END << std::endl;
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
	if (it->second.isClientInChannel(client.getClientFd()))
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
