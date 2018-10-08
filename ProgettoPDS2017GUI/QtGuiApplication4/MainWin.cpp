#include "MainWin.h"
#include "QMessageBox"
#include "QInputDialog"
#include "QDir"
#include "qlabel.h"

MainWin::MainWin(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWinClass)
{
	//Permette di visualizzare la GUI " traducendo" il file XML creato da QT Designer in codice C++.
	ui->setupUi(this);
	//Chiamo la funzione di istanziazione delle variabili condivise.
	InstantiateSharedVariables();
	//Prelevo il nome dell'utente.
	FetchUserData();
	//Inizializzo l'oggetto senderThread.
	senderThread = new SenderThread(sharedUserName, sharedOnlineFlag);
	//Avvio il Sender Thread.
	senderThread->start();
	//Inizializzo l'oggetto receiverThread.
	receiverThread = new ReceiverThread(sharedUsersSet);
	//Avvio il Receiver Thread.
	receiverThread->start();
	//Creo le connessioni tra segnali e slots.
	CreateConnections();
}

MainWin::~MainWin()
{
	//Elimino la ui.
	delete ui;

	//Azzero i puntatori, così da far decrementare il punteggio di condivisione.
	sharedUserName = nullptr;
	sharedUsersSet = nullptr;
	sharedOnlineFlag = nullptr;
}

/*Funzione di istanziazione delle variabili condivise.*/
void MainWin::InstantiateSharedVariables()
{
	//Variabile contenente i dati dell'utente.
	sharedUserName = std::make_shared<UserData>("","127.0.0.1");
	//Queue condivisa, contenente la lista degli utenti presenti nella rete locale.
	sharedUsersSet = std::make_shared<std::set<UserData, UserDataComparator>>();
	//Variabile che consente di passare dallo stato online allo stato offline e viceversa. True = online.
	sharedOnlineFlag = std::make_shared<bool>();
	//Setto a true il flag in modo da essere online di default.
	*sharedOnlineFlag = true;
}

/*Funzione che connette i segnali dei Widget alle funzioni che ne determinano il funzionamento. */
void MainWin::CreateConnections()
{
	//Main Window.
	QObject::connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(HighlightChecked(QListWidgetItem*)));
	QObject::connect(ui->shareBtn, SIGNAL(clicked()), this, SLOT(Share()));
	QObject::connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(Cancel()));
	QObject::connect(ui->actionOnline, SIGNAL(triggered()), this, SLOT(SetOnline()));
	QObject::connect(ui->actionOffline, SIGNAL(triggered()), this, SLOT(SetOffline()));
	QObject::connect(ui->actionNome, SIGNAL(triggered()), this, SLOT(SetUsername()));
	QObject::connect(ui->actionAnonimo, SIGNAL(triggered()), this, SLOT(SetAnonimo()));
	//Segnale di avvenuto aggiornamento della lista.
	QObject::connect(receiverThread, SIGNAL(SetUsersListReady()), this, SLOT(CreateLabels()));
	//Segnali di eccezione avvenuta nei thread di invio e ricezione messaggi UDP multicast.
	QObject::connect(receiverThread, SIGNAL(ReceiverException(QString)), this, SLOT(ExceptionManager(QString)));
	QObject::connect(senderThread, SIGNAL(SenderException(QString)), this, SLOT(ExceptionManager(QString)));
}

/*Funzione che inserisce gli utenti nella QlistWidget.
  QstringList è una lista di Stringhe. 
  QlistWidget, è una lista dove ogni tupla può essere una composizione di oggetti.
  Con questa funzione faccio in modo che ogni record sia composto
  da un valore stringa progeniente dalla QStringList, una checkbox, e un immagine. */
void MainWin::CreateLabels()
{
	QStringList strListNomiUtenti;
	std::string nome;
	//lista di QStringhe che conterrà i nomi degli utenti.
	std::set<QString> mylist;
	//Cancello la lista corrente per fare posto alla nuova lista.
	ui->listWidget->clear();

	qDebug() << "Creo la lista \n";
	for (std::set<UserData, UserDataComparator>::iterator it = sharedUsersSet->begin(); it != sharedUsersSet->end(); ++it)
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

/*Funzione che si attiva al click del tasto share.*/
void MainWin::Share()
{

}

/*Funzione che si attiva al click del tasto cancel.*/
void MainWin::Cancel()
{	

}

/*Funziona che controlla l'evento delle checkBox. Se si spunta un record verrà cambiato il suo colore di sfondo. 
  IDEA: Possiamo usare questo scheletro per selezionare gli utenti da aggiungere alla lista "condividi con".*/
void MainWin::HighlightChecked(QListWidgetItem *item) 
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

/*Slot richiamato da thread1 ogni secondo per 100 volte.FUNZIONE DI TEST.*/
void MainWin::Avanza(int i= 0)
{
	ui->progressBar->setValue(i);
}

//SetOnline e SetOffline assieme implementano un meccanismo di checking esclusivo che permette di essere online oppure offline.
//Questo meccanismo permette di attivare lo stato antagonista anche nel caso in cui si cerchi di disattivare lo stato attivo corrente.
void MainWin::SetOnline()
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

void MainWin::SetOffline()
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
void MainWin::SetUsername()
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
			sharedUserName->setName(text.toLocal8Bit().constData());
		}

		ui->actionAnonimo->setChecked(false);
	}
	else if (!ui->actionNome->isChecked() && !ui->actionAnonimo->isChecked())
	{
		ui->actionNome->setChecked(true);
		ui->actionAnonimo->setChecked(false);
		sharedUserName->setName("Antonio");
	}

}

/*DA ELIMINARE*/
void MainWin::SetAnonimo()
{
	if (ui->actionAnonimo->isChecked())
	{
		ui->actionNome->setChecked(false);
		sharedUserName->setName("Anonymous");
	}

	else if (!ui->actionAnonimo->isChecked() && !ui->actionNome->isChecked())
	{
		ui->actionAnonimo->setChecked(true);
		ui->actionNome->setChecked(false);
		sharedUserName->setName("Anonymous");
	}
}


/*Metodo di lettura dei dati dell'utente da file.*/
void MainWin::FetchUserData()
{
	//Creo un nuovo oggetto di tipo ifstream che punta al file dei dati dell'utente.
	std::ifstream userData("data");
	//Se il file esiste entro qui.
	if (userData.good())
	{
		std::string data;
		//Leggo e setto il nome dell'utente.
		userData >> data;
		sharedUserName->setName(data);
		//Leggo e setto la foto dell'utente.
		userData >> data;
		//Se la foto è quella di default la imposto dalle risorse del programma.
		if (data == "default")
		{
			sharedUserName->setImage(QImage(":/images/DefaultUser"));
		}
		//Altrimenti leggo e setto la foto da file.
		else
		{
			//Creo un nuovo oggetto di tipo ifstream che punta alla foto dell'utente.
			std::ifstream photo(data);
			//Se la foto esiste entro qui.
			if (photo.good())
			{
				//Leggo la foto selezionata.
				QImageReader reader(QString::fromStdString(data));
				reader.setAutoTransform(true);
				QImage newImage = reader.read();
				//Setto la foto dell'utente.
				sharedUserName->setImage(newImage);
			}
			//Altrimenti imposto la foto di default.
			else
			{
				sharedUserName->setImage(QImage(":/images/DefaultUser"));
			}
		}
	}
	//Se invece il file non esiste entro qui.
	else
	{
		SetUserName sun(sharedUserName);
		sun.exec();
	}
	userData.close();
}

/*Metodo di gestione delle eccezioni.*/
void MainWin::ExceptionManager(QString exception)
{
	ExceptionWindow ew(exception);
	ew.exec();
	QApplication::quit();
}