#include "SetUserName.h"

SetUserName::SetUserName(std::shared_ptr<UserData> sharedUserName, QWidget *parent): ui(new Ui::SetUserName)
{
	//Inizializzo il puntatore contenente i dati dell'utente.
	this->sharedUserName = sharedUserName;
	//Rimuovo la barra della finestra.
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//Mostro la grafica della finestra.
	ui->setupUi(this);
	//Se non è vuoto, scrivo il nome utente nella casella. 
	if(sharedUserName->getName()!="")
	{
		ui->SetUserLineEdit->setText(QString::fromStdString(sharedUserName->getName()));
	}
	//Creo le connessioni.
	QObject::connect(ui->SetUserNextButton, SIGNAL(clicked()), this, SLOT(NextButton()));
}

SetUserName::~SetUserName()
{
	//Elimino la ui.
	delete ui;

	//Azzero i puntatori, così da far decrementare il punteggio di condivisione.
	sharedUserName = nullptr;
}

/*Metodo di implementazione del bottone Next.*/
void SetUserName::NextButton()
{
	QString qs = ui->SetUserLineEdit->text();
	//Se la stringa non è vuota consento all'utente di procedere.
	if (qs.isEmpty() == false)
	{
		//Setto il nome dell'utente.
		sharedUserName->setName(qs.toStdString());
		//Avvio il nuovo widget di scelta foto.
		SetUserPhoto suf(sharedUserName);
		suf.exec();
		//Chiudo il widget corrente.
		close();
	}
}