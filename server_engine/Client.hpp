/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Client.hpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 19:19:04 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/25/20 23:30:46 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "iostream"
# include "vector"
# include "Request.hpp"
# include "Response.hpp"
# include "Bytes.hpp"
# include "Chunk.hpp"
# include <arpa/inet.h>
# include <sys/wait.h>
# include "cgi_data.hpp"

enum states{
	rdy_recv,
	rdy_parse,
	rdy_respone,
	finish
};

class Client{
private:
	char			*read_buff;
	char			*body_buff;

	int			_fd;
	int			state;
	std::string serv_host;
	int			serv_port;

	Bytes			bytes;
	Request			*reqst;
	Response		*resp;
	Chunk			chunk;
	cgi_data		_cgi_data;

	struct timeval	last_msg;

	struct sockaddr_in & addr;

public:

	Client(int fd, std::string const &, int const &, struct sockaddr_in & client_addr);
	~Client();

	void setFd(int fd);
	int getFd();
	int & getStatus();
	void setStatus(int status);
	char const *getBuff();
	char const *getBody();

	Request * getRequest();
	Response * getResponse();
	Bytes	& getBytes();
	Chunk	& getChunk();
	cgi_data const & getCgiData() const;

	std::string const & getServHost();
	int const & getServPort();

	int buffAppend(char const *, const int &);
	int bodyAppend(char const *, const int &);
	int buffCut(unsigned long const &);
	void clearBuff();
	void setCgiData();
	void setResponse(Response *);

	struct timeval & getLastMsg();
	void			setLastMsg();

};

#endif
