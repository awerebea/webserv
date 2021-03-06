/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:26:04 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/12 10:58:46 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPages.hpp"

ErrorPages::ErrorPages() {
	std::vector<std::string>		code200;
	code200.push_back("");
	code200.push_back("OK");
	errorPageTemplates.insert(std::make_pair(200, code200));

	std::vector<std::string>		code201;
	code201.push_back("");
	code201.push_back("Created");
	errorPageTemplates.insert(std::make_pair(201, code201));

	std::vector<std::string>		code301;
	code301.push_back("");
	code301.push_back("Moved Permanently");
	errorPageTemplates.insert(std::make_pair(301, code301));

	std::vector<std::string>		code302;
	code302.push_back("");
	code302.push_back("Found");
	errorPageTemplates.insert(std::make_pair(302, code302));

	std::vector<std::string>		code400;
	code400.push_back(
		"<html>\n"													\
		"<head><title>400 Bad Request</title></head>\n"				\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>400 Bad Request</h1></center>\n"				\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code400.push_back("Bad Request");
	errorPageTemplates.insert(std::make_pair(400, code400));

	std::vector<std::string>		code401;
	code401.push_back(
			"<html>\n"													\
		"<head><title>401 Unauthorized</title></head>\n"				\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>401 Unauthorized</h1></center>\n"				\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
	);
	code401.push_back("Unauthorized");
	errorPageTemplates.insert(std::make_pair(401, code401));

	std::vector<std::string>		code403;
	code403.push_back(
		"<html>\n"													\
		"<head><title>403 Forbidden</title></head>\n"				\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>403 Forbidden</h1></center>\n"					\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code403.push_back("Forbidden");
	errorPageTemplates.insert(std::make_pair(403, code403));

	std::vector<std::string>		code404;
	code404.push_back(
		"<html>\n"													\
		"<head><title>404 Not Found</title></head>\n"				\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>404 Not Found</h1></center>\n"					\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code404.push_back("Not Found");
	errorPageTemplates.insert(std::make_pair(404, code404));

	std::vector<std::string>		code405;
	code405.push_back(
		"<html>\n"													\
		"<head><title>405 Method Not Allowed</title></head>\n"		\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>405 Method Not Allowed</h1></center>\n"		\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code405.push_back("Not Allowed");
	errorPageTemplates.insert(std::make_pair(405, code405));

	std::vector<std::string>		code411;
	code411.push_back(
		"<html>\n"													\
		"<head><title>411 Length Required</title></head>\n"	\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>411 Length Required</h1></center>\n"		\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code411.push_back("Length Required");
	errorPageTemplates.insert(std::make_pair(411, code411));

	std::vector<std::string>		code413;
	code413.push_back(
		"<html>\n"													\
		"<head><title>413 Request Entity Too Large</title></head>\n"	\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>413 Request Entity Too Large</h1></center>\n"		\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code413.push_back("Request Entity Too Large");
	errorPageTemplates.insert(std::make_pair(413, code413));

	std::vector<std::string>		code500;
	code500.push_back(
		"<html>\n"													\
		"<head><title>500 Internal Server Error</title></head>\n"	\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>500 Internal Server Error</h1></center>\n"		\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code500.push_back("Internal Server Error");
	errorPageTemplates.insert(std::make_pair(500, code500));

	std::vector<std::string>		code502;
	code502.push_back(
		"<html>\n"													\
		"<head><title>502 Bad Gateway</title></head>\n"	\
		"<body bgcolor=\"white\">\n"								\
		"<center><h1>502 Bad Gateway</h1></center>\n"		\
		"<hr><center>webserv/0.1a</center>\n"						\
		"</body>\n"													\
		"</html>\n"
		);
	code502.push_back("Bad Gateway");
	errorPageTemplates.insert(std::make_pair(502, code502));
}

ErrorPages::~ErrorPages() {}

void					ErrorPages::valClear(std::vector<std::string> & val) {
	val.clear();
	if (!val.empty()) {
		std::cout << "Error: system failure" << std::endl;
		exit(1);
	}
}

std::map<int, std::vector<std::string> > const &
						ErrorPages::getErrorPageTemplates() const {
	return errorPageTemplates;
}
