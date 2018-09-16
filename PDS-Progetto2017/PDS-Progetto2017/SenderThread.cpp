#include "SenderThread.h"

/* Distruttore: rilascia le risorse contenute nell’oggetto.*/
SenderThread::~SenderThread()
{
	//Faccio il join del thread per assicurarmi che venga distrutto.
	if (thread.joinable())
		thread.join();
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
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
	const char *message = "malnati";

	WSADATA wsaData;
	if (WSAStartup(0x0101, &wsaData)) 
	{
		//TODO: GESTIONE ERRORE.
		perror("WSAStartup");
	}

	//Crea quel che sembra un socket UDP ordinario.
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		//TODO: GESTIONE ERRORE.
		perror("socket");
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
		char ch = 0;
		int nbytes = sendto(sock, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr));
		if (nbytes < 0) 
		{
			//TODO: GESTIONE ERRORE.
			perror("sendto");
		}
		std::cout << "SenderThread: ho inviato questo dato: " << message << std::endl;
		Sleep(multicastSendSleepTime*1000);
	}
}