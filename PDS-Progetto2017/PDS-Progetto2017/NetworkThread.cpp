#include "NetworkThread.h"

//DISTRUTTORE
//Rilascia le risorse contenute nell’oggetto.
NetworkThread::~NetworkThread()
{
	//Faccio il join del thread per assicurarmi che venga distrutto.
	if (thread.joinable())
		thread.join();
	//Azzero i puntatori, facendo così decrementare il punteggio di condivisione.
	mutexPointer = nullptr;
	queuePointer = nullptr;
	condVarPointer = nullptr;
	//Metto un blocco per far vedere ciò che viene scritto in console quando il programma termina.
	std::cout << "Sono stato distrutto, oh no!!!\nScrivi qualsiasi cosa e premi invio per uscire: " << std::endl;
	std::string a;
	std::cin >> a;
};

//Funzione di inizializzazione degli shared pointers.
void NetworkThread::init(const std::shared_ptr<std::mutex>& mutexPointer, const  std::shared_ptr<std::queue<std::string>>& queuePointer, const  std::shared_ptr<std::condition_variable>& condVarPointer)
{
	this->mutexPointer = mutexPointer;
	this->queuePointer = queuePointer;
	this->condVarPointer = condVarPointer;
	std::cout << mutexPointer.get() << "," << this->mutexPointer.get() << "\n"; // now they're the same!
};

//Funzione di avvio del thread sulla classe NetworkThread.
void NetworkThread::start()
{
	//Move assignment
	thread = std::thread(&NetworkThread::waitForData, this);
};

//Metodo di ascolto dei pacchetti udp sulla porta ****.
void NetworkThread::waitForData()
{
	std::cout << "Sono il thread.\n";
	//Definisco un lock di accesso alla risorsa.
	std::unique_lock<std::mutex> uniqueLock(*mutexPointer);
	std::cout << "lock definito. ora attendo\n";
	//Dico al thread di attendere lo sblocco del lock.
	condVarPointer->wait(uniqueLock);
	std::cout << "Ho il lock!\n";
	//Prendo il dato trasmesso dal main.
	std::string stringa = queuePointer->front();
	std::cout << "Ciao, sono un thread e sono in esecuzione.\n";
	std::cout << "Ho ricevuto questo dato: "+stringa;
};