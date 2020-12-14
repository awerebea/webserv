#include <iostream>
#include "sstream"
#include "fstream"
#include "vector"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "zconf.h"

class Server{
public:
	std::string response;

};

int main()
{
	fd_set readset, writeset;
	int listen_sock, accept_sock;
	struct sockaddr_in addr;
	std::vector<int> client_fd;
	Server	serv;
	char buff[1024];
	bzero(&buff, 1024);
	bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3031);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("listen_sock");
		return (1);
	}
	fcntl(listen_sock, F_SETFL, O_NONBLOCK);
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(opt));
	if ((bind(listen_sock, (struct sockaddr*)& addr, sizeof(addr))) < 0)
	{
		perror("bind");
		return (1);
	}

	if ((listen(listen_sock, 10)) < 0){
		perror("listen_sock");
		return (1);
	}

	int max_fd;
	for (;;){
		max_fd = listen_sock;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		FD_SET(listen_sock, &readset);
		for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); ++it){
			FD_SET(*it, &readset);
			if (serv.response.length() != 0){
				FD_SET(*it, &writeset);
			}
			if (*it > max_fd)
				max_fd = *it;
		}
		if (select(max_fd + 1, &readset, &writeset, NULL, NULL) < 0)
			return (1);
		if (FD_ISSET(listen_sock, &readset))
		{
			struct sockaddr_storage ss;
			socklen_t slen = sizeof(ss);
			if ((accept_sock = accept(listen_sock, (struct sockaddr*) & ss, &slen)) < 0)
			{
				perror("accept");
				return(1);
			}
			fcntl(accept_sock, F_SETFL, O_NONBLOCK);
			client_fd.push_back(accept_sock);
		}
		for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); ++it) {
			if (FD_ISSET(*it, &readset))
			{
				int n;
				while ((n = recv(*it, buff, sizeof(buff), 0)) > 0)
				{
					if (buff[n - 1] == '\n')
						break;
				}
				if (n <= 0)
				{
					close(*it);
					client_fd.erase(it);
					break;
				}
				serv.response = "HTTP/1.1 200 OK\r\n"
								"Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
								"Server: Apache/2.2.14 (Win32)\r\n"
								"Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
								"Content-Length: 88\r\n"
								"Content-Type: text/html\r\n"
								"Connection: Keep-Alive\r\n\r\n"
								"<html>\n"
								"<body>\n"
								"<h1>Hello, World!</h1>\n"
								"</body>\n"
								"</html>\r\n";
			}
			if (FD_ISSET(*it, &writeset)){
				if ((send(*it, serv.response.c_str(), serv.response.length(), 0)) < 0)
				{
					perror("send");
					return 1;
				}
				serv.response = "\0";
			}
		}
	}
	
}