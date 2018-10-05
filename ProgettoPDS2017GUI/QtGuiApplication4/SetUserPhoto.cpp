#include "SetUserPhoto.h"

SetUserPhoto::SetUserPhoto(std::string name, QWidget *parent): ui(new Ui::SetUserPhoto)
{
	//Memorizzo il nome dell'utente.
	this->name = name;
	//Rimuovo la barra della finestra.
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//Mostro la grafica della finestra.
	ui->setupUi(this);
	//Creo le connessioni.
	QObject::connect(ui->SetPhotoChooseButton, SIGNAL(clicked()), this, SLOT(choosePhoto()));
	QObject::connect(ui->SetPhotoNextButton, SIGNAL(clicked()), this, SLOT(nextButton()));
}

SetUserPhoto::~SetUserPhoto()
{
	delete ui;
}

/*Metodo di scelta foto.*/
void SetUserPhoto::choosePhoto()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.bmp *.png *.jpg)"));
	//Salvo il percorso del file nella variabile photo.
	photo = filePath.toStdString();
	//Leggo la foto selezionata.
	QImageReader reader(filePath);
	reader.setAutoTransform(true);
	QImage newImage = reader.read();
	//Mostro in output la foto.
	ui->UserPhotoImage->setPixmap(QPixmap::fromImage(newImage));
}

/*Metodo di implementazione del bottone Next.*/
void SetUserPhoto::nextButton()
{
	//Creo un nuovo file.
	std::fstream newUserData;
	newUserData.open("data", std::ostream::out);
	//Se l'utente non ha scelto una foto entro qui.
	if (photo.length() == 0)
	{
		newUserData << name << std::endl << "default";
	}
	//Altrimenti entro qui
	else
	{
		newUserData << name << std::endl << photo;
	}
	//Chiudo il file.
	newUserData.close();

	//Chiudo il widget.
	close();
}
