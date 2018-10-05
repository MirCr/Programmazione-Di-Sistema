/********************************************************************************
** Form generated from reading UI file 'SetUserName.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUSERNAME_H
#define UI_SETUSERNAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetUserName
{
public:
    QPushButton *SetUserNextButton;
    QLineEdit *SetUserLineEdit;
    QLabel *UserNameLabel;

    void setupUi(QWidget *SetUserName)
    {
        if (SetUserName->objectName().isEmpty())
            SetUserName->setObjectName(QStringLiteral("SetUserName"));
        SetUserName->setWindowModality(Qt::ApplicationModal);
        SetUserName->resize(261, 146);
        SetUserNextButton = new QPushButton(SetUserName);
        SetUserNextButton->setObjectName(QStringLiteral("SetUserNextButton"));
        SetUserNextButton->setGeometry(QRect(90, 100, 81, 23));
        SetUserLineEdit = new QLineEdit(SetUserName);
        SetUserLineEdit->setObjectName(QStringLiteral("SetUserLineEdit"));
        SetUserLineEdit->setGeometry(QRect(60, 60, 141, 20));
        UserNameLabel = new QLabel(SetUserName);
        UserNameLabel->setObjectName(QStringLiteral("UserNameLabel"));
        UserNameLabel->setGeometry(QRect(20, 9, 221, 41));
        UserNameLabel->setScaledContents(false);
        UserNameLabel->setAlignment(Qt::AlignCenter);
        UserNameLabel->setWordWrap(true);

        retranslateUi(SetUserName);

        QMetaObject::connectSlotsByName(SetUserName);
    } // setupUi

    void retranslateUi(QWidget *SetUserName)
    {
        SetUserName->setWindowTitle(QApplication::translate("SetUserName", "Form", nullptr));
        SetUserNextButton->setText(QApplication::translate("SetUserName", "Next", nullptr));
        UserNameLabel->setText(QApplication::translate("SetUserName", "Hi new user! Please enter the name by which other users of the network will identify you:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SetUserName: public Ui_SetUserName {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUSERNAME_H
