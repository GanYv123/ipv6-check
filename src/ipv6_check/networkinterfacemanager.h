// NetworkInterfaceManager.h
#ifndef NETWORKINTERFACEMANAGER_H
#define NETWORKINTERFACEMANAGER_H

#include <QObject>
#include <vector>
#include "NetworkInfo.h"

class NetworkInterfaceManager : public QObject {
    Q_OBJECT
public:
    // 获取单例实例
    static NetworkInterfaceManager& instance();

    // 刷新并获取所有网卡信息
    void refreshInterfaces();
    const std::vector<NetworkInterface>& interfaces() const;

signals:
    // 网卡列表更新通知
    void interfacesUpdated();

private:
    NetworkInterfaceManager();
    ~NetworkInterfaceManager();
    Q_DISABLE_COPY(NetworkInterfaceManager)

    std::vector<NetworkInterface> m_interfaces;
};

#endif // NETWORKINTERFACEMANAGER_H
