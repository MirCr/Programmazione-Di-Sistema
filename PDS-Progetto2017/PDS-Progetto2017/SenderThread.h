#include "CommonNetworkImports.h"
#include "CommonImports.h"

class SenderThread
{
	public:
		SenderThread(const char *name);
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
};