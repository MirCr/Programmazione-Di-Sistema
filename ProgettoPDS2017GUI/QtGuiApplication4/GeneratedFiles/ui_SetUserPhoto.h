/********************************************************************************
** Form generated from reading UI file 'SetUserPhoto.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUSERPHOTO_H
#define UI_SETUSERPHOTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetUserPhoto
{
public:
    QPushButton *SetPhotoSaveButton;
    QLabel *UserPhotoLabel;
    QPushButton *SetPhotoChooseButton;
    QLabel *UserPhotoImage;

    void setupUi(QWidget *SetUserPhoto)
    {
        if (SetUserPhoto->objectName().isEmpty())
            SetUserPhoto->setObjectName(QStringLiteral("SetUserPhoto"));
        SetUserPhoto->setWindowModality(Qt::NonModal);
        SetUserPhoto->resize(261, 271);
        SetPhotoSaveButton = new QPushButton(SetUserPhoto);
        SetPhotoSaveButton->setObjectName(QStringLiteral("SetPhotoSaveButton"));
        SetPhotoSaveButton->setGeometry(QRect(140, 220, 91, 31));
        QFont font;
        font.setPointSize(10);
        SetPhotoSaveButton->setFont(font);
        UserPhotoLabel = new QLabel(SetUserPhoto);
        UserPhotoLabel->setObjectName(QStringLiteral("UserPhotoLabel"));
        UserPhotoLabel->setGeometry(QRect(20, 9, 221, 51));
        UserPhotoLabel->setFont(font);
        UserPhotoLabel->setScaledContents(false);
        UserPhotoLabel->setAlignment(Qt::AlignCenter);
        UserPhotoLabel->setWordWrap(true);
        SetPhotoChooseButton = new QPushButton(SetUserPhoto);
        SetPhotoChooseButton->setObjectName(QStringLiteral("SetPhotoChooseButton"));
        SetPhotoChooseButton->setGeometry(QRect(30, 220, 91, 31));
        SetPhotoChooseButton->setFont(font);
        UserPhotoImage = new QLabel(SetUserPhoto);
        UserPhotoImage->setObjectName(QStringLiteral("UserPhotoImage"));
        UserPhotoImage->setGeometry(QRect(60, 70, 141, 141));
        UserPhotoImage->setPixmap(QPixmap(QString::fromUtf8(":/images/DefaultUser")));
        UserPhotoImage->setScaledContents(true);

        retranslateUi(SetUserPhoto);

        QMetaObject::connectSlotsByName(SetUserPhoto);
    } // setupUi

    void retranslateUi(QWidget *SetUserPhoto)
    {
        SetUserPhoto->setWindowTitle(QApplication::translate("SetUserPhoto", "Form", nullptr));
        SetPhotoSaveButton->setText(QApplication::translate("SetUserPhoto", "Save Changes", nullptr));
        UserPhotoLabel->setText(QApplication::translate("SetUserPhoto", "Now choose the profile photo by which other users of the network will identify you:", nullptr));
        SetPhotoChooseButton->setText(QApplication::translate("SetUserPhoto", "Choose photo", nullptr));
        UserPhotoImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SetUserPhoto: public Ui_SetUserPhoto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUSERPHOTO_H
