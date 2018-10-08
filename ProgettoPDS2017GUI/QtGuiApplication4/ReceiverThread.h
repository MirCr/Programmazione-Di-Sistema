#pragma once
#include "CommonNetworkImports.h"
#include "CommonImports.h"

class ReceiverThread : public QThread
{
	Q_OBJECT

	public:
		ReceiverThread(std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet, QObject *parent = 0);
		~ReceiverThread();
		/*Metodo di emissione del segnale di eccezione.*/
		void EmitException(QString exception);

	signals:
		//Segnale emesso ogni volta che viene aggiunto un nuovo oggetto al set.
		void SetUsersListReady();
		//Segnale emesso in caso di errore.
		void ReceiverException(QString exception);

		//Riscrivo il comportamento del metodo run();
	protected:
		void run() override;

	private:
		//Socket di ascolto.
		int listenSocket;
		//Queue condivisa, contenente la lista degli ip degli utenti presenti nella rete locale.
		std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUserSet;
};