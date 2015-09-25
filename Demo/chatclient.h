#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include "unicast.h"
#include <QTimer>
#include <QThread>
namespace Ui {
class ChatClient;
}

class ChatClient : public QMainWindow {
    Q_OBJECT

public:

    static std::vector<QPushButton*> onlineUsers;
    static std::vector<Unicast*> unichat;
    explicit ChatClient(QString subject, QWidget *parent = 0);
    ~ChatClient();
    bool heartBeatReceived;
    void start(const QString& msg);
    void startConnection(QString address, quint16 port);
    void closeEvent(QCloseEvent *event);
    //QTimer *heartBeatTimer;
public slots:
    void writeMessage(const QString& msg);
    void readyRead();
    void unicast();
    void heartBeatSend();
    void heartBeatReceive();


private slots:
    void on_pushButtonSend_clicked();

    void on_textEditMsg_textChanged();

    void on_pushButtonConnect_clicked();

    void on_lineEditName_textChanged();

signals:
    void sendMsgToUnicast(QString msg);

private:
    QString subject;
    Ui::ChatClient *ui;
    QTcpSocket client;
};

#endif // CHATCLIENT_H
