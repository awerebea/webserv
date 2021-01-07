#include "CGI.hpp"
#include "stdlib.h"
#include "errno.h"
// CGI

Cgi::Cgi(const cgi_data &data, const std::string &path)  : resp_buff(NULL),_cgi_data(data), file_path(path), _argv(NULL), _env(NULL) {
	return;
}

Cgi::~Cgi() {
	if (resp_buff)
		free (resp_buff);
	if (_env)
	{
		int i = 0;
		while (_env[i])
		{
			free(_env[i]);
			i++;
		}
		free(_env);
	}
	if (_argv)
	{
		int i = 0;
		while (_argv[i])
		{
			free(_argv[i]);
			i++;
		}
		free(_argv);
	}
}

int Cgi::buffAppend(const char *buff, int len){
	if (!resp_buff)
	{
		if (!(resp_buff = bytes.bytesDup(resp_buff, buff, len)))
			return 1;
	}
	else
	{
		char *tmp = resp_buff;
		if (!(resp_buff = bytes.bytesJoin(resp_buff, buff, len,bytes.getBytes())))
			return 1;
		free(tmp);
	}
	return (0);
}

char **Cgi::setEnv() {
	std::map<std::string, std::string>  env_map;
	map_type::const_iterator map_it;

	env_map["AUTH_TYPE"] = "Basic";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	//
	env_map["SCRIPT_NAME"] = "get/it/from/conf/URI";
	env_map["SERVER_NAME"] = "get/it/from/conf/parse";

	env_map["SERVER_PORT"] = std::to_string(_cgi_data.serv_port);
	map_it = _cgi_data.data->find("head");
	env_map["SERVER_PROTOCOL"] = map_it->second[2];
	env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["REMOTE_ADDR"] = inet_ntoa(_cgi_data.addr->sin_addr);
	map_it = _cgi_data.data->find("authorization");
	if (map_it == _cgi_data.data->end() || map_it->second[0].empty())
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = "";
	else
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = map_it->second[0];

	//
	env_map["PATH_TRANSLATED"] = file_path;

	map_it = _cgi_data.data->find("head");
	if (map_it->second[1].find('?') != std::string::npos)
		env_map["QUERY_STRING"] = map_it->second[1].substr(map_it->second[1].find('?') + 1);
	else
		env_map["QUERY_STRING"] = "";

	env_map["CONTENT_LENGTH"] = std::to_string(_cgi_data.body_len);
	map_it = _cgi_data.data->find("content_type");
	if (map_it == _cgi_data.data->end() || map_it->second[0].empty())
		env_map["CONTENT_TYPE"] = "";
	else
		env_map["CONTENT_TYPE"] = map_it->second[0];

	//for PHP
	env_map["REDIRECT_STATUS"] = "1";
	map_it = _cgi_data.data->find("head");
	env_map["PATH_INFO"] = map_it->second[1];
	env_map["METHOD"] = map_it->second[0];
	env_map["REQUEST_URI"] = "http://" + _cgi_data.serv_host + ":" + std::to_string(_cgi_data.serv_port) + map_it->second[1];
	char **env;
	if (!(env = (char **)malloc(sizeof(char *) * (env_map.size() + 1))))
		return (NULL);
	std::map<std::string, std::string>::iterator it = env_map.begin();
	for (int i = 0; it != env_map.end(); ++it, ++i)
		env[i] = ft_strdup((it->first + "=" + it->second).c_str());
	env[env_map.size()] = NULL;
	return env;
}

int Cgi::execute() {
	if(pipe(pipes) < 0 || pipe(err_pipe) < 0)
		return (1);
	if((pid = fork()) < 0)
		return (1);
	if (pid == 0) {
		dup2(pipes[0], 0);
		dup2(pipes[1], 1);
//		write(pipes[1], "first_name=Lebrus&last_name=Shupay", 34); // Body если метод пост
		if (execve(_argv[0], _argv, _env) < 0) {
			close(pipes[0]);
			close(pipes[1]);
			dup2(err_pipe[0], 0);
			dup2(err_pipe[1], 1);
			write(1, "1", 1);
			exit(1);
		}
	}
	return (0);
}

int Cgi::read_response(){
	int n = 0;

	while (1) {
		char line[1024];
		bzero(line, 1024);
		fd_set readset;

		waitpid(pid, &status, WNOHANG);
		if (WEXITSTATUS(status) != 0) {
			return (502);
		}

		FD_ZERO(&readset);
		FD_SET(pipes[0], &readset);
		FD_SET(err_pipe[0], &readset);

		if (select(err_pipe[0] + 1, &readset, 0, 0, 0) < 0)
			return (500); //handle error
		if (FD_ISSET(err_pipe[0], &readset)){
			return (502);
		}
		n = read(pipes[0], line, 1024);
		if((buffAppend(line, n)))
		{
			return (500); //handle error
		}
		bytes.bytesCount(n);
		if (n < 1024) {
			return (0); //file readed
		}
	}
}

int Cgi::handler(){
	if(!(_argv = (char **) malloc(sizeof(char *) * 3)))
		return (500);
	if(!(_argv[0] = ft_strdup("/Users/deddara/.brew/bin/php-cgi")))
		return (500);
	if(!(_argv[1] = ft_strdup(file_path.c_str())))
		return (500);
	_argv[2] = 0;

	if (!(_env = setEnv()))
		return (500);

	if (execute())
		return (500);

	int execute_result = read_response();

	close(pipes[0]);
	close(pipes[1]);
	close(err_pipe[0]);
	close(err_pipe[1]);
	return (execute_result);
}



char const * Cgi::getResponse() const { return resp_buff; }

const Bytes & Cgi::getBytes() const { return bytes; }