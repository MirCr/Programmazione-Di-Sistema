#pragma once
#include <string>

class UserData
{
	public:
		UserData(std::string name, std::string ip)  //TODO: Aggiungere dato immagine.
		{
			this->name = name;
			this->ip = ip;
		};
		//Funzione di prelievo del nome utente.
		std::string getName() const
		{
			return name;
		};
		//Funzione di prelievo dell'ip: usata per confrontare utenti multipli sulla rete.
		std::string getIP() const
		{
			return ip;
		};
	private:
		std::string name;
		std::string ip;
};