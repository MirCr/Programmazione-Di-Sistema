#pragma once
#include <string>
#include <QImage>

class UserData
{
	public:
		//Costruttori.
		UserData(std::string name, std::string ip)
		{
			this->name = name;
			this->ip = ip;
		};

		UserData(std::string name, std::string ip, QImage image)
		{
			this->name = name;
			this->ip = ip;
			this-> image = image;
		};

		//Funzione di prelievo del nome utente.
		std::string getName() const
		{
			return name;
		};

		//Funzione di settaggio del nome utente.
		void setName(std::string newName)
		{
			name = newName;
		};

		//Funzione di prelievo dell'ip: usata per confrontare utenti multipli sulla rete.
		std::string getIP() const
		{
			return ip;
		};

		//Funzione di settaggio dell'ip.
		std::string setIP(std::string newIP) 
		{
			ip = newIP;
		};

		//Funzione di prelievo dell'immagine.
		QImage getImage() const
		{
			return image;
		};

		//Funzione di settaggio dell'immagine.
		void setImage(QImage newImage)
		{
			image = newImage;
		};

	private:
		std::string name;
		std::string ip;
		QImage image;
};