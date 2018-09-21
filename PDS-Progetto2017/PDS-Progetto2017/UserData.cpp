#include "UserData.hpp"

UserData::UserData(std::string name, std::string ip)
{
	this->name = name;
	this->ip = ip;
};

/*GETTERS.*/
std::string UserData::getName()
{
	return name;
};

std::string UserData::getIP() const
{
	return ip;
};
