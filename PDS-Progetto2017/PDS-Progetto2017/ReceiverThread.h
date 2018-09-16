#include "CommonNetworkImports.h"
#include "CommonImports.h"

class ReceiverThread
{
public:
	ReceiverThread(std::shared_ptr<std::mutex>& mutexPointer, std::shared_ptr<std::queue<std::string>>& ipQueuePointer, std::shared_ptr<std::condition_variable>& mainReceiverCondVar, std::shared_ptr<bool>& mainReceiverFlag);
	~ReceiverThread();
	/* Metodo di avvio del thread sulla classe ReceiverThread.*/
	void start();
	/* Metodo per l'ascolto di pacchetti UDP multicast.*/
	void listenForMulticastData();
private:
	//Thread della classe.
	std::thread thread;
	//Variabile di mutua esclusione.
	std::shared_ptr<std::mutex> mutexPointer;
	//Queue condivisa, contenente la lista degli ip degli utenti presenti nella rete locale.
	std::shared_ptr<std::queue<std::string>> ipQueuePointer;
	//Variabile di condizionamento per il colloquio tra receiver e main.
	std::shared_ptr<std::condition_variable> mainReceiverCondVar;
	//Variabile che consente di evitare notifiche spurie nel main durante il colloquio con il receiver.
	std::shared_ptr<bool> mainReceiverFlag;
};