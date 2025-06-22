#include "NetworkInterfaceModel.h"
#include <QStandardItem>
#include "NetworkInterfaceManager.h"

NetworkInterfaceModel::NetworkInterfaceModel(QObject *parent)
    : QStandardItemModel(parent)
{
    // 设置两列标题
    setHorizontalHeaderLabels({ tr("属性"), tr("值") });
}

void NetworkInterfaceModel::loadInterfaces()
{
    clear();
    setHorizontalHeaderLabels({ tr("属性"), tr("值") });

    const auto &nics = NetworkInterfaceManager::instance().interfaces();
    for (const auto &nic : nics)
        appendInterface(nic);
}

void NetworkInterfaceModel::appendInterface(const NetworkInterface &nic)
{
    // 根节点
    auto *root = new QStandardItem(QString::fromStdString(nic.name));
    appendRow({ root, new QStandardItem() });

    auto addProp = [&](const QString &k, const QString &v){
        root->appendRow({ new QStandardItem(k), new QStandardItem(v) });
    };

    addProp(tr("描述"),    QString::fromStdString(nic.description));
    addProp(tr("MAC 地址"), QString::fromStdString(nic.macAddress));
    addProp(tr("MTU"),      QString::number(nic.mtu));
    addProp(tr("状态"),     nic.isUp ? tr("UP") : tr("DOWN"));
    addProp(tr("回环接口"), nic.isLoopback ? tr("是") : tr("否"));

    appendAddressList(root, "IPv4 地址", nic.ipv4Addresses, {});
    appendAddressList(root, "IPv6 地址", {}, nic.ipv6Addresses);

    // 链路速率
    auto *speedRoot = new QStandardItem(tr("链路速率"));
    root->appendRow({ speedRoot, new QStandardItem() });
    QString rx = QString::number(nic.speed.rx_bps/1e6) + " Mbps";
    QString tx = QString::number(nic.speed.tx_bps/1e6) + " Mbps";
    speedRoot->appendRow({ new QStandardItem(tr("接收")), new QStandardItem(rx) });
    speedRoot->appendRow({ new QStandardItem(tr("发送")), new QStandardItem(tx) });
    speedRoot->appendRow({ new QStandardItem(tr("双工")), new QStandardItem(QString::fromStdString(nic.speed.duplex)) });
}

void NetworkInterfaceModel::appendAddressList(QStandardItem *parent,
                                              const char *title,
                                              const std::vector<IPv4AddressInfo> &v4,
                                              const std::vector<IPv6AddressInfo> &v6)
{
    if (v4.empty() && v6.empty()) return;

    auto *addrRoot = new QStandardItem(tr(title));
    parent->appendRow({ addrRoot, new QStandardItem() });

    for (const auto &a : v4) {
        QString s = QString::fromStdString(a.address) + "/" + QString::fromStdString(a.netmask)
        + (a.isPrimary ? tr(" (主)") : "");
        addrRoot->appendRow({ new QStandardItem(s), new QStandardItem() });
    }
    for (const auto &a : v6) {
        QString s = QString::fromStdString(a.address) + "/" + QString::fromStdString(a.prefixLength)
        + (a.isPrimary ? tr(" (主)") : "");
        addrRoot->appendRow({ new QStandardItem(s), new QStandardItem() });
    }
}
