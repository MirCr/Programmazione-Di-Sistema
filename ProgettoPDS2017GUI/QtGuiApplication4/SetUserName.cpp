#include "SetUserName.h"

SetUserName::SetUserName(QWidget *parent): ui(new Ui::SetUserName)
{
	//Rimuovo la barra della finestra.
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//Mostro la grafica della finestra.
	ui->setupUi(this);
	//Creo le connessioni.
	QObject::connect(ui->SetUserNextButton, SIGNAL(clicked()), this, SLOT(nextButton()));
}

SetUserName::~SetUserName()
{
	delete ui;
}

/*Metodo di implementazione del bottone Next.*/
void SetUserName::nextButton()
{
	QString qs = ui->SetUserLineEdit->text();
	//Se la stringa non è vuota consento all'utente di procedere.
	if (qs.isEmpty() == false)
	{
		SetUserPhoto suf(qs.toStdString());
		suf.exec();
		//Chiudo il widget.
		close();
	}
}