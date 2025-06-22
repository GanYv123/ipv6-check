// NetworkInterfaceManager.cpp
#include "NetworkInterfaceManager.h"
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QFile>
#include <QAbstractSocket>

NetworkInterfaceManager& NetworkInterfaceManager::instance() {
    static NetworkInterfaceManager inst;
    return inst;
}

NetworkInterfaceManager::NetworkInterfaceManager() {
    refreshInterfaces();
}

NetworkInterfaceManager::~NetworkInterfaceManager() {}

void NetworkInterfaceManager::refreshInterfaces() {
    m_interfaces.clear();
    auto qifs = QNetworkInterface::allInterfaces();
    for (const auto& qif : qifs) {
        NetworkInterface nic;
        nic.name         = qif.name().toStdString();
        nic.description  = qif.humanReadableName().toStdString();
        nic.macAddress   = qif.hardwareAddress().toStdString();
        nic.isUp         = qif.flags().testFlag(QNetworkInterface::IsUp);
        nic.isLoopback   = qif.flags().testFlag(QNetworkInterface::IsLoopBack);
        nic.mtu          = qif.maximumTransmissionUnit();
        nic.hardwareType = "";  // 可根据需要设置

        // 地址列表
        auto entries = qif.addressEntries();
        for (const auto& e : entries) {
            if (e.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                IPv4AddressInfo v4;
                v4.address   = e.ip().toString().toStdString();
                v4.netmask   = e.netmask().toString().toStdString();
                v4.broadcast = e.broadcast().toString().toStdString();
                v4.isPrimary = false; // Qt 不区分主次，可自行判断
                nic.ipv4Addresses.push_back(v4);
            } else if (e.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                IPv6AddressInfo v6;
                v6.address      = e.ip().toString().toStdString();
                v6.prefixLength = std::to_string(e.prefixLength());
                v6.isPrimary    = false;
                nic.ipv6Addresses.push_back(v6);
            }
        }

        m_interfaces.push_back(nic);
    }
    emit interfacesUpdated();
}

const std::vector<NetworkInterface>& NetworkInterfaceManager::interfaces() const {
    return m_interfaces;
}
