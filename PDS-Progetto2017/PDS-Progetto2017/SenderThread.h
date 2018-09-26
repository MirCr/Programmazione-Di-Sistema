#include "CommonNetworkImports.h"
#include "CommonImports.h"

class SenderThread
{
	public:
		SenderThread(const char *name, std::shared_ptr<bool>& shareOnlineFlag);
		~SenderThread();
		/* Metodo di avvio del thread sulla classe SenderThread.*/
		void start();
		/*Metodo per l'invio di pacchetti UDP multicast.*/
		void sendMulticastPackets();
	private:
		//Socket di invio.
		int sendSocket;
		//Thread della classe.
		std::thread thread;
		const char *name;
		//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
		std::shared_ptr<bool> shareOnlineFlag;
};