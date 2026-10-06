#include "customer_app.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../domain/customer_directory.h"
#include "../ui/console_input.h"
#include "../ui/console_view.h"

typedef struct {
    CustomerDirectory directory;
    bool running;
} CustomerApplication;

static bool read_new_customer_profile(CustomerProfile *profile)
{
    CustomerResult validation_result;
    ConsoleInputResult input_result;

    if (profile == NULL) {
        return false;
    }

    for (;;) {
        input_result = console_read_required_text(
            "\n姓名：",
            profile->name,
            sizeof(profile->name)
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            return false;
        }

        input_result = console_read_gender(
            "性别（f 或 m）：",
            false,
            &profile->gender,
            NULL
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            return false;
        }

        input_result = console_read_integer(
            "年龄：",
            CUSTOMER_MIN_AGE,
            CUSTOMER_MAX_AGE,
            &profile->age
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            return false;
        }

        input_result = console_read_required_text(
            "电话：",
            profile->phone,
            sizeof(profile->phone)
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            return false;
        }

        input_result = console_read_required_text(
            "邮箱：",
            profile->email,
            sizeof(profile->email)
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            return false;
        }

        validation_result = customer_profile_validate(profile);
        if (validation_result == CUSTOMER_RESULT_OK) {
            return true;
        }

        console_view_show_result(validation_result);
        puts("请重新填写客户资料。");
    }
}

static bool read_customer_id(
    const char *prompt,
    int *customer_id
)
{
    return console_read_integer(
        prompt,
        -1,
        INT_MAX,
        customer_id
    ) != CONSOLE_INPUT_EOF;
}

static bool update_customer_profile(
    const Customer *current_customer,
    CustomerProfile *updated_profile
)
{
    bool changed;
    ConsoleInputResult input_result;

    if (current_customer == NULL || updated_profile == NULL) {
        return false;
    }

    *updated_profile = current_customer->profile;
    console_view_show_customer(current_customer);
    puts("\n直接回车表示保留当前值。");

    input_result = console_read_optional_text(
        "姓名：",
        updated_profile->name,
        sizeof(updated_profile->name),
        &changed
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        return false;
    }

    input_result = console_read_gender(
        "性别（f 或 m）：",
        true,
        &updated_profile->gender,
        &changed
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        return false;
    }

    input_result = console_read_optional_integer(
        "年龄：",
        CUSTOMER_MIN_AGE,
        CUSTOMER_MAX_AGE,
        &updated_profile->age,
        &changed
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        return false;
    }

    input_result = console_read_optional_text(
        "电话：",
        updated_profile->phone,
        sizeof(updated_profile->phone),
        &changed
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        return false;
    }

    input_result = console_read_optional_text(
        "邮箱：",
        updated_profile->email,
        sizeof(updated_profile->email),
        &changed
    );
    return input_result != CONSOLE_INPUT_EOF;
}

static void handle_add(CustomerApplication *application)
{
    CustomerProfile profile = {0};
    CustomerResult result;
    int customer_id;

    puts("\n--------------------- 添加客户 ---------------------");
    if (!read_new_customer_profile(&profile)) {
        application->running = false;
        return;
    }

    result = customer_directory_add(
        &application->directory,
        &profile,
        &customer_id
    );
    if (result != CUSTOMER_RESULT_OK) {
        console_view_show_result(result);
        return;
    }

    console_view_show_customer_added(customer_id);
}

static void handle_edit(CustomerApplication *application)
{
    CustomerProfile updated_profile;
    const Customer *customer;
    CustomerResult result;
    int customer_id;

    puts("\n--------------------- 修改客户 ---------------------");
    if (!read_customer_id(
        "请选择待修改客户编号（-1 退出）：",
        &customer_id
    )) {
        application->running = false;
        return;
    }

    if (customer_id == -1) {
        puts("\n你放弃了修改。");
        return;
    }

    customer = customer_directory_find(
        &application->directory,
        customer_id
    );
    if (customer == NULL) {
        console_view_show_result(CUSTOMER_RESULT_NOT_FOUND);
        return;
    }

    if (!update_customer_profile(customer, &updated_profile)) {
        application->running = false;
        return;
    }

    result = customer_profile_validate(&updated_profile);
    if (result != CUSTOMER_RESULT_OK) {
        console_view_show_result(result);
        return;
    }

    result = customer_directory_update(
        &application->directory,
        customer_id,
        &updated_profile
    );
    if (result != CUSTOMER_RESULT_OK) {
        console_view_show_result(result);
        return;
    }

    console_view_show_customer_updated();
}

static void handle_remove(CustomerApplication *application)
{
    const Customer *customer;
    CustomerResult result;
    bool confirmed;
    int customer_id;
    ConsoleInputResult input_result;

    puts("\n--------------------- 删除客户 ---------------------");
    if (!read_customer_id(
        "请选择待删除客户编号（-1 退出）：",
        &customer_id
    )) {
        application->running = false;
        return;
    }

    if (customer_id == -1) {
        puts("\n你放弃了删除。");
        return;
    }

    customer = customer_directory_find(
        &application->directory,
        customer_id
    );
    if (customer == NULL) {
        console_view_show_result(CUSTOMER_RESULT_NOT_FOUND);
        return;
    }

    console_view_show_customer(customer);
    input_result = console_read_confirmation(
        "确认是否删除（y/n）：",
        &confirmed
    );
    if (input_result == CONSOLE_INPUT_EOF) {
        application->running = false;
        return;
    }
    if (!confirmed) {
        puts("\n你放弃了删除。");
        return;
    }

    result = customer_directory_remove(
        &application->directory,
        customer_id
    );
    if (result != CUSTOMER_RESULT_OK) {
        console_view_show_result(result);
        return;
    }

    console_view_show_customer_removed();
}

static void handle_exit(CustomerApplication *application)
{
    bool confirmed;
    ConsoleInputResult input_result;

    input_result = console_read_confirmation(
        "\n确认是否退出（y/n）：",
        &confirmed
    );
    if (input_result == CONSOLE_INPUT_EOF || confirmed) {
        application->running = false;
    }
}

int customer_app_run(void)
{
    CustomerApplication application = {0};
    CustomerResult result;

    result = customer_directory_initialize(&application.directory);
    if (result != CUSTOMER_RESULT_OK) {
        console_view_show_result(result);
        return EXIT_FAILURE;
    }

    application.running = true;
    while (application.running) {
        int menu_choice;
        ConsoleInputResult input_result;

        console_view_show_main_menu();
        input_result = console_read_integer(
            "请选择（1-5）：",
            1,
            5,
            &menu_choice
        );
        if (input_result == CONSOLE_INPUT_EOF) {
            application.running = false;
            break;
        }

        switch (menu_choice) {
        case 1:
            handle_add(&application);
            break;
        case 2:
            handle_edit(&application);
            break;
        case 3:
            handle_remove(&application);
            break;
        case 4:
            console_view_show_customer_list(&application.directory);
            break;
        case 5:
            handle_exit(&application);
            break;
        default:
            break;
        }
    }

    customer_directory_destroy(&application.directory);
    console_view_show_exit_message();
    return EXIT_SUCCESS;
}
