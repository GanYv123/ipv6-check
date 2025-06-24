#include "networkInterfaceManager.h"
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QAbstractSocket>
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

NetworkInterfaceManager& NetworkInterfaceManager::instance() {
    static NetworkInterfaceManager inst;
    return inst;
}

NetworkInterfaceManager::NetworkInterfaceManager() {
    refreshInterfaces();
}

NetworkInterfaceManager::~NetworkInterfaceManager() {}

bool getInterfaceStatsAndSpeed(const std::string& ifaceName,
                               NetworkStatistics& stats,
                               LinkSpeed& speed)
{
    MIB_IFTABLE *pTable = nullptr;
    DWORD dwSize = 0;
    if (GetIfTable(nullptr, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        pTable = (MIB_IFTABLE *)malloc(dwSize);
        if (GetIfTable(pTable, &dwSize, FALSE) == NO_ERROR) {
            for (DWORD i = 0; i < pTable->dwNumEntries; ++i) {
                MIB_IFROW &row = pTable->table[i];
                std::string rowName(reinterpret_cast<char*>(row.bDescr), row.dwDescrLen);

                // 用描述字段进行匹配（你用的是 qif.humanReadableName）
                if (rowName == ifaceName || ifaceName.find(rowName) != std::string::npos) {
                    stats.rx_bytes = row.dwInOctets;
                    stats.tx_bytes = row.dwOutOctets;
                    stats.rx_packets = row.dwInUcastPkts;
                    stats.tx_packets = row.dwOutUcastPkts;

                    speed.rx_bps = row.dwSpeed;  // Windows 不区分收发速率，一般表示最大速度
                    speed.tx_bps = row.dwSpeed;
                    speed.duplex = "Unknown";    // Windows API 无法直接得知
                    free(pTable);
                    return true;
                }
            }
        }
        free(pTable);
    }
    return false;
}

static std::vector<std::string> getSystemDnsServers() {
    std::vector<std::string> dnsList;
    FIXED_INFO *pFixedInfo;
    ULONG ulOutBufLen = sizeof(FIXED_INFO);
    pFixedInfo = (FIXED_INFO *)malloc(ulOutBufLen);

    if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pFixedInfo);
        pFixedInfo = (FIXED_INFO *)malloc(ulOutBufLen);
    }

    if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {
        IP_ADDR_STRING *pIP = &pFixedInfo->DnsServerList;
        while (pIP) {
            dnsList.push_back(pIP->IpAddress.String);
            pIP = pIP->Next;
        }
    }

    free(pFixedInfo);
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
    auto dnsServers = getSystemDnsServers();
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

        // 粗略判断类型
        QString qname = QString::fromStdString(nic.name);
        if (nic.isLoopback) {
            nic.type = NetworkType::Loopback;
        } else if (qname.startsWith("Wi") || qname.contains("Wireless")) {
            nic.type = NetworkType::WiFi;
        } else if (qname.startsWith("Eth") || qname.contains("Ethernet")) {
            nic.type = NetworkType::Ethernet;
        } else {
            nic.type = NetworkType::Unknown;
        }

        // 地址
        for (const auto &entry : qif.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                IPv4AddressInfo v4;
                v4.address   = entry.ip().toString().toStdString();
                v4.netmask   = entry.netmask().toString().toStdString();
                v4.broadcast = entry.broadcast().toString().toStdString();
                v4.isPrimary = false;
                nic.ipv4Addresses.push_back(v4);
            } else if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                IPv6AddressInfo v6;
                v6.address      = entry.ip().toString().toStdString();
                v6.prefixLength = std::to_string(entry.prefixLength());
                v6.isPrimary    = false;
                nic.ipv6Addresses.push_back(v6);
            }
        }

        // 默认网关
        nic.defaultGatewayIPv4 = getDefaultGatewayIPv4(nic.name);

        // DNS 仅赋给第一个非 Loopback 网卡
        if (!dnsAssigned && !nic.isLoopback) {
            nic.dnsServers = dnsServers;
            dnsAssigned = true;
        }

        // // 暂无速率/统计实现（需要 GetIfEntry 等）
        // nic.statistics = {};
        // nic.speed = {0, 0, "Unknown"};

        getInterfaceStatsAndSpeed(nic.description, nic.statistics, nic.speed);

        m_interfaces.push_back(nic);
    }

    emit interfacesUpdated();
}

const std::vector<NetworkInterface>& NetworkInterfaceManager::interfaces() const {
    return m_interfaces;
}
