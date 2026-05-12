# ipv6-check

一个基于 Qt 开发的桌面应用程序，用于查看本机的网络接口信息，包括 IPv4、IPv6、MAC 地址、MTU、链路状态等。适用于网络调试或系统信息监测工具的组件开发。

## 项目状态

项目正在开发中，初步实现网卡信息采集和 TreeView 展示。

## 功能简介

- 枚举所有本地网卡（包括虚拟和物理接口）
- 显示以下信息：
  - 网卡名称、描述、MAC 地址
  - 是否启用、是否为回环接口、MTU
  - IPv4 和 IPv6 地址及子网掩码/前缀长度
  - （Linux）链路速率信息读取（可选）

## 仓库结构

```text
.
├── CMakeLists.txt            # 顶层构建入口
├── README.md
├── demo/                     # 文档与示例
└── src/
    └── ipv6_check/
        ├── CMakeLists.txt        # 应用构建脚本
        ├── src/                  # 业务实现代码
        │   ├── main.cpp
        │   ├── ui/
        │   └── network/
        ├── include/
        │   ├── app/              # 项目头文件（ui/network）
        │   └── ...               # ElaWidgetTools 头文件和资源
        ├── fonts/                # 字体资源
        └── lib/                  # 三方库文件
```

## 构建要求

- Qt 5 或 Qt 6（推荐 Qt 6.2+）
- CMake ≥ 3.16
- C++17 编译器（GCC / Clang / MSVC）
- 本仓库项目采用 MSVC 2022 构建
- 建议使用 Release 版本

## 构建方法

```bash
git clone https://github.com/your_username/ipv6-check.git
cd ipv6-check
cmake -S . -B build
cmake --build build --config Release
```

> [!WARNING]
>
> 若提示缺失 **ElaWidgetTools.dll** 可从 `src/ipv6_check/lib` 文件夹中获取。
