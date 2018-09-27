#include "SenderThread.h"

SenderThread::SenderThread(const char *name, std::shared_ptr<bool>& shareOnlineFlag)
{
	this->name = name;
	this->shareOnlineFlag = shareOnlineFlag;
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
	shareOnlineFlag = nullptr;
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
		if (*shareOnlineFlag == true)
		{

			iResult = (int)sendto(sendSocket, name, strlen(name), 0, (struct sockaddr*) &addr, sizeof(addr));
			if (iResult < 0)
			{
				//TODO: GESTIONE ERRORE.
				std::cout << "L'invio e' fallito con il seguente errore: " << WSAGetLastError() << std::endl;
				return;
			}
			std::cout << "SenderThread: ho inviato questo dato: " << name << std::endl;
			Sleep(multicastSendSleepTime * 1000);
		}
		//Sono offline. Non invio.
		else if (*shareOnlineFlag == false)
		{
			std::cout << "Sono OFFLINE." << std::endl;
			//imposto un tempo di riposo maggiore dato che sono offline.
			Sleep(multicastSendSleepTime * 3000);
		}
	}
}