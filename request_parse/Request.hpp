/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeremia <jjeremia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 10:45:11 by jjeremia          #+#    #+#             */
/*   Updated: 2020/12/18 14:57:28 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Request {
public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	typedef std::vector<std::string>                            value_type;

private:
	void check_common();

	map_type    _data;
	std::string _body;
	bool        _isbadrequest;

public:
	//Constructors
	Request() : _body(""), _isbadrequest(false) {};
	Request(std::string const &raw_data);
	Request(Request const &obj);
	Request &operator=(Request const &obj);
	~Request();

	void req_init(std::string const &raw_data);

	map_type::const_iterator begin() const;

	map_type::const_iterator end() const;

	//Check error
	bool error() const;

	//Check value existence
	bool is_valid_value(std::string const &key) const;

	//Element access
	const value_type &find(std::string const &key) const;
	const std::string &get_body() const;

};


#endif
