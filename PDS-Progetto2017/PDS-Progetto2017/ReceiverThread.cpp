#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(std::shared_ptr<std::mutex>& sharedMainRecvMutex, std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet, std::shared_ptr<std::condition_variable>& sharedMainRecvCondVar, std::shared_ptr<bool>& sharedMainRecvFlag)
{
	this->sharedMainRecvMutex = sharedMainRecvMutex;
	this->sharedUserSet = sharedUserSet;
	this->sharedMainRecvCondVar = sharedMainRecvCondVar;
	this->sharedMainRecvFlag = sharedMainRecvFlag;
}

/* Distruttore: rilascia le risorse contenute nell’oggetto.*/
ReceiverThread::~ReceiverThread()
{
	std::cout << "Chiusura di ReceiverThread..." << std::endl;
	//Faccio il join del thread per assicurarmi che venga distrutto.
	if (thread.joinable())
		thread.join();
	//Termino la ricezione sul socket.
	shutdown(listenSocket, 0);
	//Chiudo il socket.
	closesocket(listenSocket);
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
	//Azzero i puntatori, facendo così decrementare il punteggio di condivisione.
	sharedMainRecvMutex = nullptr;
	sharedUserSet = nullptr;
	sharedMainRecvCondVar = nullptr;
	sharedMainRecvFlag = nullptr;
};

/* Metodo di avvio del thread sulla classe ReceiverThread.*/
void ReceiverThread::start()
{
	std::cout << "ReceiverThread: sono in esecuzione." << std::endl;
	thread = std::thread(&ReceiverThread::listenForMulticastData, this);
};

/* Metodo per l'ascolto di pacchetti UDP multicast.*/
void ReceiverThread::listenForMulticastData()
{
	//Variabile winsock.
	WSADATA wsaData;
	//Interi contenenti rispettivamente codici di errore e flags.
	int iResult, ipFlag;

	//Inizializzo Winsock.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "Lo startup di Winsock e' fallito con il seguente errore: " << iResult << std::endl;
		return;
	}

	//Prelevo la lista di ip del dispositivo corrente.
	std::list <std::string> hostIP = getHostIPs();

	//SETTING DEL LISTENER.
	//Creo quel che sembra un socket UDP ordinario.
	listenSocket = (int) socket(AF_INET, SOCK_DGRAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "La creazione del socket e' fallita con il seguente errore: " << WSAGetLastError() << std::endl;
		return;
	}

	//Consento a più socket di usare lo stesso numero di porta.
	u_int yes = 1;
	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
	if (iResult < 0)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "L'opzione di riuso dell'indirizzo e' fallita con il seguente errore: " << WSAGetLastError() << std::endl;
		return;
	}

	//Setto l'indirizzo destinazione.
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //Differisce dal sender.
	addr.sin_port = htons(multicastPort);

	//Associazione dell'indirizzo locale con il socket.
	iResult = bind(listenSocket, (struct sockaddr*) &addr, sizeof(addr));
	if (iResult == SOCKET_ERROR) 
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "Il bind e'  fallito con il seguente errore: " << WSAGetLastError() << std::endl;
		return;
	}

	//Richiedo che il kernel si unisca ad un gruppo multicast definito dall'ip multicastIP.
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	iResult = setsockopt(listenSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iResult < 0)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "L'opzione multicast e' fallita con il seguente errore: " << WSAGetLastError() << std::endl;
		return;
	}

	//Loop di ricezione messaggi UDP.
	while (1) 
	{
		ipFlag = 0;
		char msgbuf[multicastMsgBufSize];
		int addrlen = sizeof(addr);
		iResult = recvfrom(listenSocket, msgbuf, multicastMsgBufSize, 0, (struct sockaddr *) &addr, &addrlen);
		if (iResult < 0)
		{
			//TODO: GESTIONE ERRORE.
			std::cout << "La ricezione e' fallita con il seguente errore: " << WSAGetLastError() << std::endl;
			return;
		}
		//Inserisco il carattere di terminazione al termine della stringa.
		msgbuf[iResult] = '\0';

		//Creo una stringa contenente l'ip ricevuto.
		std::string receivedIp = inet_ntoa(addr.sin_addr);
		//Se l'ip coincide con uno di quelli contenuti nella lista setto il flag ad uno.
		for (std::list<std::string>::iterator it = hostIP.begin(); it != hostIP.end(); it++)
		{
			iResult = receivedIp.compare(*it);
			if (iResult == 0)
			{
				ipFlag = 1;
			}
		}
		//Se in precedenza è stato posto il flag ad 1 (messaggio ricevuto dall'host stesso) non entro.
		if (ipFlag == 0)
		{
			std::cout << "ReceiverThread: ho ricevuto questo dato: " << msgbuf << " da " << receivedIp << std::endl;
			//Definisco un lock di accesso alla risorsa.
			std::lock_guard<std::mutex> uniqueLock(*sharedMainRecvMutex);
			//Inserisco nome utente ed ip nel set.
			sharedUserSet->insert(UserData(std::string(msgbuf),receivedIp));
			//Setto il flag a false così che il main possa eseguire la sua parte di codice.
			*sharedMainRecvFlag = true;
			//Notifico al main che ora può leggere il dato.
			sharedMainRecvCondVar->notify_one();
		}
	}
	return;
};

/*Metodo per prelevare la lista di ip del PC corrente.*/
std::list <std::string> ReceiverThread::getHostIPs()
{
	int i;
	//List contenente gli IP del dispositivo corrente.
	std::list <std::string> hostIP;
	//Stringa contenente il nome dell'host corrente.
	char hostName[80];
	//Intero contenente il codice di errore.
	int iResult;
	//Provo a prendere il nome dell'host corrente.
	iResult = gethostname(hostName, sizeof(hostName));
	if (iResult == SOCKET_ERROR)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "Il prelievo del nome dell'host e' fallito con il seguente errore: " << WSAGetLastError() << std::endl;
	}
	std::cout << "Il nome dell'host e': " << hostName << "." << std::endl;

	//Prelevo la lista degli indirizzi del dispositivo corrente.
	struct hostent *phe = gethostbyname(hostName);
	if (phe == 0)
	{
		//TODO: GESTIONE ERRORE.
		std::cout << "Il prelievo della lista degli indirizzi dell'host e' fallito." << std::endl;
	}

	//Ciclo e salvo gli ip del dispositivo corrente.
	for (i = 0; phe->h_addr_list[i] != 0; ++i)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		hostIP.push_front(std::string(inet_ntoa(addr)));
		std::cout << "Indirizzo " << i << ": " << hostIP.front() << std::endl;
	}

	//Inserisco nella coda anche l'indirizzo di loopback 127.0.0.1.
	hostIP.push_front(std::string("127.0.0.1"));
	std::cout << "Indirizzo " << i << ": " << hostIP.front() << std::endl;

	//restituisco la lista di ip del dispositivo corrente.
	return hostIP;
}