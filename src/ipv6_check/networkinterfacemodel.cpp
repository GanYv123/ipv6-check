#include "networkInterfaceModel.h"
#include <QStandardItem>
#include "NetworkInterfaceManager.h"

NetworkInterfaceModel::NetworkInterfaceModel(QObject *parent)
    : QStandardItemModel(parent)
{
    setHorizontalHeaderLabels({ tr("网络信息") });
}

void NetworkInterfaceModel::loadInterfaces()
{
    clear();
    setHorizontalHeaderLabels({ tr("网络信息") });

    const auto &nics = NetworkInterfaceManager::instance().interfaces();
    for (const auto &nic : nics)
        appendInterface(nic);
}

void NetworkInterfaceModel::appendInterface(const NetworkInterface &nic)
{
    // 工具函数：创建不可编辑的 QStandardItem
    auto makeItem = [](const QString &text) {
        auto *item = new QStandardItem(text);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        return item;
    };

    // 根节点：网卡名称 + 描述
    const auto nicTopic = QString::fromStdString(nic.name) + " - " + QString::fromStdString(nic.description);
    auto *root = makeItem(nicTopic);
    appendRow(root);

    // 属性项添加器
    auto addProp = [&](const QString &k, const QString &v){
        root->appendRow(makeItem(k + ": " + v));
    };

    // 添加基础属性
    addProp(tr("MAC 地址"), QString::fromStdString(nic.macAddress));
    addProp(tr("MTU"), QString::number(nic.mtu));
    addProp(tr("状态"), nic.isUp ? tr("UP") : tr("DOWN"));
    addProp(tr("回环接口"), nic.isLoopback ? tr("是") : tr("否"));

    // 添加地址列表
    appendAddressList(root, "IPv4 地址", nic.ipv4Addresses, {});
    appendAddressList(root, "IPv6 地址", {}, nic.ipv6Addresses);

    // 链路速率子节点
    auto *speedRoot = makeItem(tr("链路速率"));
    root->appendRow(speedRoot);
    speedRoot->appendRow(makeItem(tr("接收: ") + QString::number(nic.speed.rx_bps / 1e6) + " Mbps"));
    speedRoot->appendRow(makeItem(tr("发送: ") + QString::number(nic.speed.tx_bps / 1e6) + " Mbps"));
    speedRoot->appendRow(makeItem(tr("双工: ") + QString::fromStdString(nic.speed.duplex)));

    // 网络类型
    QString typeStr;
    switch (nic.type) {
    case NetworkType::Ethernet: typeStr = "Ethernet"; break;
    case NetworkType::WiFi:     typeStr = "WiFi"; break;
    case NetworkType::Loopback: typeStr = "Loopback"; break;
    case NetworkType::Virtual:  typeStr = "Virtual"; break;
    default:                    typeStr = "Unknown"; break;
    }
    addProp(tr("网络类型"), typeStr);

    // 默认网关
    if (!nic.defaultGatewayIPv4.empty())
        addProp(tr("默认网关 IPv4"), QString::fromStdString(nic.defaultGatewayIPv4));
    if (!nic.defaultGatewayIPv6.empty())
        addProp(tr("默认网关 IPv6"), QString::fromStdString(nic.defaultGatewayIPv6));

    // DNS 列表
    if (!nic.dnsServers.empty()) {
        auto *dnsRoot = makeItem(tr("DNS 服务器"));
        root->appendRow(dnsRoot);
        for (const auto &dns : nic.dnsServers) {
            dnsRoot->appendRow(makeItem(QString::fromStdString(dns)));
        }
    }

    // 流量统计
    auto *statsRoot = makeItem(tr("流量统计"));
    root->appendRow(statsRoot);
    statsRoot->appendRow(makeItem(tr("接收字节: ") + QString::number(nic.statistics.rx_bytes)));
    statsRoot->appendRow(makeItem(tr("发送字节: ") + QString::number(nic.statistics.tx_bytes)));
    statsRoot->appendRow(makeItem(tr("接收包数: ") + QString::number(nic.statistics.rx_packets)));
    statsRoot->appendRow(makeItem(tr("发送包数: ") + QString::number(nic.statistics.tx_packets)));

}

void NetworkInterfaceModel::appendAddressList(QStandardItem *parent,
                                              const char *title,
                                              const std::vector<IPv4AddressInfo> &v4,
                                              const std::vector<IPv6AddressInfo> &v6)
{
    if (v4.empty() && v6.empty()) return;

    auto makeItem = [](const QString &text) {
        auto *item = new QStandardItem(text);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        return item;
    };

    auto *addrRoot = makeItem(tr(title));
    parent->appendRow(addrRoot);

    for (const auto &a : v4) {
        QString s = QString::fromStdString(a.address) + "/" + QString::fromStdString(a.netmask);
        if (a.isPrimary) s += tr(" (主)");
        addrRoot->appendRow(makeItem(s));
    }

    for (const auto &a : v6) {
        QString s = QString::fromStdString(a.address) + "/" + QString::fromStdString(a.prefixLength);
        if (a.isPrimary) s += tr(" (主)");
        addrRoot->appendRow(makeItem(s));
    }
}
