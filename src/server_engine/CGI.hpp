#ifndef CGI_H
# define CGI_H
# include "includes.hpp"
# include <map>
# include <vector>
#include "cgi_data.hpp"
#include "unistd.h"
#include "Bytes.hpp"
#include <sys/types.h>
#include <sys/wait.h>

class Cgi{
private:
	char				* resp_buff;
	const cgi_data		& _cgi_data;
	std::string const	&file_path;
	std::string const	&bin_path;
	Bytes				bytes;
	char			*body;
	std::string			method;

	int fd[2];
	int fd_tmp;
	pid_t pid;
	char **_argv;
	char **_env;
	int status;


public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Cgi(cgi_data const &, std::string const &, std::string const &, char *);
	~Cgi();


	char *			getResponse();
	Bytes const &	getBytes() const;

	char			**setEnv();
	int				execute();
	int				handler();
	int				buffAppend(const char *buff, int len);
	int				read_response();
	void 			clearResponseBuf();


};

#endif
