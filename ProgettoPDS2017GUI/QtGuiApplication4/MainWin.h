#pragma once
//Qt.
#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <QDebug>

//Standard C++.
#include <set>
#include <string>
#include <fstream>

//Custom classes.
#include "ui_MainWin.h"
#include "SetUserName.h"
#include "SetUserPhoto.h"

//Network threads.
#include "SenderThread.h"
#include "ReceiverThread.h"

class MainWin : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWin(QWidget *parent = 0);
		~MainWin();

	public slots:
		void createConnections();
		void createLabels();
		void share();
		void cancel();
		void highlightChecked(QListWidgetItem *item);
		void avanza(int); //TEST FUNCTION.
		void setOnline();
		void setOffline();
		void setUsername();
		void setAnonimo();
		void fetchUserData(std::string &name);

	private:
		// Tramite l'oggetto ui posso accedere a tutti gli elementi grafici.
		Ui::MainWinClass *ui;
		SenderThread *senderThread;
		ReceiverThread *receiverThread;
		std::shared_ptr<bool> sharedOnlineFlag;
		std::shared_ptr<std::string> sharedName;
		std::shared_ptr<std::set<UserData, UserDataComparator>> sharedUserSet;
};
