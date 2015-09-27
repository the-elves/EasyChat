/********************************************************************************
** Form generated from reading UI file 'unicast.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNICAST_H
#define UI_UNICAST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Unicast
{
public:
    QPushButton *pushButtonSend;
    QTextBrowser *textBrowserChat;
    QTextEdit *textEditMsg;

    void setupUi(QWidget *Unicast)
    {
        if (Unicast->objectName().isEmpty())
            Unicast->setObjectName(QStringLiteral("Unicast"));
        Unicast->resize(448, 350);
        pushButtonSend = new QPushButton(Unicast);
        pushButtonSend->setObjectName(QStringLiteral("pushButtonSend"));
        pushButtonSend->setGeometry(QRect(340, 290, 101, 51));
        QFont font;
        font.setUnderline(false);
        pushButtonSend->setFont(font);
        textBrowserChat = new QTextBrowser(Unicast);
        textBrowserChat->setObjectName(QStringLiteral("textBrowserChat"));
        textBrowserChat->setGeometry(QRect(10, 10, 431, 271));
        textBrowserChat->setFont(font);
        textEditMsg = new QTextEdit(Unicast);
        textEditMsg->setObjectName(QStringLiteral("textEditMsg"));
        textEditMsg->setGeometry(QRect(10, 290, 321, 51));
        textEditMsg->setFont(font);

        retranslateUi(Unicast);

        QMetaObject::connectSlotsByName(Unicast);
    } // setupUi

    void retranslateUi(QWidget *Unicast)
    {
        Unicast->setWindowTitle(QApplication::translate("Unicast", "Form", 0));
        pushButtonSend->setText(QApplication::translate("Unicast", "Send", 0));
    } // retranslateUi

};

namespace Ui {
    class Unicast: public Ui_Unicast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNICAST_H
