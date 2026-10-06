#ifndef CUSTOMER_MANAGEMENT_CONSOLE_INPUT_H
#define CUSTOMER_MANAGEMENT_CONSOLE_INPUT_H

#include <stdbool.h>
#include <stddef.h>

#include "../domain/customer_types.h"

typedef enum {
    CONSOLE_INPUT_OK = 0,
    CONSOLE_INPUT_EOF,
    CONSOLE_INPUT_INVALID,
    CONSOLE_INPUT_TOO_LONG
} ConsoleInputResult;

/**
 * @brief 读取指定范围内的整数。
 *
 * 函数会持续读取，直到用户输入合法整数或遇到输入结束。
 *
 * @param prompt 提示文本。
 * @param minimum 最小允许值。
 * @param maximum 最大允许值。
 * @param value 输出整数。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_integer(
    const char *prompt,
    int minimum,
    int maximum,
    int *value
);

/**
 * @brief 读取必填文本。
 *
 * @param prompt 提示文本。
 * @param buffer 输出缓冲区。
 * @param buffer_size 缓冲区容量。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_required_text(
    const char *prompt,
    char *buffer,
    size_t buffer_size
);

/**
 * @brief 读取可选文本。
 *
 * 用户直接回车时保持原值，并将 changed 设置为 false。
 *
 * @param prompt 提示文本。
 * @param buffer 输出缓冲区。
 * @param buffer_size 缓冲区容量。
 * @param changed 输出是否提供了新值。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_optional_text(
    const char *prompt,
    char *buffer,
    size_t buffer_size,
    bool *changed
);

/**
 * @brief 读取可选整数。
 *
 * 用户直接回车时保持原值，并将 changed 设置为 false。
 *
 * @param prompt 提示文本。
 * @param minimum 最小允许值。
 * @param maximum 最大允许值。
 * @param value 输出整数。
 * @param changed 输出是否提供了新值。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_optional_integer(
    const char *prompt,
    int minimum,
    int maximum,
    int *value,
    bool *changed
);

/**
 * @brief 读取性别。
 *
 * 接受 f/F 或 m/M；允许空行时，空行表示保留原值。
 *
 * @param prompt 提示文本。
 * @param allow_empty 是否允许空行。
 * @param gender 输出性别。
 * @param changed 输出是否提供了新值。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_gender(
    const char *prompt,
    bool allow_empty,
    CustomerGender *gender,
    bool *changed
);

/**
 * @brief 读取 y/n 确认。
 *
 * @param prompt 提示文本。
 * @param confirmed 输出是否确认。
 * @return 输入处理结果。
 */
ConsoleInputResult console_read_confirmation(
    const char *prompt,
    bool *confirmed
);

#endif
