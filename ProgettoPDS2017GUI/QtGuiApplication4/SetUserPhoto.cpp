#include "SetUserPhoto.h"

SetUserPhoto::SetUserPhoto(std::shared_ptr<UserData> sharedUserName, QWidget *parent): ui(new Ui::SetUserPhoto)
{
	//Inizializzo il puntatore contenente i dati dell'utente.
	this->sharedUserName = sharedUserName;
	//Rimuovo la barra della finestra.
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//Mostro la grafica della finestra.
	ui->setupUi(this);
	//Creo le connessioni.
	QObject::connect(ui->SetPhotoChooseButton, SIGNAL(clicked()), this, SLOT(ChoosePhoto()));
	QObject::connect(ui->SetPhotoSaveButton, SIGNAL(clicked()), this, SLOT(SaveButton()));
}

SetUserPhoto::~SetUserPhoto()
{
	//Elimino la ui.
	delete ui;

	//Azzero i puntatori, così da far decrementare il punteggio di condivisione.
	sharedUserName = nullptr;
}

/*Metodo di scelta foto.*/
void SetUserPhoto::ChoosePhoto()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.bmp *.png *.jpg)"));
	
	//Salvo il percorso del file nella variabile photo.
	photo = filePath.toStdString();

	//Leggo la foto selezionata.
	QImageReader reader(filePath);
	reader.setAutoTransform(true);
	QImage newImage = reader.read();
	//Setto la foto dell'utente.
	sharedUserName->setImage(newImage);

	//Mostro in output la foto.
	ui->UserPhotoImage->setPixmap(QPixmap::fromImage(newImage));
}

/*Metodo di implementazione del bottone Save.*/
void SetUserPhoto::SaveButton()
{
	//Creo un nuovo file.
	std::fstream newUserData;
	newUserData.open("data", std::ostream::out);
	//Se l'utente non ha scelto una foto entro qui.
	if (photo.length() == 0)
	{
		//Imposto come foto dell'utente quella di default.
		sharedUserName->setImage(QImage(":/images/DefaultUser"));
		//Scrivo i dati su file.
		newUserData << sharedUserName->getName() << std::endl << "default";
	}
	//Altrimenti entro qui.
	else
	{
		//Scrivo i dati su file.
		newUserData << sharedUserName->getName() << std::endl << photo;
	}
	//Chiudo il file.
	newUserData.close();

	//Chiudo il widget corrente.
	close();
}
