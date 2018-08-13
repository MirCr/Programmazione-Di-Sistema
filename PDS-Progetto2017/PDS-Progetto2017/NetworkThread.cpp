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
	notEmptyCondVar = nullptr;
	emptyCondVar = nullptr;
	//Metto un blocco per far vedere ciò che viene scritto in console quando il programma termina.
	std::cout << "Thread secondario: sono stato distrutto, oh no!!!\nScrivi qualsiasi cosa e premi invio per uscire: " << std::endl;
	std::string a;
	std::cin >> a;
};

//Funzione di inizializzazione degli shared pointers.
void NetworkThread::init(const std::shared_ptr<std::mutex>& mutexPointer, const  std::shared_ptr<std::queue<std::string>>& queuePointer, const  std::shared_ptr<std::condition_variable>& notEmptyCondVar, const  std::shared_ptr<std::condition_variable>& emptyCondVar)
{
	this->mutexPointer = mutexPointer;
	this->queuePointer = queuePointer;
	this->notEmptyCondVar = notEmptyCondVar;
	this->emptyCondVar = emptyCondVar;
	//std::cout << mutexPointer.get() << "," << this->mutexPointer.get() << "\n"; // now they're the same!
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
	std::cout << "Thread secondario: sono in esecuzione." << std::endl;
	//Definisco un lock di accesso alla risorsa.
	std::unique_lock<std::mutex> uniqueLock(*mutexPointer);
	std::cout << "Thread secondario: lock definito. ora attendo che venga inserito un dato." << std::endl;
	//Dico al thread di attendere lo sblocco del lock.
	notEmptyCondVar->wait(uniqueLock, [this]() {return queuePointer->size() > 0; });
	//Prendo il dato trasmesso dal main.
	std::string stringa = queuePointer->front();
	std::cout << "Thread secondario: ho ricevuto questo dato: "+stringa << std::endl;
	//Sblocco il mutex.
	uniqueLock.unlock();
	//Notifico al thread che ora può andare in esecuzione.
	emptyCondVar->notify_one();
	std::cout << "Thread secondario: ho notificato al thread principale che ora puo' continuare l'esecuzione." << std::endl;
};