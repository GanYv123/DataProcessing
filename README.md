# DataProcessing

## 软件简介

本软件是一款用于从 Excel 导入数据并在软件中进行数据整合的工具。通过该软件，用户可以轻松地将 Excel 文件中的数据导入到软件中进行处理和分析。

## 功能特性

- **Excel 数据导入**：支持从 Excel 文件中导入数据（支持金山的wps 和 微软的 excel）。
- **数据整合**：提供数据整合和管理功能，
    实现了将不同的excel的学生成绩信息和考勤信息汇总成一张总评表，提供自己添加和删除学生。
- **友好的用户界面**：简单直观的操作界面，易于使用。

## 安装指南
  提供绿色版本直接解压就可使用
  提供单文件的exe文件
  提供安装包版本

### 系统要求

- 主要构建环境：Windows + Qt / MSVC，使用 C++17。
- Qt 模块：Core、Gui、Widgets、Sql；使用 Boost 头文件和仓库自带的 QXlsx。
- `DataProcessing.pro` 中的 Boost 路径需按本机安装位置调整（Windows 配置当前指向 `D:/OPLIB/boost_1_85_0`）。
- 工程包含 Linux 配置分支；其他平台的依赖、路径及部署需按实际环境检查，本文不保证跨平台直接构建。

### 安装步骤

1. 克隆仓库到本地：
    ```bash
    git clone https://github.com/GanYv123/DataProcessing.git
    ```
2. 安装 Qt、匹配的 C++17 编译器及 Boost，并检查 `DataProcessing.pro` 中的依赖路径。QXlsx 已包含在仓库中。
3. 在 Qt Creator 中打开 `DataProcessing.pro`，选择匹配的 Kit，点击构建后运行。Windows 构建配置使用 MSVC 编译选项，建议选择 MSVC Kit。
4. 如使用已发布的程序，可查看 [Releases](https://github.com/GanYv123/DataProcessing/releases)；具体包格式以该页面实际提供的文件为准。

## 使用手册

### 1. 启动软件

双击运行生成的可执行文件 打开软件主界面。

### 2. 导入数据

1. 在主界面上，点击菜单栏的“文件”->“导入”。
2. 在弹出的文件选择对话框中，选择要导入的 Excel 文件，然后点击“打开”。
3. 提供直接拖拽文件到界面打开
4. 提供直接通过配置文件导入信息
5. 数据库连接相关实现见 `sqldata.*` 和 `mainwindow.cpp`；使用前需配置实际数据库连接参数。

### 3. 导出数据

1. 数据处理完成后，点击菜单栏的“文件”->“导出”,会导出一张成绩汇总表。
2. 或者点击保存配置，实现数据的本地保存。
3. 在弹出的文件保存对话框中，选择保存位置和文件格式，然后点击“保存”。

## 贡献指南

欢迎对本项目提出贡献。您可以通过以下方式参与：

1. 提交问题（Issues）
2. 提交功能请求（Feature Requests）
3. 提交合并请求（Pull Requests）

### 提交代码

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交代码 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开一个 Pull Request

## 许可证

仓库当前未提供独立的 `LICENSE` 文件，开源许可尚未明确。第三方依赖的许可请参考对应目录。

## 联系信息

如有任何问题或建议，请通过以下方式联系我们：

- 电子邮件：xuyang030726@gmail.com
---

感谢您使用本软件！
