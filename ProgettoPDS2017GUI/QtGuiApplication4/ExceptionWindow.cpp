#include "ExceptionWindow.h"

ExceptionWindow::ExceptionWindow(QString exception) : ui(new Ui::ExceptionWindow)
{
	//Rimuovo la barra della finestra.
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//Mostro la grafica della finestra.
	ui->setupUi(this);
	//Aggiorno la label della finestra.
	ui->ExceptionText->setText(exception);
	//Creo le connessioni.
	QObject::connect(ui->ExceptionOkButton, SIGNAL(clicked()), this, SLOT(OkButton()));
}

ExceptionWindow::~ExceptionWindow()
{
	//Elimino la ui.
	delete ui;
}

/*Metodo di implementazione del bottone Ok.*/
void ExceptionWindow::OkButton()
{
	//Chiudo il widget corrente.
	close();
}