#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "CGI.hpp"
# include "ErrorPages.hpp"
# include "VirtServer.hpp"
# include "cgi_data.hpp"
# include "get_next_line.hpp"
# include "includes.hpp"
# include <dirent.h>
# include <fcntl.h>
# include <map>
# include <stdexcept>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

struct									s_response {
	char *								headers;
	char *								headersCurr;
	size_t								headersLength;
	char *								body;
	char *								bodyCurr;
	size_t								bodyLength;
	char *								data_begin_p;
	char *								curr;
	size_t								shiftBody;
};

class Response{
private:
	std::map<std::string, std::vector<std::string> > const *
										_data;
	std::map<int, std::vector<std::string> > const *
										errorPageTempl;

	int									errCode;
	int									reqBodyLen;
	char *								body;
	char *						reqBody;
	size_t								bodyLength;
	size_t								currLocationInd;
	const VirtServer *					virtServ;
	std::vector<Location *>				location;
	std::string							responseHeaders;
	std::string							redirectURI;
	std::map<int, std::string> const *	errorPage;
	std::string							filePath;
	std::string							fileModifiedTime;
	std::string							webservVersion;
	std::string							dirListing;
	std::string							fileExt;
	std::string                         realm;   // for WWW-Authenticate
	struct s_response					response;
	Cgi								*	cgi;

	// errHandlersFlags: 0b00000001 - 403 checked, 0b00000010 - 404 checked
	char								errHandlersFlags;

	int									checkAuth();
	int									checkLocation();
	int									checkAllowMethods();
	int									checkFile();
	void								errorHandler();
	void								error403Handler();
	void								error404Handler();
	void								generateBody();
	void								generateFilePath();
	void								buildResponse();
	void								generateDirListing();
	void								cgi_response_parser(Cgi &);
	int									checkLimitClientBody();
	int									checkExtForCgiHandling();
	void								putHandler();

public:
	typedef std::map<std::string, std::vector<std::string> > const
										map_type;
										Response();
										~Response();

	void	setServerData(VirtServer const &);
	void	setErrorPageTempl(const std::map<int, std::vector<std::string> > *);
	void	setErrcode(int const &num );
	void	setReqBody(char *);
	void	responsePrepare(int &, std::map<std::string,
						std::vector<std::string> > const *, const cgi_data &);
	void	connectionHandler(int & status);
	void	clearResponseData();

	struct s_response &					getResponseStruct();
	VirtServer const &					getVirtServer() const;
};

#endif
