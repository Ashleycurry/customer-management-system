# 客户信息管理系统

[中文](README.md) | [English](README-English.md)

一个基于 C11 的控制台客户信息管理系统示例。项目根据 `docs/客户信息管理系统.docx` 的需求实现，支持添加、修改、删除客户和查看客户列表。

项目将原始单文件示例拆分为应用层、领域层和控制台界面层，并加入输入校验、动态数组、结果码和资源释放。

## 功能特性

- 添加客户：录入姓名、性别、年龄、电话和邮箱。
- 修改客户：根据客户编号修改资料，直接回车保留原值。
- 删除客户：根据客户编号删除客户，删除前要求 `y/n` 确认。
- 客户列表：展示当前所有客户的编号和详细资料。
- 退出系统：退出前要求 `y/n` 确认。
- 客户编号从 `1` 开始，删除记录后自动整理后续编号。
- 校验菜单、姓名、性别、年龄、电话、邮箱和确认输入。
- 使用动态数组管理客户记录，容量不足时自动扩展。

## 目录结构

```text
customer-management-system/
├── README.md
├── README-English.md
├── docs/
│   └── 客户信息管理系统.docx
└── src/
    ├── main.c
    ├── app/
    │   ├── customer_app.c
    │   └── customer_app.h
    ├── domain/
    │   ├── customer_directory.c
    │   ├── customer_directory.h
    │   └── customer_types.h
    └── ui/
        ├── console_input.c
        ├── console_input.h
        ├── console_view.c
        └── console_view.h
```

## 模块说明

| 模块 | 职责 |
| --- | --- |
| `src/main.c` | 程序入口，启动应用 |
| `src/app/` | 编排菜单流程和业务操作 |
| `src/domain/` | 管理客户资料、目录和业务规则 |
| `src/ui/` | 负责控制台输入、校验和展示 |
| `docs/` | 保存原始需求文档 |

应用层连接领域层和控制台层，不直接管理客户数组。领域层负责客户目录的增删改查、编号维护、资料校验和资源生命周期。控制台层负责读取输入、展示菜单与结果。

## 编译运行

项目使用标准 C11，不依赖第三方库。请在项目根目录执行以下命令。

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Isrc \
    src/main.c \
    src/app/customer_app.c \
    src/domain/customer_directory.c \
    src/ui/console_input.c \
    src/ui/console_view.c \
    -o customer-management-system
```

运行程序：

```bash
./customer-management-system
```

Windows PowerShell：

```powershell
.\customer-management-system.exe
```

## 使用流程

```text
1. 添加客户
2. 修改客户
3. 删除客户
4. 客户列表
5. 退出
```

添加客户时输入姓名、性别、年龄、电话和邮箱。性别使用 `f` 表示女、`m` 表示男。修改客户时输入编号，直接回车保留当前字段；输入 `-1` 取消。删除客户时输入编号并确认 `y/n`，删除后后续编号会重新整理。

## 校验规则

| 字段 | 规则 |
| --- | --- |
| 姓名 | 必填，最多 `63` 字节 |
| 性别 | 只接受 `f/F` 或 `m/M` |
| 年龄 | `1` 到 `150` |
| 电话 | 允许数字、`+`、`-`、括号和空格，至少包含 3 位数字 |
| 邮箱 | 必须包含一个 `@`、域名分隔点且不能有空格 |

## 工程化设计

代码通过头文件暴露模块接口，使用 `CustomerDirectory` 封装客户集合，使用 `CustomerResult` 表示业务结果。客户记录使用动态数组保存，目录容量不足时自动扩展。应用退出前调用销毁函数释放动态内存。

## 当前边界

客户数据只保存在当前进程内，退出后不会持久化。当前版本没有数据库、文件存储、登录认证、网络接口、图形界面、Makefile、CMake 配置或自动化测试。

## 后续扩展

可以继续增加文件或数据库持久化、客户查询、分组标签、导入导出、单元测试以及 CMake 或 Makefile 构建流程。

## 参考资料

[客户信息管理系统需求文档](docs/客户信息管理系统.docx)

[C 源代码](src/)
