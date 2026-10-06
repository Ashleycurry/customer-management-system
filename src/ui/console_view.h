#ifndef CUSTOMER_MANAGEMENT_CONSOLE_VIEW_H
#define CUSTOMER_MANAGEMENT_CONSOLE_VIEW_H

#include "../domain/customer_directory.h"

/**
 * @brief 显示客户管理系统主菜单。
 */
void console_view_show_main_menu(void);

/**
 * @brief 显示客户列表。
 *
 * @param directory 待展示的客户目录。
 */
void console_view_show_customer_list(
    const CustomerDirectory *directory
);

/**
 * @brief 显示单个客户的详细资料。
 *
 * @param customer 待展示的客户。
 */
void console_view_show_customer(const Customer *customer);

/**
 * @brief 显示业务错误提示。
 *
 * @param result 业务处理结果。
 */
void console_view_show_result(CustomerResult result);

/**
 * @brief 显示客户添加成功提示。
 *
 * @param customer_id 新客户编号。
 */
void console_view_show_customer_added(int customer_id);

/**
 * @brief 显示客户修改成功提示。
 */
void console_view_show_customer_updated(void);

/**
 * @brief 显示客户删除成功提示。
 */
void console_view_show_customer_removed(void);

/**
 * @brief 显示程序退出提示。
 */
void console_view_show_exit_message(void);

#endif
