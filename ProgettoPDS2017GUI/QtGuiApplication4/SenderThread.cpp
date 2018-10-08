#include "SenderThread.h"

SenderThread::SenderThread(std::shared_ptr<UserData>& sharedUserName, std::shared_ptr<bool>& sharedOnlineFlag, QObject *parent): QThread(parent)
{
	this->sharedUserName = sharedUserName;
	this->sharedOnlineFlag = sharedOnlineFlag;
}

SenderThread::~SenderThread()
{
	qDebug() << "Chiusura di SenderThread... \n";
	//Termino l'invio dal socket.
	shutdown(sendSocket, 1);
	//Chiudo il socket.
	closesocket(sendSocket);
	//Termino l'uso della Winsock 2 DLL(Ws2_32.dll).
	WSACleanup();
	//Azzero i puntatori, così da far decrementare il punteggio di condivisione.
	sharedUserName = nullptr;
	sharedOnlineFlag = nullptr;
}

/*Metodo chiamato all'avvio del thread.*/
void SenderThread::run()
{
	//Variabile winsock.
	WSADATA wsaData;
	//Intero contenente il codice di errore.
	int iResult;

	//Inizializzo Winsock.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		EmitException(QString::fromStdString("Lo startup di Winsock e' fallito con il seguente errore: " + std::to_string(iResult)));
		return;
	}

	//Crea quel che sembra un socket UDP ordinario.
	sendSocket = (int)socket(AF_INET, SOCK_DGRAM, 0);
	if (sendSocket == INVALID_SOCKET)
	{
		EmitException(QString::fromStdString("La creazione del socket e' fallita con il seguente errore: " + std::to_string(WSAGetLastError())));
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
		//Converto il nome dell'utente in un const char *;
		std::string nome = sharedUserName->getName();
		const char * cname = nome.c_str();

		//Sono online. Quindi invio.
		if (*sharedOnlineFlag == true)
		{

			iResult = (int)sendto(sendSocket, cname, strlen(cname), 0, (struct sockaddr*) &addr, sizeof(addr));
			if (iResult < 0)
			{
				EmitException(QString::fromStdString("L'invio dei dati e' fallito con il seguente errore: " + std::to_string(WSAGetLastError())));
				return;
			}
			qDebug() << "SenderThread: ho inviato questo dato: " << cname << "\n";
			Sleep(multicastOnlineSendSleepTime * 1000);
		}
		//Sono offline. Non invio.
		else if (*sharedOnlineFlag == false)
		{
			qDebug() << "SenderThread: Sono OFFLINE." << "\n";
			//imposto un tempo di riposo maggiore dato che sono offline.
			Sleep(multicastOfflineSendSleepTime * 1000);
		}
	}
}

/*Metodo di emissione del segnale di eccezione.*/
void SenderThread::EmitException(QString exception)
{
	//Aggiungo una riga comune a tutte le eccezioni.
	exception.append(". Si prega di verificare la connessione e di riavviare il programma.");
	//Emetto il segnale relativo alle eccezioni.
	emit SenderException(exception);
}