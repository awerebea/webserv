/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Client.cpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:28:34 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:21:35 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Client.hpp"
#include "sys/time.h"

Client::Client(int fd, std::string const & host, int const & port, struct sockaddr_in & client_addr) :
	read_buff(nullptr), body_buff(nullptr), _fd(fd), state(0), serv_host(host), serv_port(port), addr(client_addr) {
	body_len = 0;
	body_capacity = 0;
	body_occupied = 0;
	read_buff_capacity = 0;
	read_buff_occupied = 0;
	reqst = new Request();
	resp = new Response();
	gettimeofday(&last_msg, NULL);
}

Client::~Client() {
		delete reqst;
		delete resp;
}

void Client::setFd(int fd) { _fd = fd; }
int Client::getFd() { return _fd; }
int & Client::getStatus() {return state; }
void Client::setStatus(int status) { state = status; }
Request * Client::getRequest() { return reqst; }
Response * Client::getResponse() { return resp; }

Bytes & Client::getBytes() {
	return bytes;
}

Chunk & Client::getChunk() {
	return chunk;
}

const std::string & Client::getServHost() { return serv_host; }

const int & Client::getServPort() { return serv_port; }

int Client::bodyAppend(char const *buff, const int &len) {
	char *		tmp = nullptr;
	if (body_buff) {
		if (body_capacity < body_occupied + len) {
			tmp = body_buff;
			body_capacity = (body_occupied + len) * 2;
			if(!(body_buff = (char*)malloc((body_capacity + 1)))) {
				return 1;
			}
			bzero(body_buff, body_capacity);
			body_buff[body_capacity] = '\0';
			ft_memcpy(body_buff, tmp, body_occupied);
			ft_memcpy(body_buff + body_occupied, buff, len);
			body_occupied += len;
			free(tmp);
			tmp = nullptr;
		} else {
			ft_memcpy(body_buff + body_occupied, buff, len);
			body_occupied += len;
		}
	} else {
		body_capacity = len;
		if (!(body_buff = (char*)malloc(len + 1))) {
			return 1;
		}
		body_buff[len] = '\0';
		ft_memcpy(body_buff, buff, len);
		body_occupied = body_capacity;
	}
	return 0;
}

int Client::buffAppend(char const * buff, const int & len) {
	char *		tmp = nullptr;
	if (read_buff) {
		if (read_buff_capacity < read_buff_occupied + len) {
			tmp = read_buff;
			read_buff_capacity = (read_buff_occupied + len) * 2;
			if(!(read_buff = (char*)malloc(read_buff_capacity + 1))) {
				return 1;
			}
			bzero(read_buff, read_buff_capacity);
			read_buff[read_buff_capacity] = '\0';
			ft_memcpy(read_buff, tmp, read_buff_occupied);
			ft_memcpy(read_buff + read_buff_occupied, buff, len);
			read_buff_occupied += len;
			free(tmp);
			tmp = nullptr;
		} else {
			ft_memcpy(read_buff + read_buff_occupied, buff, len);
			read_buff_occupied += len;
		}
	} else {
		read_buff_capacity = len;
		if (!(read_buff = (char*)malloc(len + 1))) {
			return 1;
		}
		read_buff[len] = '\0';
		ft_memcpy(read_buff, buff, len);
		read_buff_occupied = read_buff_capacity;
	}
	return 0;
}

int Client::buffCut(const unsigned long &len) {
	char *tmp = read_buff;
	if (!(read_buff = bytes.bytesCut(read_buff, len)))
		return 1;
	free(tmp);
	return (0);
}

const char * Client::getBuff() { return read_buff; }

char * Client::getBody() {
	return body_buff;
}

void Client::setBody(char *str) { body_buff = str; }

struct timeval &Client::getLastMsg(){
	return last_msg;
}

void		Client::setLastMsg(){
	gettimeofday(&last_msg, NULL);
}

void Client::clearBuff()
{
	if (read_buff)
		free(read_buff);
//	if (body_buff)
//		free(body_buff);
	body_capacity = 0;
	body_occupied = 0;
	read_buff_capacity = 0;
	read_buff_occupied = 0;
	read_buff = nullptr;
//	body_buff = nullptr;
}

void Client::setCgiData() {
	_cgi_data.data = &reqst->getData();
	_cgi_data.addr = &addr;
	_cgi_data.body_len = body_len;
	_cgi_data.serv_host = serv_host;
	_cgi_data.serv_port = serv_port;
}

void Client::setResponse(Response *response) {
	resp = response;
}

void Client::setRequest(Request *req) { reqst = req; }

cgi_data const & Client::getCgiData() const { return _cgi_data; }


void Client::clearReqBody() {
	if (body_buff){
		free(body_buff);
		body_buff = nullptr;
	}
}

void Client::setBodyLen(const int &num) { body_len = num; }
const int & Client::getBodyLen() const { return body_len; }
