#include "ReceiverThread.h"
#include "SenderThread.h"

int main()
{
	//Variabile di mutua esclusione.
	std::shared_ptr<std::mutex> mutexPointer = std::make_shared<std::mutex>();
	//Queue condivisa, contenente la lista degli ip degli utenti presenti nella rete locale.
	std::shared_ptr<std::queue<std::string>> queuePointer = std::make_shared<std::queue<std::string>>();
	//Variabile di condizionamento per il colloquio tra receiver e main.
	std::shared_ptr<std::condition_variable> mainReceiverCondVar = std::make_shared<std::condition_variable>();
	//Variabile che consente di evitare notifiche spurie nel main durante il colloquio con il receiver.
	std::shared_ptr<bool> mainReceiverFlag = std::make_shared<bool>();

	//Creo un thread di ricezione dei messaggi multicast.
	ReceiverThread rt(mutexPointer, queuePointer, mainReceiverCondVar, mainReceiverFlag);
	//Avvio il thread per la ricezione dei messaggi multicast.
	rt.start();
	//Creo un thread di l'invio di messaggi multicast.
	SenderThread st;
	//Avvio il thread per l'invio di messaggi multicast.
	st.start();

	//Setto a false il flag per le notifiche spurie.
	*mainReceiverFlag = false;

	//TODO: ciclo infinito: vedere se tenerlo o meno.
	while (1)
	{
		//Creo un lock per accedere ai dati della queue.
		std::unique_lock<std::mutex> uniqueLock(*mutexPointer);
		//Protezione contro notifiche spurie.
		while (*mainReceiverFlag == false)
		{  		
			//Attendo lo sblocco del lock.
			mainReceiverCondVar->wait(uniqueLock);
		}
		//Prendo il dato presente nella queue e lo salvo in una stringa.
		std::string stringa = queuePointer->front();
		//Rimuovo il dato dalla queue e stampo a schermo il valore ricevuto.
		queuePointer->pop();
		std::cout << "Thread principale: ho ricevuto questo dato: " << stringa << std::endl;
		//Riabilito il flag per evitare notifiche spurie.
		*mainReceiverFlag = false;
		//Sblocco il mutex.
		uniqueLock.unlock();
	}

	//Termino il programma.
	std::cout << "Thread principale: Uscita wait e terminazione dell'esecuzione del programma." << std::endl;

	//Pulitura degli shared pointers.
	mutexPointer = nullptr;
	queuePointer = nullptr;
	mainReceiverCondVar = nullptr;
	mainReceiverFlag = nullptr;

	return 0;
}