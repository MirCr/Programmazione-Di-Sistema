#pragma once
//Qt.
#include <QDialog>
#include <QDebug>
#include <QImageReader>
#include <QImage>
#include "QFileDialog"

//Standard C++.
#include <fstream>
#include <memory>

//Custom classes.
#include "ui_SetUserPhoto.h"
#include "UserData.hpp"

class SetUserPhoto: public QDialog
{
	Q_OBJECT
	public:
		SetUserPhoto(std::shared_ptr<UserData> sharedUserName, QWidget *parent = 0);
		~SetUserPhoto();

	public slots:
		void ChoosePhoto();
		void SaveButton();

	private:
		//Tramite l'oggetto ui posso accedere a tutti gli elementi grafici.
		Ui::SetUserPhoto *ui;

		//Oggetto di tipo UserName contenente i dati dell'utente.
		std::shared_ptr<UserData> sharedUserName;

		//Stringa contenente il percorso della foto.
		std::string photo;
};

