#include "networkInterfaceManager.h"
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <winsock2.h>
#include <iphlpapi.h>
#include <vector>
#include <string>
#include <cstring>

#pragma comment(lib, "iphlpapi.lib")

NetworkInterfaceManager& NetworkInterfaceManager::instance() {
    static NetworkInterfaceManager inst;
    return inst;
}

NetworkInterfaceManager::NetworkInterfaceManager() {
    refreshInterfaces();
}

NetworkInterfaceManager::~NetworkInterfaceManager() {}

static void fillIPv4Info(const QNetworkAddressEntry &entry, NetworkInterface &nic) {
    IPv4AddressInfo v4;
    v4.address   = entry.ip().toString().toStdString();
    v4.netmask   = entry.netmask().toString().toStdString();
    v4.broadcast = entry.broadcast().toString().toStdString();
    v4.isPrimary = false;
    nic.ipv4Addresses.push_back(v4);
}

static void fillIPv6Info(const QNetworkAddressEntry &entry, NetworkInterface &nic) {
    IPv6AddressInfo v6;
    v6.address      = entry.ip().toString().toStdString();
    v6.prefixLength = std::to_string(entry.prefixLength());
    v6.isPrimary    = false;
    nic.ipv6Addresses.push_back(v6);
}

bool getInterfaceStatsAndSpeed(const std::string& ifaceDescr,
                               NetworkStatistics& stats,
                               LinkSpeed& speed)
{
    DWORD dwSize = 0;
    GetIfTable(nullptr, &dwSize, FALSE);
    std::vector<char> buffer(dwSize);
    MIB_IFTABLE* pTable = reinterpret_cast<MIB_IFTABLE*>(buffer.data());

    if (GetIfTable(pTable, &dwSize, FALSE) == NO_ERROR) {
        for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
            MIB_IFROW &row = pTable->table[i];
            std::string rowDescr(reinterpret_cast<char*>(row.bDescr), row.dwDescrLen);

            if (rowDescr == ifaceDescr || ifaceDescr.find(rowDescr) != std::string::npos) {
                stats.rx_bytes    = row.dwInOctets;
                stats.tx_bytes    = row.dwOutOctets;
                stats.rx_packets  = row.dwInUcastPkts;
                stats.tx_packets  = row.dwOutUcastPkts;

                speed.rx_bps = speed.tx_bps = row.dwSpeed;
                speed.duplex = "Unknown";
                return true;
            }
        }
    }
    return false;
}

static std::vector<std::string> getSystemDnsServers() {
    std::vector<std::string> dnsList;
    ULONG bufLen = sizeof(FIXED_INFO);
    std::vector<char> buffer(bufLen);
    FIXED_INFO* pInfo = reinterpret_cast<FIXED_INFO*>(buffer.data());

    if (GetNetworkParams(pInfo, &bufLen) == ERROR_BUFFER_OVERFLOW) {
        buffer.resize(bufLen);
        pInfo = reinterpret_cast<FIXED_INFO*>(buffer.data());
    }

    if (GetNetworkParams(pInfo, &bufLen) == NO_ERROR) {
        IP_ADDR_STRING *pIP = &pInfo->DnsServerList;
        while (pIP) {
            dnsList.emplace_back(pIP->IpAddress.String);
            pIP = pIP->Next;
        }
    }
    return dnsList;
}

static std::string getDefaultGatewayIPv4(const std::string& ifaceName) {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD buflen = sizeof(adapterInfo);

    if (GetAdaptersInfo(adapterInfo, &buflen) == NO_ERROR) {
        PIP_ADAPTER_INFO pAdapter = adapterInfo;
        while (pAdapter) {
            if (ifaceName == pAdapter->AdapterName &&
                pAdapter->GatewayList.IpAddress.String[0] != '\0') {
                return pAdapter->GatewayList.IpAddress.String;
            }
            pAdapter = pAdapter->Next;
        }
    }
    return "";
}

void NetworkInterfaceManager::refreshInterfaces() {
    m_interfaces.clear();

    const auto qifs = QNetworkInterface::allInterfaces();
    const auto dnsServers = getSystemDnsServers();
    bool dnsAssigned = false;

    for (const auto &qif : qifs) {
        NetworkInterface nic;
        nic.name         = qif.name().toStdString();
        nic.description  = qif.humanReadableName().toStdString();
        nic.macAddress   = qif.hardwareAddress().toStdString();
        nic.isUp         = qif.flags().testFlag(QNetworkInterface::IsUp);
        nic.isLoopback   = qif.flags().testFlag(QNetworkInterface::IsLoopBack);
        nic.mtu          = qif.maximumTransmissionUnit();
        nic.hardwareType = "";

        // 判断类型
        const QString qname = QString::fromStdString(nic.name).toLower();
        if (nic.isLoopback) {
            nic.type = NetworkType::Loopback;
        } else if (qname.startsWith("wi") || qname.contains("wireless")) {
            nic.type = NetworkType::WiFi;
        } else if (qname.startsWith("eth") || qname.contains("ethernet")) {
            nic.type = NetworkType::Ethernet;
        } else {
            nic.type = NetworkType::Unknown;
        }

        // 地址信息
        for (const auto &entry : qif.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                fillIPv4Info(entry, nic);
            else if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol)
                fillIPv6Info(entry, nic);
        }

        // 默认网关 IPv4
        nic.defaultGatewayIPv4 = getDefaultGatewayIPv4(nic.name);

        // DNS
        if (!dnsAssigned && !nic.isLoopback) {
            nic.dnsServers = dnsServers;
            dnsAssigned = true;
        }

        // 统计与速率
        getInterfaceStatsAndSpeed(nic.description, nic.statistics, nic.speed);

        m_interfaces.push_back(nic);
    }

    emit interfacesUpdated();
}

const std::vector<NetworkInterface>& NetworkInterfaceManager::interfaces() const {
    return m_interfaces;
}
