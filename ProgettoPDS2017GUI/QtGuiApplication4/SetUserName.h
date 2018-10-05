#pragma once
#include <QDialog>
#include <QDebug>
#include "ui_SetUserName.h"
#include "SetUserPhoto.h"

class SetUserName: public QDialog
{
	Q_OBJECT
	public:
		SetUserName(QWidget *parent = 0);
		~SetUserName();

	public slots:
		void nextButton();

	private:
		Ui::SetUserName *ui;
};

