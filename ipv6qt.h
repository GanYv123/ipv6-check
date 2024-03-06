#pragma once

#include <QtWidgets/QDialog>
#include "ui_ipv6qt.h"
#include <QNetworkInterface>
#include <map>

class ipv6Qt : public QDialog
{
    Q_OBJECT

public:
    ipv6Qt(QWidget *parent = nullptr);
    ~ipv6Qt();
    bool judgeIpv6(QList<QNetworkInterface>& interface);

private:
    Ui::ipv6QtClass ui;
    QList<QNetworkInterface> interfaces;
    std::map<QString,QString> ipv6_infos;
private slots:
    void on_bt_init_clicked();
    void on_bt_chooseNet_clicked();
    void on_bt_getip_clicked();
};
