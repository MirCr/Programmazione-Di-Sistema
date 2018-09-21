#include "CommonNetworkImports.h"
#include "CommonImports.h"

class ReceiverThread
{
	public:
		ReceiverThread(std::shared_ptr<std::mutex>& sharedMainRecvMutex, std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet, std::shared_ptr<std::condition_variable>& sharedMainRecvCondVar, std::shared_ptr<bool>& sharedMainRecvFlag);
		~ReceiverThread();
		/* Metodo di avvio del thread sulla classe ReceiverThread.*/
		void start();
		/* Metodo per l'ascolto di pacchetti UDP multicast.*/
		void listenForMulticastData();
		/*Metodo per prelevare la lista di ip del PC corrente.*/
		std::list <std::string> getHostIPs();
	private:
		//Socket di ascolto.
		int listenSocket;
		//Thread della classe.
		std::thread thread;
		//Variabile di mutua esclusione.
		std::shared_ptr<std::mutex> sharedMainRecvMutex;
		//Queue condivisa, contenente la lista degli ip degli utenti presenti nella rete locale.
		std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUserSet;
		//Variabile di condizionamento per il colloquio tra receiver e main.
		std::shared_ptr<std::condition_variable> sharedMainRecvCondVar;
		//Variabile che consente di evitare notifiche spurie nel main durante il colloquio con il receiver.
		std::shared_ptr<bool> sharedMainRecvFlag;
};