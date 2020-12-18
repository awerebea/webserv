#include "Response.hpp"

int Response::bad_req() {
	int fd;
	char buff[2048];
	bzero(buff, sizeof(buff));
	body.clear();
	date.clear();
	date = my_localtime();
	fd = open("./html_files/50x.html", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 500;
	}
	while (read(fd, buff, sizeof(buff))) {
		body.append(buff);
	}
	response = "HTTP/1.1 400 Bad Request\r\nDate: ";
	response.append(date);
	response.append("Server: webserv0.0\r\nContent-Length: ");
	response.append(std::to_string(body.length())); response.append("\r\n");
	response.append("Content-Type: text/html; charset=UTF-8\r\nConnection: Closed\r\n\r\n");
	response.append(body);
	response.append("\r\n\r\n");
	return (0);
}

int Response::ok() {
	int fd;
	char buff[2048];
	bzero(buff, sizeof(buff));
	body.clear();
	date.clear();
	date = my_localtime();
	fd = open("./html_files/index.html", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 500;
	}
	while (read(fd, buff, sizeof(buff))) {
		body.append(buff);
	}
	response = "HTTP/1.1 200 OK\r\nDate: ";
	response.append(date);
	response.append("Server: webserv0.0\r\nContent-Length: ");
	response.append(std::to_string(body.length())); response.append("\r\n");
	response.append("Content-Type: text/html; charset=UTF-8\r\nConnection: Keep-Alive\r\n\r\n");
	response.append(body);
	response.append("\r\n\r\n");
	return (0);
}

int Response::server_error() {
	int fd;
	char buff[2048];
	bzero(buff, sizeof(buff));
	body.clear();
	date.clear();
	date = my_localtime();
	fd = open("./html_files/50x.html", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 500;
	}
	while (read(fd, buff, sizeof(buff))) {
		body.append(buff);
	}
	response = "HTTP/1.1 500 InternalServerError\r\nDate: ";
	response.append(date);
	response.append("Server: webserv0.0\r\nContent-Length: ");
	response.append(std::to_string(body.length())); response.append("\r\n");
	response.append("Content-Type: text/html; charset=UTF-8\r\nConnection: Keep-Alive\r\n\r\n");
	response.append(body);
	response.append("\r\n\r\n");
	return (0);
}

int Response::response_prepare(const Request *req) {
	if (req->error()) {
		if (bad_req()) {
			server_error();
			return 1;
		}
	}
	else {
		if (ok()){
			server_error();
			return 1;
		}
	}
	return (0);
}