#include "unicast.h"
#include "ui_unicast.h"
#include <iostream>
#include <QTime>
#include <QScrollBar>
#include <chatclient.h>

Unicast::Unicast(QString subject, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Unicast)
{
    ui->setupUi(this);
    this->subject = subject;
    this->setWindowTitle(subject);
}

Unicast::~Unicast()
{
    delete ui;
}

QString Unicast::getSubject() {
    return this->subject;
}


void Unicast::on_pushButtonSend_clicked()
{
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "Me" + ": " + ui->textEditMsg->toPlainText() + "\n");
    ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
    emit sendMessage(this->subject + "$%^" + ui->textEditMsg->toPlainText());
    ui->textEditMsg->setText("");
}

void Unicast::on_textEditMsg_textChanged()
{
    if(ui->textEditMsg->toPlainText() == "")
        ui->pushButtonSend->setEnabled(false);
    else
        ui->pushButtonSend->setEnabled(true);
}

void Unicast::receiveMessage(QString msg) {
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + msg + "\n");
    ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
}
