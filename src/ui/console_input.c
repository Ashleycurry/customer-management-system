#include "console_input.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONSOLE_LINE_BUFFER_SIZE 512U

static void discard_remaining_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);
}

static ConsoleInputResult read_line(
    const char *prompt,
    char *buffer,
    size_t buffer_size
)
{
    char *line_end;

    if (prompt == NULL || buffer == NULL || buffer_size < 2U) {
        return CONSOLE_INPUT_INVALID;
    }

    fputs(prompt, stdout);
    fflush(stdout);

    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return CONSOLE_INPUT_EOF;
    }

    line_end = strpbrk(buffer, "\r\n");
    if (line_end == NULL) {
        discard_remaining_line();
        return CONSOLE_INPUT_TOO_LONG;
    }

    *line_end = '\0';
    return CONSOLE_INPUT_OK;
}

static char *trim_whitespace(char *text)
{
    char *start = text;
    char *end;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';

    return start;
}

static bool parse_integer(const char *text, int *value)
{
    char *end;
    long parsed_value;

    if (text == NULL || value == NULL || text[0] == '\0') {
        return false;
    }

    errno = 0;
    parsed_value = strtol(text, &end, 10);
    if (errno == ERANGE
        || end == text
        || *end != '\0'
        || parsed_value < INT_MIN
        || parsed_value > INT_MAX) {
        return false;
    }

    *value = (int)parsed_value;
    return true;
}

static ConsoleInputResult read_integer_line(
    const char *prompt,
    int minimum,
    int maximum,
    int *value,
    bool allow_empty,
    bool *changed
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;
    int parsed_value;

    if (minimum > maximum || value == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (allow_empty && normalized_line[0] == '\0') {
            if (changed != NULL) {
                *changed = false;
            }
            return CONSOLE_INPUT_OK;
        }

        if (parse_integer(normalized_line, &parsed_value)
            && parsed_value >= minimum
            && parsed_value <= maximum) {
            *value = parsed_value;
            if (changed != NULL) {
                *changed = true;
            }
            return CONSOLE_INPUT_OK;
        }

        printf("请输入 %d-%d 之间的整数。\n", minimum, maximum);
    }
}

static ConsoleInputResult read_text_line(
    const char *prompt,
    char *buffer,
    size_t buffer_size,
    bool allow_empty,
    bool *changed
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;

    if (buffer == NULL || buffer_size < 2U) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (allow_empty && normalized_line[0] == '\0') {
            if (changed != NULL) {
                *changed = false;
            }
            return CONSOLE_INPUT_OK;
        }

        if (normalized_line[0] == '\0') {
            puts("输入不能为空，请重新输入。");
            continue;
        }
        if (strlen(normalized_line) >= buffer_size) {
            puts("输入内容超出长度限制，请重新输入。");
            continue;
        }

        strcpy(buffer, normalized_line);
        if (changed != NULL) {
            *changed = true;
        }
        return CONSOLE_INPUT_OK;
    }
}

ConsoleInputResult console_read_integer(
    const char *prompt,
    int minimum,
    int maximum,
    int *value
)
{
    return read_integer_line(
        prompt,
        minimum,
        maximum,
        value,
        false,
        NULL
    );
}

ConsoleInputResult console_read_required_text(
    const char *prompt,
    char *buffer,
    size_t buffer_size
)
{
    return read_text_line(
        prompt,
        buffer,
        buffer_size,
        false,
        NULL
    );
}

ConsoleInputResult console_read_optional_text(
    const char *prompt,
    char *buffer,
    size_t buffer_size,
    bool *changed
)
{
    return read_text_line(
        prompt,
        buffer,
        buffer_size,
        true,
        changed
    );
}

ConsoleInputResult console_read_optional_integer(
    const char *prompt,
    int minimum,
    int maximum,
    int *value,
    bool *changed
)
{
    return read_integer_line(
        prompt,
        minimum,
        maximum,
        value,
        true,
        changed
    );
}

ConsoleInputResult console_read_gender(
    const char *prompt,
    bool allow_empty,
    CustomerGender *gender,
    bool *changed
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;
    char normalized_gender;

    if (gender == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请重新输入。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (allow_empty && normalized_line[0] == '\0') {
            if (changed != NULL) {
                *changed = false;
            }
            return CONSOLE_INPUT_OK;
        }

        if (strlen(normalized_line) == 1U) {
            normalized_gender = (char)tolower(
                (unsigned char)normalized_line[0]
            );
            if (normalized_gender == 'f' || normalized_gender == 'm') {
                *gender = (CustomerGender)normalized_gender;
                if (changed != NULL) {
                    *changed = true;
                }
                return CONSOLE_INPUT_OK;
            }
        }

        puts("性别只能输入 f 或 m，请重新输入。");
    }
}

ConsoleInputResult console_read_confirmation(
    const char *prompt,
    bool *confirmed
)
{
    char line[CONSOLE_LINE_BUFFER_SIZE];
    char *normalized_line;
    ConsoleInputResult result;
    char choice;

    if (confirmed == NULL) {
        return CONSOLE_INPUT_INVALID;
    }

    for (;;) {
        result = read_line(prompt, line, sizeof(line));
        if (result == CONSOLE_INPUT_EOF) {
            return result;
        }
        if (result == CONSOLE_INPUT_TOO_LONG) {
            puts("输入内容过长，请输入 y 或 n。");
            continue;
        }

        normalized_line = trim_whitespace(line);
        if (strlen(normalized_line) == 1U) {
            choice = (char)tolower((unsigned char)normalized_line[0]);
            if (choice == 'y' || choice == 'n') {
                *confirmed = choice == 'y';
                return CONSOLE_INPUT_OK;
            }
        }

        puts("输入有误，请输入 y 或 n。");
    }
}
