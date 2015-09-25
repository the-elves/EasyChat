#ifndef UNICAST_H
#define UNICAST_H

#include <QWidget>

namespace Ui {
class Unicast;
}

class Unicast : public QWidget
{
    Q_OBJECT

public:
    explicit Unicast(QString subject, QWidget *parent = 0);
    ~Unicast();
    QString getSubject();

public slots:
    void receiveMessage(QString msg);

private slots:
    void on_pushButtonSend_clicked();

    void on_textEditMsg_textChanged();

signals:
    void sendMessage(QString msg);

private:
    QString subject;
    Ui::Unicast *ui;
};


#endif // UNICAST_H
