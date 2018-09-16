#include "CommonNetworkImports.h"
#include "CommonImports.h"

class SenderThread
{
public:
	~SenderThread();
	/* Metodo di avvio del thread sulla classe SenderThread.*/
	void start();
	/*Metodo per l'invio di pacchetti UDP multicast.*/
	void sendMulticastPackets();
private:
	//Thread della classe.
	std::thread thread;
};