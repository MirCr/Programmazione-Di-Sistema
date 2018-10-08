#pragma once
//Qt.
#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <QDebug>

//Standard C++.
#include <condition_variable>
#include <mutex>
#include <set>
#include <string>
#include <fstream>

//Custom classes.
#include "ui_MainWin.h"
#include "SetUserName.h"
#include "SetUserPhoto.h"
#include "ExceptionWindow.h"

//Network threads.
#include "SenderThread.h"
#include "ReceiverThread.h"

class MainWin : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWin(QWidget *parent = 0);
		~MainWin();
		void InstantiateSharedVariables();

	public slots:
		void CreateConnections();
		void CreateLabels();
		void Share();
		void Cancel();
		void HighlightChecked(QListWidgetItem *item);
		void Avanza(int); //TEST FUNCTION.
		void SetOnline();
		void SetOffline();
		void SetUsername();
		void SetAnonimo();
		void FetchUserData();
		void ExceptionManager(QString exception);

	private:
		//Tramite l'oggetto ui posso accedere a tutti gli elementi grafici.
		Ui::MainWinClass *ui;

		//Thread di l'invio di messaggi multicast.
		SenderThread *senderThread;
		//Thread di ricezione dei messaggi multicast.
		ReceiverThread *receiverThread;

		//Oggetto di tipo UserName contenente i dati dell'utente.
		std::shared_ptr<UserData> sharedUserName;
		//Queue condivisa, contenente la lista degli utenti presenti nella rete locale.
		std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUsersSet;
		//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
		std::shared_ptr<bool> sharedOnlineFlag;
};
