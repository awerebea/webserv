#ifndef SERVER_H
# define SERVER_H
#include <iostream>
#include "vector"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "zconf.h"
#include "unistd.h"
#include "Client.hpp"

class Server{

private:
	std::string response;
	std::string body;
	std::string date;
	std::vector<int> client_fd;
	std::vector<Client*> client_session;
	std::vector<int> server_socks;
	fd_set readset, writeset;
	int max_fd;

public:
	Server() : max_fd(0) { server_socks.reserve(100); };
	~Server(){};
	int launch();
	int setup(std::string const & host, int const port);
	void set_prepare();
	void recv_msg(std::vector<Client*>::iterator it);
	void response_prepare();
	void response_prepare_2();
	void closeConnection(std::vector<Client*>::iterator it);

};


#endif