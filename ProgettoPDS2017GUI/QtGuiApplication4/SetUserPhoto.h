#pragma once
#include <QDialog>
#include <QDebug>
#include <QImageReader>
#include <QImage>
#include "QFileDialog"
#include <fstream>
#include "ui_SetUserPhoto.h"

class SetUserPhoto: public QDialog
{
	Q_OBJECT
	public:
		SetUserPhoto(std::string name, QWidget *parent = 0);
		~SetUserPhoto();

	public slots:
		void choosePhoto();
		void nextButton();

	private:
		Ui::SetUserPhoto *ui;
		std::string name;
		std::string photo;
};

