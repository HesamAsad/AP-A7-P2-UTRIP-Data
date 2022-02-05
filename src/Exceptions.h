#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>	
#include <exception>

class Bad_request : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Bad Request";
	}
};

class Permission_error : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Permission Denied";
	}
};

class Not_found : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Not Found";
	}
};

class Not_enough_room : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Not Enough Room";
	}
};

class Not_enough_credit : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Not Enough Credit";
	}
};

class Insufficient_rating : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Insufficient Ratings";
	}
};

#endif