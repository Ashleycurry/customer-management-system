#ifndef CUSTOMER_MANAGEMENT_CUSTOMER_DIRECTORY_H
#define CUSTOMER_MANAGEMENT_CUSTOMER_DIRECTORY_H

#include <stddef.h>

#include "customer_types.h"

/**
 * @brief 初始化客户目录。
 *
 * @param directory 待初始化的客户目录。
 * @return 初始化结果。
 */
CustomerResult customer_directory_initialize(CustomerDirectory *directory);

/**
 * @brief 释放客户目录占用的动态内存。
 *
 * @param directory 待销毁的客户目录。
 */
void customer_directory_destroy(CustomerDirectory *directory);

/**
 * @brief 校验客户资料。
 *
 * @param profile 待校验的客户资料。
 * @return 校验结果。
 */
CustomerResult customer_profile_validate(const CustomerProfile *profile);

/**
 * @brief 添加客户。
 *
 * @param directory 目标客户目录。
 * @param profile 待添加的客户资料。
 * @param customer_id 输出新客户编号，可传入 NULL。
 * @return 添加结果。
 */
CustomerResult customer_directory_add(
    CustomerDirectory *directory,
    const CustomerProfile *profile,
    int *customer_id
);

/**
 * @brief 修改指定编号的客户资料。
 *
 * @param directory 目标客户目录。
 * @param customer_id 待修改的客户编号。
 * @param profile 新的客户资料。
 * @return 修改结果。
 */
CustomerResult customer_directory_update(
    CustomerDirectory *directory,
    int customer_id,
    const CustomerProfile *profile
);

/**
 * @brief 删除指定编号的客户。
 *
 * 删除后会重新整理后续客户编号，使编号保持从 1 开始连续排列。
 *
 * @param directory 目标客户目录。
 * @param customer_id 待删除的客户编号。
 * @return 删除结果。
 */
CustomerResult customer_directory_remove(
    CustomerDirectory *directory,
    int customer_id
);

/**
 * @brief 获取客户数量。
 *
 * @param directory 目标客户目录。
 * @return 客户数量；传入 NULL 时返回 0。
 */
size_t customer_directory_count(const CustomerDirectory *directory);

/**
 * @brief 按索引读取客户记录。
 *
 * @param directory 目标客户目录。
 * @param index 客户索引。
 * @return 只读客户指针；索引无效时返回 NULL。
 */
const Customer *customer_directory_get(
    const CustomerDirectory *directory,
    size_t index
);

/**
 * @brief 按编号查找客户。
 *
 * @param directory 目标客户目录。
 * @param customer_id 客户编号。
 * @return 只读客户指针；找不到时返回 NULL。
 */
const Customer *customer_directory_find(
    const CustomerDirectory *directory,
    int customer_id
);

/**
 * @brief 获取业务结果对应的中文提示。
 *
 * @param result 业务处理结果。
 * @return 稳定的中文提示文本。
 */
const char *customer_result_message(CustomerResult result);

#endif
