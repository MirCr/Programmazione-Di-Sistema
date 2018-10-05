/********************************************************************************
** Form generated from reading UI file 'MainWin.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWinClass
{
public:
    QAction *actionOnline;
    QAction *actionOffline;
    QAction *actionNome;
    QAction *actionAnonimo;
    QAction *actionAggiungi_foto_profilo;
    QAction *actionVolto_anonimo;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QProgressBar *progressBar;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelBtn;
    QPushButton *shareBtn;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menuBar;
    QMenu *menusettings;
    QMenu *menuOnLine_2;
    QMenu *menuUsername_2;
    QMenu *menuFoto_Profilo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWinClass)
    {
        if (MainWinClass->objectName().isEmpty())
            MainWinClass->setObjectName(QStringLiteral("MainWinClass"));
        MainWinClass->setWindowModality(Qt::NonModal);
        MainWinClass->resize(760, 486);
        actionOnline = new QAction(MainWinClass);
        actionOnline->setObjectName(QStringLiteral("actionOnline"));
        actionOnline->setCheckable(true);
        actionOnline->setChecked(true);
        actionOnline->setMenuRole(QAction::NoRole);
        actionOffline = new QAction(MainWinClass);
        actionOffline->setObjectName(QStringLiteral("actionOffline"));
        actionOffline->setCheckable(true);
        actionNome = new QAction(MainWinClass);
        actionNome->setObjectName(QStringLiteral("actionNome"));
        actionNome->setCheckable(true);
        actionAnonimo = new QAction(MainWinClass);
        actionAnonimo->setObjectName(QStringLiteral("actionAnonimo"));
        actionAnonimo->setCheckable(true);
        actionAnonimo->setChecked(true);
        actionAggiungi_foto_profilo = new QAction(MainWinClass);
        actionAggiungi_foto_profilo->setObjectName(QStringLiteral("actionAggiungi_foto_profilo"));
        actionVolto_anonimo = new QAction(MainWinClass);
        actionVolto_anonimo->setObjectName(QStringLiteral("actionVolto_anonimo"));
        centralWidget = new QWidget(MainWinClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMinimumSize(QSize(668, 291));
        listWidget->setAutoScrollMargin(16);

        horizontalLayout_2->addWidget(listWidget);

        verticalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(verticalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setValue(24);

        verticalLayout_2->addWidget(progressBar);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        cancelBtn = new QPushButton(centralWidget);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));

        horizontalLayout->addWidget(cancelBtn);

        shareBtn = new QPushButton(centralWidget);
        shareBtn->setObjectName(QStringLiteral("shareBtn"));

        horizontalLayout->addWidget(shareBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        MainWinClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWinClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 760, 21));
        menusettings = new QMenu(menuBar);
        menusettings->setObjectName(QStringLiteral("menusettings"));
        menuOnLine_2 = new QMenu(menusettings);
        menuOnLine_2->setObjectName(QStringLiteral("menuOnLine_2"));
        menuOnLine_2->setTearOffEnabled(false);
        menuUsername_2 = new QMenu(menusettings);
        menuUsername_2->setObjectName(QStringLiteral("menuUsername_2"));
        menuFoto_Profilo = new QMenu(menusettings);
        menuFoto_Profilo->setObjectName(QStringLiteral("menuFoto_Profilo"));
        MainWinClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWinClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWinClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWinClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWinClass->setStatusBar(statusBar);

        menuBar->addAction(menusettings->menuAction());
        menusettings->addAction(menuOnLine_2->menuAction());
        menusettings->addAction(menuUsername_2->menuAction());
        menusettings->addAction(menuFoto_Profilo->menuAction());
        menuOnLine_2->addAction(actionOnline);
        menuOnLine_2->addAction(actionOffline);
        menuUsername_2->addAction(actionNome);
        menuUsername_2->addAction(actionAnonimo);
        menuFoto_Profilo->addAction(actionAggiungi_foto_profilo);
        menuFoto_Profilo->addAction(actionVolto_anonimo);

        retranslateUi(MainWinClass);

        QMetaObject::connectSlotsByName(MainWinClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWinClass)
    {
        MainWinClass->setWindowTitle(QApplication::translate("MainWinClass", "MainWin", nullptr));
        actionOnline->setText(QApplication::translate("MainWinClass", "Online", nullptr));
        actionOffline->setText(QApplication::translate("MainWinClass", "Offline", nullptr));
        actionNome->setText(QApplication::translate("MainWinClass", "Nome", nullptr));
        actionAnonimo->setText(QApplication::translate("MainWinClass", "Anonimo", nullptr));
        actionAggiungi_foto_profilo->setText(QApplication::translate("MainWinClass", "Aggiungi foto profilo", nullptr));
        actionVolto_anonimo->setText(QApplication::translate("MainWinClass", "Volto anonimo", nullptr));
        label->setText(QApplication::translate("MainWinClass", "Tempo rimanente:", nullptr));
        cancelBtn->setText(QApplication::translate("MainWinClass", "Annulla", nullptr));
        shareBtn->setText(QApplication::translate("MainWinClass", "Condividi", nullptr));
        menusettings->setTitle(QApplication::translate("MainWinClass", "Opzioni", nullptr));
        menuOnLine_2->setTitle(QApplication::translate("MainWinClass", "Stato", nullptr));
        menuUsername_2->setTitle(QApplication::translate("MainWinClass", "Username", nullptr));
        menuFoto_Profilo->setTitle(QApplication::translate("MainWinClass", "Foto Profilo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWinClass: public Ui_MainWinClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
