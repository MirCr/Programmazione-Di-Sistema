#include "MainWin.h"
#include "QMessageBox"
#include "QInputDialog"
#include "QDir"
#include "qlabel.h"

MainWin::MainWin(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWinClass)
{
	//permette di visualizzare la GUI " traducendo" il file XML creato da QT Designer in codice C++.
	ui->setupUi(this);
	//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
	sharedOnlineFlag = std::make_shared<bool>();
	//Setto a true il flag in modo da essere online di default.
	*sharedOnlineFlag = true;
	//Creo la lista di utenti connessi in rete.
	sharedUserSet = std::make_shared<std::set<UserData, UserDataComparator>>();
	//Setto il nome utente.
	sharedName = std::make_shared<std::string>();
	std::string name;
	fetchUserData(name);
	//Inizializzo l'oggetto senderThread.
	senderThread = new SenderThread(sharedName, sharedOnlineFlag);
	//Avvio il Sender Thread.
	senderThread->start();
	//Inizializzo l'oggetto receiverThread.
	receiverThread = new ReceiverThread(sharedUserSet);
	//Avvio il Receiver Thread.
	receiverThread->start();
	//Creo le connessioni tra segnali e slots.
	createConnections();
}

MainWin::~MainWin()
{
	delete ui;
}

//Funzione che connette i segnali dei Widget alle funzioni che ne determinano il funzionamento. 
void MainWin::createConnections()
{
	//Main Window.
	QObject::connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(highlightChecked(QListWidgetItem*)));
	QObject::connect(ui->shareBtn, SIGNAL(clicked()), this, SLOT(share()));
	QObject::connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancel()));
	QObject::connect(ui->actionOnline, SIGNAL(triggered()), this, SLOT(setOnline()));
	QObject::connect(ui->actionOffline, SIGNAL(triggered()), this, SLOT(setOffline()));
	QObject::connect(ui->actionNome, SIGNAL(triggered()), this, SLOT(setUsername()));
	QObject::connect(ui->actionAnonimo, SIGNAL(triggered()), this, SLOT(setAnonimo()));
	QObject::connect(receiverThread, SIGNAL(SetUsersListReady()), this, SLOT(createLabels()));
}

//Funzione che inserisce gli utenti nella QlistWidget.
// QstringList è una lista di Stringhe. 
// QlistWidget, è una lista dove ogni tupla può essere una composizione di oggetti.
//Con questa funzione faccio in modo che ogni record sia composto
//da un valore stringa progeniente dalla QStringList, una checkbox, e un immagine. 
void MainWin::createLabels(/*std::shared_ptr<std::set<UserData, UserDataComparator>>& sharedUserSet*/)
{
	QStringList strListNomiUtenti;
	std::string nome;
	//lista di QStringhe che conterrà i nomi degli utenti.
	std::set<QString> mylist;
	//Cancello la lista corrente per fare posto alla nuova lista.
	ui->listWidget->clear();

	qDebug() << "Creo la lista \n";
	for (std::set<UserData, UserDataComparator>::iterator it = sharedUserSet->begin(); it != sharedUserSet->end(); ++it)
	{
		//Prelevo in nome ricevuto come String e lo salvo in un oggetto di tipo QString.
		nome = it->getName();
		//Inserisco in nome QString nella QStringlist da caricare nella QListWidgetItem.
		strListNomiUtenti << QString::fromStdString(nome);
	}	

	//Inserisco la QStringList nella QListWidget.
	ui->listWidget->addItems(strListNomiUtenti);
	//Regolo la dimensione delle immagini di profilo. 
	ui->listWidget->setIconSize(QSize(100, 100));

	//Aggiungo all'oggetto QListWidgeItem una checkbox non spuntata e un'immagine.
	QListWidgetItem* item = 0;
	for (int i = 0; i < ui->listWidget->count(); ++i)
	{
		item = ui->listWidget->item(i);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Unchecked);
		item->setIcon(QIcon("Resources/immagine.png"));		
	}
}

//Funzione che si attiva al click del tasto share.
void MainWin::share()
{

}

//Funzione che si attiva al click del tasto cancel.
void MainWin::cancel()
{	

}

//Funziona che controlla l'evento delle checkBox. Se si spunta un record verrà cambiato il suo colore di sfondo. 
//IDEA: Possiamo usare questo scheletro per selezionare gli utenti da aggiungere alla lista "condividi con". 
void MainWin::highlightChecked(QListWidgetItem *item) 
{
	if (item->checkState() == Qt::Checked) 
	{
		item->setBackgroundColor(QColor("#ffffb2"));	
	}
	else
	{
		item->setBackgroundColor(QColor("#ffffff"));
	}
}

//Slot richiamato da thread1 ogni secondo per 100 volte.FUNZIONE DI TEST. 
void MainWin::avanza(int i= 0)
{
	ui->progressBar->setValue(i);
}

//SetOnline e SetOffline assieme implementano un meccanismo di checking esclusivo che permette di essere online oppure offline.
//Questo meccanismo permette di attivare lo stato antagonista anche nel caso in cui si cerchi di disattivare lo stato attivo corrente.
void MainWin::setOnline()
{	
	if (ui->actionOnline->isChecked())
	{
		ui->actionOffline->setChecked(false);
		*sharedOnlineFlag = true;
	}
	else if (!ui->actionOnline->isChecked() && !ui->actionOffline->isChecked())
	{
		ui->actionOnline->setChecked(true);
		ui->actionOffline->setChecked(false);
		*sharedOnlineFlag = true;
	}
}

void MainWin::setOffline()
{
	if (ui->actionOffline->isChecked())
	{
		ui->actionOnline->setChecked(false);
		*sharedOnlineFlag = false;
	}
	
	else if (!ui->actionOffline->isChecked() && !ui->actionOnline->isChecked())
	{
		ui->actionOffline->setChecked(true);
		ui->actionOnline->setChecked(false);
		*sharedOnlineFlag = false;
	}
}

/*SetUsername e SetAnonimo assieme implementano un meccanismo di checking esclusivo che permette all'utente di usare o il proprio username o in nome "Anonymous".
Questo meccanismo permette di attivare lo stato antagonista anche nel caso in cui si cerchi di disattivare lo stato attivo corrente.*/
void MainWin::setUsername()
{
	if (ui->actionNome->isChecked())
	{
		qDebug()<<"****************************** \n";
		bool ok;
		QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
		if (ok && !text.isEmpty())
		{
			//CAmbio il nome untente con quello iserito dall'utente.
			ui->actionNome->setText(text);
			//Converto da QString a std::string. e la passo allo sharedName.
			*sharedName = text.toLocal8Bit().constData();
		}

		ui->actionAnonimo->setChecked(false);
	}
	else if (!ui->actionNome->isChecked() && !ui->actionAnonimo->isChecked())
	{
		ui->actionNome->setChecked(true);
		ui->actionAnonimo->setChecked(false);
		*sharedName = "Antonio";
	}

}


void MainWin::setAnonimo()
{
	if (ui->actionAnonimo->isChecked())
	{
		ui->actionNome->setChecked(false);
		*sharedName = "Anonymous";
	}

	else if (!ui->actionAnonimo->isChecked() && !ui->actionNome->isChecked())
	{
		ui->actionAnonimo->setChecked(true);
		ui->actionNome->setChecked(false);
		*sharedName = "Anonymous";
	}
}


/*Funzione di lettura dei dati dell'utente da file.*/
void MainWin::fetchUserData(std::string &name)
{
	//Creo un nuovo oggetto di tipo ifstream che punta al file dei dati dell'utente
	std::ifstream userData("data");
	//Se il file esiste entro qui.
	if (userData.good())
	{
		userData >> name;
	}
	//Se invece il file non esiste entro qui.
	else
	{
		SetUserName sun;
		sun.exec();
	}
	userData.close();
}