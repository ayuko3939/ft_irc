
	if (code == 1)
		return (":Welcome to the Internet Relay Network " + arg1);
	else if (code == 2)
		return ("Your host is " + arg1 + ", running version " + arg2);
	else if (code == 3)
		return ("This server was created " + arg1);
	else if (code == 4)
		return (arg1 + " " + arg2 + " " + arg3 + " " + arg4);
	else if (code == 251)
		return (":There are " + arg1 + " users and " + arg2);
	else if (code == 252)
		return (arg1 + " :operator(s) online");
	else if (code == 253)
		return (arg1 + " :unknown connection(s)");
	else if (code == 254)
		return (arg1 + " :channels formed");
	else if (code == 255)
		return (":I have " + arg1 + " clients and " + arg2 + " servers");
	else if (code == 324)
		return (arg1 + " " + arg2);
	else if (code == 331)
		return (arg1 + " :No topic is set");
	else if (code == 332)
		return (arg1 + " :" + arg2);
	else if (code == 341)
		return (arg1 + " " + arg2);
	else if (code == 353)
		return (arg1 + " " + arg2 + " :" + arg3);
	else if (code == 366)
		return (arg1 + " :End of /NAMES list");
	else if (code == 372)
		return (":- " + arg1);
	else if (code == 375)
		return (":- " + arg1 + " Message of the day - ");
	else if (code == 376)
		return (":End of MOTD command");
	else if (code == 401)
		return (":No such " + arg1);
	else if (code == 402)
		return (arg1 + " :No such server");
	else if (code == 403)
		return (arg1 + " :No such channel");
	else if (code == 404)
		return (arg1 + " :Cannot send to channel");
	else if (code == 409)
		return (":No origin specified");
	else if (code == 431)
		return (":No nickname given");
	else if (code == 432)
		return (arg1 + " :Erroneus nickname");
	else if (code == 433)
		return (arg1 + " :Nickname is already in use");
	else if (code == 441)
		return (arg1 + " " + arg2 + " :They aren't on that channel");
	else if (code == 442)
		return (arg1 + " :You're not on that channel");
	else if (code == 443)
		return (arg1 + " " + arg2 + " :is already on channel");
	else if (code == 461)
		return (arg1 + " :Not enough parameters");
	else if (code == 462)
		return (":You may not reregister");
	else if (code == 464)
		return (":Password incorrect");
	else if (code == 471)
		return (arg1 + " :Cannot join channel (+l)");
	else if (code == 472)
		return (arg1 + " :is unknown mode char to me for " + arg2);
	else if (code == 473)
		return (arg1 + " :Cannot join channel (+i)");
	else if (code == 475)
		return (arg1 + " :Cannot join channel (+k)");
	else if (code == 476)
		return (arg1 + " :Bad Channel Mask");
	else if (code == 481)
		return (":Permission Denied- You're not an IRC operator");
	else if (code == 482)
		return (arg1 + " :You're not channel operator");
	else if (code == 484)
		return (":Your connection is restricted!");
	else if (code == 501)
		return (":Unknown MODE flag");
	else if (code == 503)
		return (":" + arg1);
	else
		return "";
