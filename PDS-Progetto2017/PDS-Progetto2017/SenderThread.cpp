#include "SenderThread.h"

SenderThread::SenderThread(std::string name, std::shared_ptr<bool>& sharedOnlineFlag)
{
	this->name = name;
	this->sharedOnlineFlag = sharedOnlineFlag;
}

/* Distruttore: rilascia le risorse contenute nell’oggetto.*/
SenderThread::~SenderThread()
{
	std::cout << "Chiusura di SenderThread..." << std::endl;
	//Faccio il join del thread per assicurarmi che venga distrutto.
	if (thread.joinable())
		thread.join();
	//Termino l'invio dal socket.
	shutdown(sendSocket, 1);
	//Chiudo il socket.
	closesocket(sendSocket);
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
	//Azzero il puntatore, facendo così decrementare il punteggio di condivisione.
	sharedOnlineFlag = nullptr;
}

/* Metodo di avvio del thread sulla classe SenderThread.*/
void SenderThread::start()
{
	std::cout << "SenderThread: sono in esecuzione." << std::endl;
	thread = std::thread(&SenderThread::sendMulticastPackets, this);
}

/*Metodo per l'invio di pacchetti UDP multicast.*/
void SenderThread::sendMulticastPackets()
{
	//Converto il nome dell'utente in un const char *;
	const char * cname = name.c_str();

	//Variabile winsock.
	WSADATA wsaData;
	//Intero contenente il codice di errore.
	int iResult;

	//Inizializzo Winsock.
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "Lo startup di Winsock e' fallito con il seguente errore: " << iResult << std::endl;
		return;
	}

	//Crea quel che sembra un socket UDP ordinario.
	sendSocket = (int) socket(AF_INET, SOCK_DGRAM, 0);
	if (sendSocket == INVALID_SOCKET)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "La creazione del socket e' fallita con il seguente errore: " << WSAGetLastError() << std::endl;
		return;
	}

	//Setting dell'indirizzo di destinazione.
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(multicastIP);
	addr.sin_port = htons(multicastPort);

	//Invio dei dati verso la destinazione.
	while (1) 
	{
		//Sono online. Quindi invio.
		if (*sharedOnlineFlag == true)
		{

			iResult = (int)sendto(sendSocket, cname, strlen(cname), 0, (struct sockaddr*) &addr, sizeof(addr));
			if (iResult < 0)
			{
				//TODO: GESTIONE ERRORE.
				std::cout << "L'invio e' fallito con il seguente errore: " << WSAGetLastError() << std::endl;
				return;
			}
			std::cout << "SenderThread: ho inviato questo dato: " << cname << std::endl;
			Sleep(multicastOnlineSendSleepTime * 1000);
		}
		//Sono offline. Non invio.
		else if (*sharedOnlineFlag == false)
		{
			std::cout << "SenderThread: Sono OFFLINE." << std::endl;
			//imposto un tempo di riposo maggiore dato che sono offline.
			Sleep(multicastOfflineSendSleepTime * 1000);
		}
	}
}