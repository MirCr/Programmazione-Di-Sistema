#pragma once
//Qt.
#include <QDialog>
#include <QDebug>

//Standard C++.
#include <fstream>
#include <memory>

//Custom classes.
#include "ui_SetUserName.h"
#include "SetUserPhoto.h"
#include "UserData.hpp"

class SetUserName: public QDialog
{
	Q_OBJECT
	public:
		SetUserName(std::shared_ptr<UserData> sharedUserName, QWidget *parent = 0);
		~SetUserName();

	public slots:
		void NextButton();

	private:
		//Tramite l'oggetto ui posso accedere a tutti gli elementi grafici.
		Ui::SetUserName *ui;

		//Oggetto di tipo UserName contenente i dati dell'utente.
		std::shared_ptr<UserData> sharedUserName;
};

