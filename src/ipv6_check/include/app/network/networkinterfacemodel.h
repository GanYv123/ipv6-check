#ifndef NETWORKINTERFACEMODEL_H
#define NETWORKINTERFACEMODEL_H

#include <QStandardItemModel>
#include "NetworkInfo.h"

class NetworkInterfaceModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit NetworkInterfaceModel(QObject *parent = nullptr);

    // 从单例里拉最新数据，重建整棵树
    void loadInterfaces();

private:
    void appendInterface(const NetworkInterface &nic);
    void appendAddressList(QStandardItem *parent,
                           const char *title,
                           const std::vector<IPv4AddressInfo> &v4,
                           const std::vector<IPv6AddressInfo> &v6);
};

#endif // NETWORKINTERFACEMODEL_H
