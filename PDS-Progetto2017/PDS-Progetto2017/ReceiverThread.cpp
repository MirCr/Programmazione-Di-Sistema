#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(std::shared_ptr<std::mutex>& mutexPointer, std::shared_ptr<std::queue<std::string>>& ipQueuePointer,  std::shared_ptr<std::condition_variable>& mainReceiverCondVar, std::shared_ptr<bool>& mainReceiverFlag)
{
	this->mutexPointer = mutexPointer;
	this->ipQueuePointer = ipQueuePointer;
	this->mainReceiverCondVar = mainReceiverCondVar;
	this->mainReceiverFlag = mainReceiverFlag;
}

/* Distruttore: rilascia le risorse contenute nell’oggetto.*/
ReceiverThread::~ReceiverThread()
{
	//Faccio il join del thread per assicurarmi che venga distrutto.
	if (thread.joinable())
		thread.join();
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
	//Azzero i puntatori, facendo così decrementare il punteggio di condivisione.
	mutexPointer = nullptr;
	ipQueuePointer = nullptr;
	mainReceiverCondVar = nullptr;
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

	//Consente a più socket di usare lo stesso numero di porta.
	u_int yes = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) < 0)
	{
		//TODO: GESTIONE ERRORE.
		perror("Reusing ADDR failed");
	}

	//Settaggio dell'indirizzo destinazione.
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //Differisce dal sender.
	addr.sin_port = htons(multicastPort);

	//Associazione dell'indirizzo locale con il socket.
	if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
	{
		//TODO: GESTIONE ERRORE.
		perror("bind");
	}

	//Utilizzo di setsockopt() per richiedere che il kernel si unisca ad un gruppo multicast.
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0)
	{
		//TODO: GESTIONE ERRORE.
		perror("setsockopt");
	}

	//Entrata in un loop leggi e stampa.
	//TODO: trovare un metodo più furbo per l'attesa di dati e ridurre l'uso del C.
	while (1) 
	{
		char msgbuf[multicastMsgBufSize];
		int addrlen = sizeof(addr);
		int nbytes = recvfrom(sock, msgbuf, multicastMsgBufSize, 0, (struct sockaddr *) &addr, &addrlen);
		if (nbytes < 0) 
		{
			//TODO: GESTIONE ERRORE.
			perror("recvfrom");
		}
		//Inserisco il carattere di terminazione stringa.
		msgbuf[nbytes] = '\0';

		std::cout << "ReceiverThread: ho ricevuto questo dato: " << msgbuf << std::endl;
		//Definisco un lock di accesso alla risorsa.
		std::lock_guard<std::mutex> uniqueLock(*mutexPointer);
		//Inserisco l'elemento ricevuto nella queue.
		ipQueuePointer->push(msgbuf);
		//Setto il flag a false così che il main possa eseguire la sua parte di codice.
		*mainReceiverFlag = true;
		//Notifico al main che ora può leggere il dato.
		mainReceiverCondVar->notify_one();
		std::cout << "ReceiverThread: ho inserito il dato e notificato il thread principale." << std::endl;
	}

	return;
};