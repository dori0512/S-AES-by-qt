# S-AES by Qt

**教学用简化 AES（S-AES）的完整可运行实现：算法核心 + Qt 图形界面 + CBC / 多重加密 / 暴力破解实验。**

> 基于 Stallings《密码编码学与网络安全》附录中的 **Simplified AES**。  
> 目标是把课本里的置换、S-Box、密钥扩展「跑起来」，并用 GUI 做可演示、可交叉验证的实验，而不是生产级加密。

[![C++](https://img.shields.io/badge/C++-Qt-00599C?logo=cplusplus)](https://www.qt.io/)
[![Qt](https://img.shields.io/badge/Qt-5.13+-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![License](https://img.shields.io/badge/purpose-coursework%20%2F%20demo-lightgrey)](#免责声明)

---

## 亮点

| 模块 | 你能看到什么 |
|------|----------------|
| **S-AES 核心** | 16-bit 分组 / 16-bit 密钥；密钥扩展、轮函数、加解密闭环 |
| **Qt GUI** | 加密 / 解密分栏，二进制与 ASCII 输入校验，错误提示 |
| **工作模式** | CBC 加解密实验界面 |
| **多重加密** | 双重 / 三重（两密钥）相关实验控件 |
| **分析实验** | 暴力破解界面，便于观察密钥空间与耗时 |

详细逐步实验记录（含截图）见：[docs/lab-report.md](./docs/lab-report.md)  
算法说明 PDF、接口说明见仓库根目录附件。

---

## 仓库结构

```
S-AES-by-qt/
├── README.md                 # 本文件（作品集入口）
├── docs/lab-report.md        # 原课程实验长报告（截图 + 过程）
├── S-AES接口文档.md
├── S-AES算法介绍.pdf
└── S-AES/                    # Qt 工程
    ├── S-AEC.pro
    ├── aes.{h,cpp}           # 算法核心
    ├── *widget*.{h,cpp}      # 各功能界面
    └── main.cpp
```

---

## 快速开始

### 环境

- Qt **5.13+**（原开发：Qt 5.13.2 / Qt Creator 4.10.1）
- 支持的 Kits：MSVC 或 MinGW（与你的 Qt 安装一致即可）

### 打开与运行

1. 用 Qt Creator 打开 `S-AES/S-AEC.pro`
2. 配置 Kit → 构建 → 运行
3. 在界面中输入 **16-bit 二进制密钥** 与明文（二进制或可编码为 ASCII 的字符串），测试加解密是否互逆

命令行（示例，路径按本机 Qt 安装调整）：

```bash
cd S-AES
qmake S-AEC.pro
make        # 或 nmake / mingw32-make
```

---

## 学习/面试时怎么讲

1. **为什么是 16-bit？** — 缩小 AES 结构，便于手算与单步调试，同时保留「代换–置换–密钥混合」骨架。  
2. **GUI 解决什么？** — 把算法从「纸上作业」变成可演示产物，方便交叉测试与截图留证。  
3. **CBC / 多重加密 / 破解** — 把工作模式与攻击面放进同一工程，形成「实现 → 模式 → 分析」闭环。

---

## 相关文档

- [接口文档](./S-AES接口文档.md)
- [完整实验报告（原 README）](./docs/lab-report.md)
- `S-AES算法介绍.pdf`

---

## 免责声明

本项目仅用于密码学课程理解与演示。**请勿**将 S-AES 或本实现用于保护真实数据；真实场景请使用经过充分审查的标准算法与库（如 AES-GCM via OS/CryptoAPI/OpenSSL 等）。

---

<p align="center"><sub>Coursework crypto, engineered to be runnable.</sub></p>
