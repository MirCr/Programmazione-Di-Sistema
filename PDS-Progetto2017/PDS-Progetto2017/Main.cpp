#include "NetworkThread.h"

//Mutua esclusione: protegge la coda.
std::shared_ptr<std::mutex> mutexPointer(new std::mutex);
//Queue condivisa.
std::shared_ptr<std::queue<std::string>> queuePointer(new std::queue<std::string>);
//Variabile di condizionamento: indica che la coda non è vuota.
std::shared_ptr<std::condition_variable> condVarPointer(new std::condition_variable);

int main()
{
	//Creo un lock per l'accesso alle risorse
	std::lock_guard<std::mutex> lockGuard(*mutexPointer);
	//Creo un thread per le operazioni web.
	NetworkThread nt;
	nt.init(mutexPointer, queuePointer, condVarPointer);
	//Avvio il thread della classe NetworkThread.
	nt.start();

	//Inserisco un elemento nella queue.
	queuePointer->push("ciao dal thread principale!!!\n");
	//Notifico al thread che ora può andare in esecuzione.
	condVarPointer->notify_all();
	std::cout << "Sono il thread principale, ora dormo\n";
	//Dormo per 3 secondi.
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "Sono il thread principale, ho finito di dormire.\n";

	//Termino il programma.
	return 0;
}