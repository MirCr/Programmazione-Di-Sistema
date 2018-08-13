#include "NetworkThread.h"

//Mutua esclusione: protegge la coda.
std::shared_ptr<std::mutex> mutexPointer(new std::mutex);
//Queue condivisa.
std::shared_ptr<std::queue<std::string>> queuePointer(new std::queue<std::string>);
//Variabile di condizionamento: indica che la coda non è vuota.
std::shared_ptr<std::condition_variable> notEmptyCondVar(new std::condition_variable);
//Variabile di condizionamento: indica che la coda è vuota.
std::shared_ptr<std::condition_variable> emptyCondVar(new std::condition_variable);

int main()
{
	//Creo un lock per l'accesso alle risorse
	//std::lock_guard<std::mutex> lockGuard(*mutexPointer);
	std::unique_lock<std::mutex> uniqueLock1(*mutexPointer);
	//Creo un thread per le operazioni web.
	NetworkThread nt;
	nt.init(mutexPointer, queuePointer, notEmptyCondVar, emptyCondVar);
	//Avvio il thread della classe NetworkThread.
	nt.start();
	//Attendo per 3 secondi (Consente di sapere se il thread è effettivamente in attesa).
	std::this_thread::sleep_for(std::chrono::seconds(3));
	//Inserisco un elemento nella queue.
	queuePointer->push("Ciao dal thread principale!!!");
	//Sblocco il mutex.
	uniqueLock1.unlock();
	//Notifico al thread che ora può andare in esecuzione.
	notEmptyCondVar->notify_one();
	//Rilascio il mutex usato.
	uniqueLock1.release();
	//Creo un nuovo mutex per attendere l'esecuzione.
	std::unique_lock<std::mutex> uniqueLock2(*mutexPointer);
	std::cout << "Thread principale: attendo il lock." << std::endl;
	//Dico al thread di attendere lo sblocco del lock.
	emptyCondVar->wait(uniqueLock2);
	std::cout << "Thread principale: Uscita wait e terminazione dell'esecuzione del programma." << std::endl;

	//Termino il programma.
	return 0;
}