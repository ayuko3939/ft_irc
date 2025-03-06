#ifndef IRC_HPP
# define IRC_HPP

// Libraries
# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

# include <iostream>
# include <fstream> // file
# include <sstream> // string
# include <string>
# include <signal.h>
# include <cstring> // memset
# include <ctime> // tm

# include <netdb.h> // getaddrinfo
# include <poll.h> // poll

# include <vector>
# include <map>


// # include <sys/types.h>
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <arpa/inet.h>
// # include <cstdlib>

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

// Settings
# define CONFG_FILE "config/oprators.conf"

// Error masege
# define ERROR_OPEN_FILE		"Error: could not open file."
# define ERROR_FILE_EMPTY		"Error: file is empty."
# define ERROR_FILE_CONTENTS	"Error: file is wrong."
# define ERROR_SERVER_SOCKET	"Error: socket creation failed"
# define ERROR_SERVER_SETSOCKETOPT	"Error: setsockopt failed"
# define ERROR_SERVER_BIND		"Error: bind failed on port "
# define ERROR_SERVER_LISTEN	"Error: listen failed"

#endif