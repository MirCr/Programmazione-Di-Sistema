#pragma once
//Qt.
#include <QDialog>
#include <QDebug>


//Custom classes.
#include "ui_ExceptionWindow.h"

class ExceptionWindow : public QDialog
{
	Q_OBJECT

	public:
		ExceptionWindow(QString);
		~ExceptionWindow();

	public slots:
		void OkButton();

	private:
		//Tramite l'oggetto ui posso accedere a tutti gli elementi grafici.
		Ui::ExceptionWindow *ui;
};