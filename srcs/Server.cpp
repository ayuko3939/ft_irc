/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:18 by hasega            #+#    #+#             */
/*   Updated: 2025/04/09 18:05:40 by yohasega         ###   ########.fr       */
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
_clientList(),
_channelList(),
_serverInfo(NULL)
{
    std::cout << INDIGO "[Server] Server running..." END << std::endl;
    
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

// Getter
std::string &Server::getPassword() { return (_password); }
std::string &Server::getDateTime() { return (_dateTime); }
std::map<const int, Client>& Server::getClientList() { return (_clientList); }
std::map<std::string, Channel>& Server::getChannelList() { return (_channelList); }


/*******************************************************************/
/*                       Server Functions                          */
/*******************************************************************/

bool Server::_signal = false;

void Server::signalHandler(int signal)
{
	(void)signal;
	std::cout << INDIGO "[Server] Server shutdown..." END << std::endl;
	Server::_signal = true;
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
	
	// ソケットをノンブロッキングモードに設定
	// レビュー用
	// if (fcntl(_serverSockFd, F_SETFL, O_NONBLOCK) == -1)
	// 	throw (ERROR_SERVER_SETSOCKETOPT);
	// 理想の実装
	int flags = fcntl(_serverSockFd, F_GETFL, 0);
	if (flags == -1)
		throw (ERROR_SERVER_SETSOCKETOPT);
	if (fcntl(_serverSockFd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw (ERROR_SERVER_SETSOCKETOPT);

	// ソケットオプションを設定
	int socketOpt = 1;
	if (setsockopt(_serverSockFd, SOL_SOCKET, SO_REUSEADDR, &socketOpt, sizeof(socketOpt)) < 0)
		throw (ERROR_SERVER_SETSOCKETOPT);

	// ソケットにアドレスをバインド
	if (bind(_serverSockFd, _serverInfo->ai_addr, _serverInfo->ai_addrlen) < 0)
		throw (ERROR_SERVER_BIND);

	// 待ち受け状態にする
	if (listen(_serverSockFd, 10) < 0)
		throw (ERROR_SERVER_LISTEN);

	// 取得済みのアドレス情報は、ソケットのバインド後は不要となるため、メモリを解放
	freeaddrinfo(_serverInfo);
}


/*******************************************************************/
/*                       Client Management                         */
/*******************************************************************/

// クライアントリストからクライアントFDに対応するクライアントが存在するかチェック
bool Server::isClientExist(int clientFd)
{
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return (false);
	return (true);
}

// クライアントリストからニックネームに対応するクライアントが存在するかチェック
bool Server::isClientExist(std::string &nickname)
{
	std::map<const int, Client>::iterator it = _clientList.begin();
	while (it != _clientList.end())
	{
		if (it->second.getNickname() == nickname)
			return (true);
		++it;
	}
	return (false);
}

// クライアントリストからクライアントのFDに対応するクライアントを取得
Client *Server::getClient(int clientFd)
{
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);

	// クライアントが見つからなかった場合、エラー文を出力してNULLを返す
	if (it == _clientList.end())
		return (NULL);
	return (&it->second);
}

// クライアントリストからニックネームに対応するクライアントのFDを取得
int Server::getClientFdByNick(std::string &nick)
{
	std::map<const int, Client>::iterator it = _clientList.begin();
	while (it != _clientList.end())
	{
		if (it->second.getNickname() == nick)
			return (it->first);
		++it;
	}
	return (-1);
}

// クライアントリストからクライアントのFDに対応するニックネームを取得
std::string Server::getNickname(int clientFd)
{
	std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return ("");
	return (it->second.getNickname());
}

// クライアントリストに追加
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

// クライアントリストから削除し、ソケットを閉じる
void Server::deleteClient(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd)
{
	std::cout << INDIGO SERVER_DISCONNECT_CLIENT << clientFd << END << std::endl;

	// クライアントが参加しているチャンネルからクライアントを削除
	for (std::map<std::string, Channel>::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
	{
		if (it->second.isClientInChannel(clientFd))
			it->second.removeClient(clientFd);
	}

	_clientList.erase(clientFd);
	close(clientFd);
	pollFds.erase(it);

	std::cout << INDIGO SERVER_NUMBER_OF_CLIENTS << pollFds.size() - 1 << END << std::endl;
}

// クライアントにデータを送信する
void Server::sendServerReply(int clientFd, std::string &message)
{
	std::istringstream iss(message);
	std::string line;

	// クライアントにデータを送信
	send(clientFd, message.c_str(), message.size(), 0);
	
	// 送信したデータを表示
	while (getline(iss, line))
	{
		std::cout << BLUE "[Server] " << clientFd << " > " << line << END << std::endl; 
	}
}


/*******************************************************************/
/*                       Channel Management                        */
/*******************************************************************/

// チャンネルを作成し、チャンネルリストに追加
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

// チャンネルにクライアントを追加
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

// チャンネルが存在するか確認
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
