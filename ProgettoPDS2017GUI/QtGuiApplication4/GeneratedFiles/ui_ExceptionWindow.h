/********************************************************************************
** Form generated from reading UI file 'ExceptionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXCEPTIONWINDOW_H
#define UI_EXCEPTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExceptionWindow
{
public:
    QPushButton *ExceptionOkButton;
    QLabel *ExceptionText;
    QLabel *ExceptionImage;

    void setupUi(QWidget *ExceptionWindow)
    {
        if (ExceptionWindow->objectName().isEmpty())
            ExceptionWindow->setObjectName(QStringLiteral("ExceptionWindow"));
        ExceptionWindow->resize(350, 151);
        ExceptionOkButton = new QPushButton(ExceptionWindow);
        ExceptionOkButton->setObjectName(QStringLiteral("ExceptionOkButton"));
        ExceptionOkButton->setGeometry(QRect(140, 120, 75, 23));
        QFont font;
        font.setPointSize(10);
        ExceptionOkButton->setFont(font);
        ExceptionText = new QLabel(ExceptionWindow);
        ExceptionText->setObjectName(QStringLiteral("ExceptionText"));
        ExceptionText->setGeometry(QRect(80, 20, 241, 81));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ExceptionText->sizePolicy().hasHeightForWidth());
        ExceptionText->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(10);
        font1.setKerning(true);
        ExceptionText->setFont(font1);
        ExceptionText->setTextFormat(Qt::AutoText);
        ExceptionText->setScaledContents(false);
        ExceptionText->setAlignment(Qt::AlignCenter);
        ExceptionText->setWordWrap(true);
        ExceptionImage = new QLabel(ExceptionWindow);
        ExceptionImage->setObjectName(QStringLiteral("ExceptionImage"));
        ExceptionImage->setGeometry(QRect(10, 20, 61, 81));
        ExceptionImage->setPixmap(QPixmap(QString::fromUtf8(":/images/Exception")));
        ExceptionImage->setScaledContents(true);
        ExceptionImage->setWordWrap(false);

        retranslateUi(ExceptionWindow);

        QMetaObject::connectSlotsByName(ExceptionWindow);
    } // setupUi

    void retranslateUi(QWidget *ExceptionWindow)
    {
        ExceptionWindow->setWindowTitle(QApplication::translate("ExceptionWindow", "Form", nullptr));
        ExceptionOkButton->setText(QApplication::translate("ExceptionWindow", "Ok", nullptr));
        ExceptionText->setText(QString());
        ExceptionImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ExceptionWindow: public Ui_ExceptionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXCEPTIONWINDOW_H
