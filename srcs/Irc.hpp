#ifndef IRC_HPP
# define IRC_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

# include <iostream>
# include <string>
# include <signal.h>
# include <cstring> // memset

# include <netdb.h> 

# include <vector>
# include <map>


// # include <sys/types.h>
// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <arpa/inet.h>
// # include <cstdlib>
// # include <poll.h>
// # include <unistd.h>
// # include <sstream>
// # include <cerrno>
// # include <ctime>


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

#endif