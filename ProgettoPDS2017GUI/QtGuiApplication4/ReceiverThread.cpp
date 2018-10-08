#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet, QObject *parent) : QThread(parent)
{
	this->sharedUserSet = sharedUserSet;
}

/* Distruttore: rilascia le risorse contenute nell’oggetto.*/
ReceiverThread::~ReceiverThread()
{
	qDebug() << "Chiusura di ReceiverThread...\n";
	//Termino la ricezione sul socket.
	shutdown(listenSocket, 0);
	//Chiudo il socket.
	closesocket(listenSocket);
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
	//Azzero i puntatori, così da far decrementare il punteggio di condivisione.	
	sharedUserSet = nullptr;
}

/*Metodo chiamato all'avvio del thread.*/
void ReceiverThread::run()
{
	//Variabile winsock.
	WSADATA wsaData;
	//Intero contenente il codice di errore.
	int iResult;
	//Boolean di verifica ip.
	bool isContained;
	//List contenente gli IP del dispositivo corrente.
	std::set <std::string> hostIP;
	//Stringa contenente il nome dell'host corrente.
	char hostName[80];

	//Inizializzo Winsock.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		EmitException(QString::fromStdString("Lo startup di Winsock e' fallito con il seguente errore: " + std::to_string(iResult)));
		return;
	}



	//PRELIEVO DELLA LISTA DI IP DEL DISPOSITIVO CORRENTE.
	//Provo a prendere il nome dell'host corrente.
	iResult = gethostname(hostName, sizeof(hostName));
	if (iResult == SOCKET_ERROR)
	{
		EmitException(QString::fromStdString("Il prelievo del nome dell'host e' fallito con il seguente errore: " + std::to_string(WSAGetLastError())));
		return;
	}
	qDebug() << "Il nome dell'host e': " << hostName << "." << "\n";

	//Prelevo la lista degli indirizzi del dispositivo corrente.
	struct hostent *phe = gethostbyname(hostName);
	if (phe == 0)
	{
		EmitException(QString::fromStdString("Il prelievo della lista degli indirizzi dell'host e' fallito"));
		return;
	}

	//Ciclo e salvo gli ip del dispositivo corrente.
	for (iResult = 0; phe->h_addr_list[iResult] != 0; ++iResult)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[iResult], sizeof(struct in_addr));
		hostIP.insert(std::string(inet_ntoa(addr)));
		qDebug() << "Indirizzo " << iResult << ": " << inet_ntoa(addr) << "\n";
	}

	//Inserisco nella coda anche l'indirizzo di loopback 127.0.0.1.
	hostIP.insert(std::string("127.0.0.1"));
	qDebug() << "Indirizzo " << iResult << ": " << "127.0.0.1" << "\n";



	//SETTING DEL LISTENER.
	//Creo quel che sembra un socket UDP ordinario.
	listenSocket = (int)socket(AF_INET, SOCK_DGRAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		EmitException(QString::fromStdString("La creazione del socket e' fallita con il seguente errore: " + std::to_string(WSAGetLastError())));
		return;
	}

	//Consento a più socket di usare lo stesso numero di porta.
	u_int yes = 1;
	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
	if (iResult < 0)
	{	
		EmitException(QString::fromStdString("L'opzione di riuso dell'indirizzo e' fallita con il seguente errore: " + std::to_string(WSAGetLastError())));
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
		EmitException(QString::fromStdString("Il bind degli indirizzi e' fallito con il seguente errore: " + std::to_string(WSAGetLastError())));
		return;
	}

	//Richiedo che il kernel si unisca ad un gruppo multicast definito dall'ip multicastIP.
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(multicastIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	iResult = setsockopt(listenSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iResult < 0)
	{
		EmitException(QString::fromStdString("Il prelievo del nome dell'host e' fallito con il seguente errore: " + std::to_string(WSAGetLastError())));
		return;
	}

	//Loop di ricezione messaggi UDP.
	while (1)
	{
		char msgbuf[multicastMsgBufSize];
		int addrlen = sizeof(addr);
		iResult = recvfrom(listenSocket, msgbuf, multicastMsgBufSize, 0, (struct sockaddr *) &addr, &addrlen);
		if (iResult < 0)
		{
			EmitException(QString::fromStdString("La ricezione dei dati e' fallita con il seguente errore: " + std::to_string(WSAGetLastError())));
			return;
		}
		//Inserisco il carattere di terminazione al termine della stringa.
		msgbuf[iResult] = '\0';

		//Creo una stringa contenente l'ip ricevuto.
		std::string receivedIp = inet_ntoa(addr.sin_addr);

		//Se l'ip coincide con uno di quelli contenuti nel set isContained verrà settato a true.
		isContained = hostIP.find(receivedIp) != hostIP.end();
		//Se in precedenza è stato posto isContained a true (messaggio ricevuto dall'host stesso) non entro.
		if (isContained == false)
		{
			qDebug() << "ReceiverThread: ho ricevuto questo dato: " << msgbuf;
			//Inserisco nome utente ed ip nel set.
			sharedUserSet->insert(UserData(std::string(msgbuf), receivedIp));
			//Emetto un segnale per indicare che è stato inserito un utente nella lista.
			emit SetUsersListReady();
		}
		qDebug() << "ReceiverThread: ciclo. \n";
	}
	return;
}

/*Metodo di emissione del segnale di eccezione.*/
void ReceiverThread::EmitException(QString exception)
{
	//Aggiungo una riga comune a tutte le eccezioni.
	exception.append(". Si prega di verificare la connessione e di riavviare il programma.");
	//Emetto il segnale relativo alle eccezioni.
	emit ReceiverException(exception);
}