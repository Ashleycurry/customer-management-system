# Customer Management System

[中文](README.md) | [English](README-English.md)

A C11 console customer management system based on `docs/客户信息管理系统.docx`. It supports adding, updating, deleting, and listing customer records.

The original single-file example has been organized into application, domain, and console UI layers with input validation, a dynamic array, explicit result codes, and resource cleanup.

## Features

- Add customers with a name, gender, age, phone number, and email.
- Update a customer by ID; pressing Enter keeps the current value.
- Delete a customer by ID after `y/n` confirmation.
- Display all current customer records.
- Confirm before exiting.
- Assign IDs from `1` and renumber later records after deletion.
- Validate menu choices, customer fields, and confirmation input.
- Store records in a dynamically growing array.

## Directory Structure

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

## Module Overview

| Module | Responsibility |
| --- | --- |
| `src/main.c` | Application entry point |
| `src/app/` | Orchestrates menu actions and workflows |
| `src/domain/` | Owns customer data and business rules |
| `src/ui/` | Handles console input, validation, and output |
| `docs/` | Contains the original requirements document |

The application layer connects the domain and console layers. The domain layer manages customer CRUD operations, ID maintenance, validation, and resource ownership. The console layer handles user input and presentation.

## Build and Run

The project uses standard C11 and has no third-party dependencies. Run these commands from the project root.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Isrc \
    src/main.c \
    src/app/customer_app.c \
    src/domain/customer_directory.c \
    src/ui/console_input.c \
    src/ui/console_view.c \
    -o customer-management-system
```

Run the application:

```bash
./customer-management-system
```

On Windows PowerShell:

```powershell
.\customer-management-system.exe
```

## User Flow

```text
1. Add customer
2. Update customer
3. Delete customer
4. Customer list
5. Exit
```

When adding a customer, enter a name, gender, age, phone number, and email. Use `f` for female and `m` for male. When updating, enter the ID and press Enter to keep an existing field; enter `-1` to cancel. Deletion requires `y/n` confirmation and renumbers later IDs.

## Validation Rules

| Field | Rule |
| --- | --- |
| Name | Required; maximum `63` bytes |
| Gender | Accepts only `f/F` or `m/M` |
| Age | Between `1` and `150` |
| Phone | Allows digits, `+`, `-`, parentheses, and spaces; at least 3 digits |
| Email | Must contain one `@`, a domain separator dot, and no spaces |

## Engineering Design

Headers expose explicit module interfaces. `CustomerDirectory` encapsulates the customer collection, while `CustomerResult` communicates business outcomes. Records are stored in a dynamically growing array, and the directory releases its allocated memory before the application exits.

## Current Scope

Customer data is kept in memory and is lost when the process exits. There is no database, file persistence, authentication, network API, graphical interface, Makefile, CMake configuration, or automated test suite.

## Possible Extensions

Future work can add file or database persistence, customer search, groups and tags, import and export, automated tests, and CMake or Makefile build automation.

## References

[Customer management requirements](docs/客户信息管理系统.docx)

[C source code](src/)
