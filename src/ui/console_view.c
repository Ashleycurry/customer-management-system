#include "console_view.h"

#include <stdio.h>

static const char *gender_name(CustomerGender gender)
{
    switch (gender) {
    case CUSTOMER_GENDER_FEMALE:
        return "女";
    case CUSTOMER_GENDER_MALE:
        return "男";
    default:
        return "未知";
    }
}

void console_view_show_main_menu(void)
{
    puts("\n\n-------------- 客户信息管理系统 --------------");
    puts("1. 添加客户");
    puts("2. 修改客户");
    puts("3. 删除客户");
    puts("4. 客户列表");
    puts("5. 退出");
}

void console_view_show_customer_list(
    const CustomerDirectory *directory
)
{
    size_t index;
    size_t customer_count;

    if (directory == NULL) {
        puts("客户目录不可用。");
        return;
    }

    customer_count = customer_directory_count(directory);
    if (customer_count == 0U) {
        puts("\n还没有客户信息，快去添加吧！");
        return;
    }

    puts("\n------------------- 客户列表 -------------------");
    printf(
        "%-6s %-18s %-8s %-8s %-18s %s\n",
        "编号",
        "姓名",
        "性别",
        "年龄",
        "电话",
        "邮箱"
    );

    for (index = 0U; index < customer_count; index++) {
        const Customer *customer =
            customer_directory_get(directory, index);

        if (customer == NULL) {
            continue;
        }

        printf(
            "%-6d %-18s %-8s %-8d %-18s %s\n",
            customer->id,
            customer->profile.name,
            gender_name(customer->profile.gender),
            customer->profile.age,
            customer->profile.phone,
            customer->profile.email
        );
    }
}

void console_view_show_customer(const Customer *customer)
{
    if (customer == NULL) {
        puts("客户信息不可用。");
        return;
    }

    puts("\n---------------- 客户信息 ----------------");
    printf("编号：%d\n", customer->id);
    printf("姓名：%s\n", customer->profile.name);
    printf("性别：%s\n", gender_name(customer->profile.gender));
    printf("年龄：%d\n", customer->profile.age);
    printf("电话：%s\n", customer->profile.phone);
    printf("邮箱：%s\n", customer->profile.email);
}

void console_view_show_result(CustomerResult result)
{
    printf("\n%s\n", customer_result_message(result));
}

void console_view_show_customer_added(int customer_id)
{
    printf("\n客户添加成功，客户编号：%d。\n", customer_id);
}

void console_view_show_customer_updated(void)
{
    puts("\n客户修改成功。");
}

void console_view_show_customer_removed(void)
{
    puts("\n客户删除成功。");
}

void console_view_show_exit_message(void)
{
    puts("\n你退出了客户信息管理系统。");
}
