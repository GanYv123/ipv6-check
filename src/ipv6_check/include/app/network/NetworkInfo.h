#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include <string>
#include <vector>
#include <cstdint>

// 记录 IPv4 地址信息
struct IPv4AddressInfo {
    std::string address;      // IPv4 地址，如 192.168.1.10
    std::string netmask;      // 子网掩码，如 255.255.255.0
    std::string broadcast;    // 广播地址，如 192.168.1.255
    bool        isPrimary;    // 是否为主地址
};

// 记录 IPv6 地址信息
struct IPv6AddressInfo {
    std::string address;      // IPv6 地址，如 fe80::1
    std::string prefixLength; // 前缀长度，如 /64
    bool        isPrimary;    // 是否为主地址
};

// 记录流量统计
struct NetworkStatistics {
    uint64_t rx_bytes = 0;
    uint64_t tx_bytes = 0;
    uint64_t rx_packets = 0;
    uint64_t tx_packets = 0;
};

// 链路速率信息
struct LinkSpeed {
    uint64_t    rx_bps;       // 接收速率，单位：比特/秒
    uint64_t    tx_bps;       // 发送速率，单位：比特/秒
    std::string duplex;       // 全双工（full）或半双工（half）
};

// 网络流量统计
struct LinkStatistics {
    uint64_t rx_bytes;    // 累计接收字节数
    uint64_t tx_bytes;    // 累计发送字节数
    uint64_t rx_packets;  // 接收数据包数
    uint64_t tx_packets;  // 发送数据包数
};

// 网络类型定义
enum class NetworkType {
    Unknown,
    Ethernet,
    WiFi,
    Loopback,
    Virtual
};

// 记录单个网卡信息
struct NetworkInterface {
    std::string                     name;               // 网卡名称，如 eth0、wlan0
    std::string                     macAddress;         // MAC 地址，如 01:23:45:67:89:ab
    bool                            isUp;               // 是否已激活 (UP)
    bool                             isLoopback;        // 是否为回环接口
    uint32_t                        mtu;                // 最大传输单元 (MTU)
    std::string                     driver;             // 驱动名称，如 e1000e
    std::string                     hardwareType;       // 硬件类型描述，如 Ethernet, Wireless
    std::vector<IPv4AddressInfo>    ipv4Addresses;      // IPv4 地址列表
    std::vector<IPv6AddressInfo>    ipv6Addresses;      // IPv6 地址列表
    LinkSpeed                       speed;              // 链路速率信息
    NetworkStatistics               statistics;         // 网络流量统计
    std::string                     description;        // 接口描述或别名
    std::string                     defaultGatewayIPv4; // 默认网关 IPv4
    std::string                     defaultGatewayIPv6; // 默认网关 IPv6
    std::vector<std::string>        dnsServers;         // DNS 服务器列表
    NetworkType                     type;               // 网络类型
};

#endif // NETWORKINFO_H
