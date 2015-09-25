/********************************************************************************
** Form generated from reading UI file 'chatclient.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATCLIENT_H
#define UI_CHATCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatClient
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonSend;
    QTextBrowser *textBrowserChat;
    QTextEdit *textEditMsg;
    QPushButton *pushButtonConnect;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QLabel *labelOnlineUsers;

    void setupUi(QMainWindow *ChatClient)
    {
        if (ChatClient->objectName().isEmpty())
            ChatClient->setObjectName(QStringLiteral("ChatClient"));
        ChatClient->resize(452, 352);
        QFont font;
        font.setUnderline(true);
        ChatClient->setFont(font);
        centralWidget = new QWidget(ChatClient);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButtonSend = new QPushButton(centralWidget);
        pushButtonSend->setObjectName(QStringLiteral("pushButtonSend"));
        pushButtonSend->setGeometry(QRect(340, 290, 101, 51));
        QFont font1;
        font1.setUnderline(false);
        pushButtonSend->setFont(font1);
        textBrowserChat = new QTextBrowser(centralWidget);
        textBrowserChat->setObjectName(QStringLiteral("textBrowserChat"));
        textBrowserChat->setGeometry(QRect(10, 10, 321, 271));
        textBrowserChat->setFont(font1);
        textEditMsg = new QTextEdit(centralWidget);
        textEditMsg->setObjectName(QStringLiteral("textEditMsg"));
        textEditMsg->setGeometry(QRect(10, 290, 321, 51));
        textEditMsg->setFont(font1);
        pushButtonConnect = new QPushButton(centralWidget);
        pushButtonConnect->setObjectName(QStringLiteral("pushButtonConnect"));
        pushButtonConnect->setGeometry(QRect(170, 190, 101, 51));
        pushButtonConnect->setFont(font1);
        labelName = new QLabel(centralWidget);
        labelName->setObjectName(QStringLiteral("labelName"));
        labelName->setGeometry(QRect(50, 110, 71, 20));
        QFont font2;
        font2.setPointSize(14);
        font2.setUnderline(false);
        labelName->setFont(font2);
        lineEditName = new QLineEdit(centralWidget);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setGeometry(QRect(160, 100, 191, 31));
        lineEditName->setFont(font1);
        labelOnlineUsers = new QLabel(centralWidget);
        labelOnlineUsers->setObjectName(QStringLiteral("labelOnlineUsers"));
        labelOnlineUsers->setGeometry(QRect(340, 10, 101, 21));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setUnderline(true);
        font3.setWeight(50);
        labelOnlineUsers->setFont(font3);
        labelOnlineUsers->setAlignment(Qt::AlignCenter);
        ChatClient->setCentralWidget(centralWidget);

        retranslateUi(ChatClient);

        QMetaObject::connectSlotsByName(ChatClient);
    } // setupUi

    void retranslateUi(QMainWindow *ChatClient)
    {
        ChatClient->setWindowTitle(QApplication::translate("ChatClient", "ChatClient", 0));
        pushButtonSend->setText(QApplication::translate("ChatClient", "Send", 0));
        pushButtonConnect->setText(QApplication::translate("ChatClient", "Connect", 0));
        labelName->setText(QApplication::translate("ChatClient", "Name: ", 0));
        labelOnlineUsers->setText(QApplication::translate("ChatClient", "Online Users", 0));
    } // retranslateUi

};

namespace Ui {
    class ChatClient: public Ui_ChatClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATCLIENT_H
