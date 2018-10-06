#pragma once
#include "CommonNetworkImports.h"
#include "CommonImports.h"

class ReceiverThread : public QThread
{
	Q_OBJECT

public:
	ReceiverThread(std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet, QObject *parent = 0);
	~ReceiverThread();
	void listenForMulticastData();
	std::set <std::string> ReceiverThread::getHostIPs();

	// run() emetterà il segnale SetUsersListReady() ogni volta che ha una lista pronta e la passerà al metodo che aggiorna la WidgetList.
signals:
	void SetUsersListReady();

	//Riscrivo il comportamento del metodo run();
protected:
	void run() override;

private:
	//Socket di ascolto.
	int listenSocket;
	//Queue condivisa, contenente la lista degli ip degli utenti presenti nella rete locale.
	std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUserSet;
};