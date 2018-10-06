#pragma once
#include "CommonNetworkImports.h"
#include "CommonImports.h"

class SenderThread : public QThread
{
	Q_OBJECT

	public:
		SenderThread(std::shared_ptr<UserData>& sharedName, std::shared_ptr<bool>& sharedOnlineFlag, QObject *parent = 0);
		~SenderThread();
		void sendMulticastPackets();

	// run() emetterà il segnale startSender().
	signals:
		void startSender();

	//Riscrivo il comportamento del metodo run();
	protected:
		void run() override;

	private:
		//Socket di invio.
		int sendSocket;
		//Thread della classe.
		std::thread thread;
		//Nome dell'utente.
		std::shared_ptr<UserData> sharedUserName;
		//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
		std::shared_ptr<bool> sharedOnlineFlag;
};
