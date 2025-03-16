#ifndef IRC_HPP
# define IRC_HPP

// Libraries
#include "IrcNumerics.hpp"

# include <iostream>
# include <fstream> // file
# include <sstream> // string
# include <string>
# include <signal.h>
# include <cstring> // memset
# include <ctime> // tm
# include <cstdlib> // exit

# include <netdb.h> // getaddrinfo
# include <poll.h> // poll

# include <vector>
# include <map>


// # include <sys/types.h>
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <arpa/inet.h>


// # include <unistd.h>

// # include <cerrno>



// Color Paretto
# define RED "\033[38;5;196m" // 太字　"\033[38;5;196;1m"
# define ORANGE "\033[38;5;208m"
# define YELLOW "\033[38;5;226m"
# define GREEN "\033[38;5;082m"
# define BLUE "\033[38;5;027m"
# define INDIGO "\033[38;5;025m"
# define VIOLET "\033[38;5;062m"
# define WHITE "\033[38;5;231m"
# define BROWN "\033[38;5;137m"
# define END "\033[0m"

// ===== Error masege =====
// fatal errors
# define ERROR_OPEN_FILE			"Error: could not open file"
# define ERROR_FILE_EMPTY			"Error: file is empty"
# define ERROR_FILE_CONTENTS		"Error: file is wrong"
# define ERROR_SERVER_SOCKET		"Error: socket creation failed"
# define ERROR_SERVER_SETSOCKETOPT	"Error: setsockopt failed"
# define ERROR_SERVER_BIND			"Error: bind failed on port "
# define ERROR_SERVER_LISTEN		"Error: listen failed"
// normal errors
# define ERROR_SERVER_ACCEPT		"Error: accept failed"
# define ERROR_SERVER_FULL			"Error: server is already full"
# define ERROR_CLIENT_NOT_FOUND		"Error: client not found #"
# define ERROR_SERVER_RECV			"Error: recv failed"
# define ERROR_CLIENT_INVALID_INFO	"Error: invalid client information"
# define ERROR_SERVER_POLL			"Error: poll failed on fd: "
# define ERROR_CHANNEL_EXIST		"Error: Channel already exists #"
# define ERROR_CHANNEL_NOT_FOUND	"Error: Channel not found #"
# define ERROR_SERVER_EMPTY_CMD		"Error: empty command"


// # define 

// Server message
# define SERVER_ADD_CLIENT			"[Server] Added client prot:"
# define SERVER_DISCONNECT_CLIENT	"[Server] Client disconnected prot:"
# define SERVER_NUMBER_OF_CLIENTS	"[Server] The current number of clients is "
# define SEVER_REQUIREMENTS			"CHANNELLEN=20 NICKLEN=10 TOPICLEN=50"
# define DEFAULT_KICK_COMMENT		"Kicked by channel operator"

// Settings
# define CONFG_FILE "config/operators.conf"
# define MAX_CLIENTS 4
# define BUF_SIZE_MSG 1024
# define NUM_OF_CMD 12

// サーバー停止フラグ
extern bool g_ServerShutdown;

// コマンド関連
struct s_ircCommand
{
	std::string		prefix; 
	std::string		name; // コマンド名
	std::string		message; // メッセージ
};


#endif