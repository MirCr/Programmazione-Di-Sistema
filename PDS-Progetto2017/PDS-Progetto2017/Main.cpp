#include "ReceiverThread.h"
#include "SenderThread.h"

int main()
{
	//Variabile di mutua esclusione.
	std::shared_ptr<std::mutex> sharedMainRecvMutex = std::make_shared<std::mutex>();
	//Queue condivisa, contenente la lista degli utenti presenti nella rete locale.
	std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUserSet = std::make_shared<std::set<UserData, UserDataComparator>>();
	//Variabile di condizionamento per il colloquio tra receiver e main.
	std::shared_ptr<std::condition_variable> sharedMainRecvCondVar = std::make_shared<std::condition_variable>();
	//Variabile che consente di evitare notifiche spurie nel main durante il colloquio con il receiver.
	std::shared_ptr<bool> sharedMainRecvFlag = std::make_shared<bool>();

	//Creo un thread di ricezione dei messaggi multicast.
	ReceiverThread rt(sharedMainRecvMutex, sharedUserSet, sharedMainRecvCondVar, sharedMainRecvFlag);
	//Avvio il thread per la ricezione dei messaggi multicast.
	rt.start();
	//Creo un thread di l'invio di messaggi multicast.
	SenderThread st("Antonio");
	//Avvio il thread per l'invio di messaggi multicast.
	st.start();

	//Setto a false il flag per le notifiche spurie.
	*sharedMainRecvFlag = false;

	//TODO: ciclo infinito: vedere se tenerlo o meno.
	while (1)
	{
		//Creo un lock per accedere ai dati della queue.
		std::unique_lock<std::mutex> uniqueLock(*sharedMainRecvMutex);
		//Protezione contro notifiche spurie.
		while (*sharedMainRecvFlag == false)
		{  		
			//Attendo lo sblocco del lock.
			sharedMainRecvCondVar->wait(uniqueLock);
		}

		int i = 0;
		for (std::set<UserData, UserDataComparator>::iterator it= sharedUserSet->begin(); it != sharedUserSet->end(); it++)
		{
			std::cout << "Thread principale: Elemento " << i << ": " << it->getIP() << std::endl;
		}
		//Prendo il dato presente nella queue e lo salvo in una stringa.
		//Riabilito il flag per evitare notifiche spurie.
		*sharedMainRecvFlag = false;
		//Sblocco il mutex.
		uniqueLock.unlock();
	}

	//Termino il programma.
	std::cout << "Thread principale: Uscita wait e terminazione dell'esecuzione del programma." << std::endl;

	//Pulitura degli shared pointers.
	sharedMainRecvMutex = nullptr;
	sharedUserSet = nullptr;
	sharedMainRecvCondVar = nullptr;
	sharedMainRecvFlag = nullptr;

	return 0;
}