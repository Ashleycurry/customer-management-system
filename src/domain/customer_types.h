#ifndef CUSTOMER_MANAGEMENT_CUSTOMER_TYPES_H
#define CUSTOMER_MANAGEMENT_CUSTOMER_TYPES_H

#include <stddef.h>

#define CUSTOMER_NAME_MAX_LENGTH 63U
#define CUSTOMER_PHONE_MAX_LENGTH 31U
#define CUSTOMER_EMAIL_MAX_LENGTH 127U
#define CUSTOMER_INITIAL_CAPACITY 8U
#define CUSTOMER_MIN_AGE 1
#define CUSTOMER_MAX_AGE 150

/**
 * @brief 客户性别。
 */
typedef enum {
    CUSTOMER_GENDER_FEMALE = 'f',
    CUSTOMER_GENDER_MALE = 'm'
} CustomerGender;

/**
 * @brief 客户基础资料。
 */
typedef struct {
    char name[CUSTOMER_NAME_MAX_LENGTH + 1U];
    CustomerGender gender;
    int age;
    char phone[CUSTOMER_PHONE_MAX_LENGTH + 1U];
    char email[CUSTOMER_EMAIL_MAX_LENGTH + 1U];
} CustomerProfile;

/**
 * @brief 带系统编号的客户记录。
 */
typedef struct {
    int id;
    CustomerProfile profile;
} Customer;

/**
 * @brief 客户目录的业务处理结果。
 */
typedef enum {
    CUSTOMER_RESULT_OK = 0,
    CUSTOMER_RESULT_INVALID_ARGUMENT,
    CUSTOMER_RESULT_NOT_FOUND,
    CUSTOMER_RESULT_MEMORY_ALLOCATION,
    CUSTOMER_RESULT_INVALID_NAME,
    CUSTOMER_RESULT_INVALID_GENDER,
    CUSTOMER_RESULT_INVALID_AGE,
    CUSTOMER_RESULT_INVALID_PHONE,
    CUSTOMER_RESULT_INVALID_EMAIL
} CustomerResult;

/**
 * @brief 客户目录。
 *
 * 目录负责维护客户记录集合及其生命周期，不向界面层暴露内部数组。
 */
typedef struct {
    Customer *customers;
    size_t count;
    size_t capacity;
} CustomerDirectory;

#endif
