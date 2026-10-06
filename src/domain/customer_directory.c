#include "customer_directory.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int is_blank_text(const char *text)
{
    const unsigned char *cursor;

    if (text == NULL || text[0] == '\0') {
        return 1;
    }

    cursor = (const unsigned char *)text;
    while (*cursor != '\0') {
        if (!isspace(*cursor)) {
            return 0;
        }
        cursor++;
    }

    return 1;
}

static int is_valid_phone(const char *phone)
{
    size_t digit_count = 0U;
    const unsigned char *cursor;

    if (phone == NULL || phone[0] == '\0') {
        return 0;
    }

    cursor = (const unsigned char *)phone;
    while (*cursor != '\0') {
        if (isdigit(*cursor)) {
            digit_count++;
        } else if (*cursor != '+'
            && *cursor != '-'
            && *cursor != '('
            && *cursor != ')'
            && *cursor != ' ') {
            return 0;
        }
        cursor++;
    }

    return digit_count >= 3U;
}

static int is_valid_email(const char *email)
{
    const char *at_sign;
    const char *last_at_sign;
    const char *domain_dot;
    const unsigned char *cursor;

    if (email == NULL || email[0] == '\0') {
        return 0;
    }

    cursor = (const unsigned char *)email;
    while (*cursor != '\0') {
        if (isspace(*cursor)) {
            return 0;
        }
        cursor++;
    }

    at_sign = strchr(email, '@');
    last_at_sign = strrchr(email, '@');
    if (at_sign == NULL || at_sign != last_at_sign || at_sign == email) {
        return 0;
    }

    domain_dot = strchr(at_sign + 1, '.');
    if (domain_dot == NULL
        || domain_dot == at_sign + 1
        || domain_dot[1] == '\0') {
        return 0;
    }

    return at_sign[1] != '\0';
}

static CustomerResult ensure_capacity(CustomerDirectory *directory)
{
    size_t new_capacity;
    Customer *new_customers;

    if (directory->count < directory->capacity) {
        return CUSTOMER_RESULT_OK;
    }

    if (directory->capacity == 0U) {
        new_capacity = CUSTOMER_INITIAL_CAPACITY;
    } else {
        if (directory->capacity > SIZE_MAX / 2U) {
            return CUSTOMER_RESULT_MEMORY_ALLOCATION;
        }
        new_capacity = directory->capacity * 2U;
    }

    if (new_capacity > SIZE_MAX / sizeof(Customer)) {
        return CUSTOMER_RESULT_MEMORY_ALLOCATION;
    }

    new_customers = realloc(
        directory->customers,
        new_capacity * sizeof(Customer)
    );
    if (new_customers == NULL) {
        return CUSTOMER_RESULT_MEMORY_ALLOCATION;
    }

    directory->customers = new_customers;
    directory->capacity = new_capacity;
    return CUSTOMER_RESULT_OK;
}

static size_t find_index(
    const CustomerDirectory *directory,
    int customer_id
)
{
    size_t index;

    if (directory == NULL) {
        return SIZE_MAX;
    }

    for (index = 0U; index < directory->count; index++) {
        if (directory->customers[index].id == customer_id) {
            return index;
        }
    }

    return SIZE_MAX;
}

CustomerResult customer_directory_initialize(CustomerDirectory *directory)
{
    if (directory == NULL) {
        return CUSTOMER_RESULT_INVALID_ARGUMENT;
    }

    directory->customers = NULL;
    directory->count = 0U;
    directory->capacity = 0U;
    return CUSTOMER_RESULT_OK;
}

void customer_directory_destroy(CustomerDirectory *directory)
{
    if (directory == NULL) {
        return;
    }

    free(directory->customers);
    directory->customers = NULL;
    directory->count = 0U;
    directory->capacity = 0U;
}

CustomerResult customer_profile_validate(const CustomerProfile *profile)
{
    if (profile == NULL) {
        return CUSTOMER_RESULT_INVALID_ARGUMENT;
    }

    if (is_blank_text(profile->name)
        || strlen(profile->name) > CUSTOMER_NAME_MAX_LENGTH) {
        return CUSTOMER_RESULT_INVALID_NAME;
    }

    if (profile->gender != CUSTOMER_GENDER_FEMALE
        && profile->gender != CUSTOMER_GENDER_MALE) {
        return CUSTOMER_RESULT_INVALID_GENDER;
    }

    if (profile->age < CUSTOMER_MIN_AGE
        || profile->age > CUSTOMER_MAX_AGE) {
        return CUSTOMER_RESULT_INVALID_AGE;
    }

    if (strlen(profile->phone) > CUSTOMER_PHONE_MAX_LENGTH
        || !is_valid_phone(profile->phone)) {
        return CUSTOMER_RESULT_INVALID_PHONE;
    }

    if (strlen(profile->email) > CUSTOMER_EMAIL_MAX_LENGTH
        || !is_valid_email(profile->email)) {
        return CUSTOMER_RESULT_INVALID_EMAIL;
    }

    return CUSTOMER_RESULT_OK;
}

CustomerResult customer_directory_add(
    CustomerDirectory *directory,
    const CustomerProfile *profile,
    int *customer_id
)
{
    CustomerResult result;
    Customer *customer;

    if (directory == NULL || profile == NULL) {
        return CUSTOMER_RESULT_INVALID_ARGUMENT;
    }

    result = customer_profile_validate(profile);
    if (result != CUSTOMER_RESULT_OK) {
        return result;
    }

    result = ensure_capacity(directory);
    if (result != CUSTOMER_RESULT_OK) {
        return result;
    }

    customer = &directory->customers[directory->count];
    customer->id = (int)directory->count + 1;
    customer->profile = *profile;
    directory->count++;

    if (customer_id != NULL) {
        *customer_id = customer->id;
    }

    return CUSTOMER_RESULT_OK;
}

CustomerResult customer_directory_update(
    CustomerDirectory *directory,
    int customer_id,
    const CustomerProfile *profile
)
{
    CustomerResult result;
    size_t index;

    if (directory == NULL || profile == NULL) {
        return CUSTOMER_RESULT_INVALID_ARGUMENT;
    }

    index = find_index(directory, customer_id);
    if (index == SIZE_MAX) {
        return CUSTOMER_RESULT_NOT_FOUND;
    }

    result = customer_profile_validate(profile);
    if (result != CUSTOMER_RESULT_OK) {
        return result;
    }

    directory->customers[index].profile = *profile;
    return CUSTOMER_RESULT_OK;
}

CustomerResult customer_directory_remove(
    CustomerDirectory *directory,
    int customer_id
)
{
    size_t index;
    size_t move_index;

    if (directory == NULL) {
        return CUSTOMER_RESULT_INVALID_ARGUMENT;
    }

    index = find_index(directory, customer_id);
    if (index == SIZE_MAX) {
        return CUSTOMER_RESULT_NOT_FOUND;
    }

    for (move_index = index + 1U;
        move_index < directory->count;
        move_index++) {
        directory->customers[move_index - 1U] =
            directory->customers[move_index];
        directory->customers[move_index - 1U].id =
            (int)move_index;
    }

    directory->count--;
    if (directory->count > 0U) {
        directory->customers[directory->count - 1U].id =
            (int)directory->count;
    }
    memset(
        &directory->customers[directory->count],
        0,
        sizeof(Customer)
    );

    return CUSTOMER_RESULT_OK;
}

size_t customer_directory_count(const CustomerDirectory *directory)
{
    return directory == NULL ? 0U : directory->count;
}

const Customer *customer_directory_get(
    const CustomerDirectory *directory,
    size_t index
)
{
    if (directory == NULL || index >= directory->count) {
        return NULL;
    }

    return &directory->customers[index];
}

const Customer *customer_directory_find(
    const CustomerDirectory *directory,
    int customer_id
)
{
    size_t index = find_index(directory, customer_id);

    if (index == SIZE_MAX) {
        return NULL;
    }

    return &directory->customers[index];
}

const char *customer_result_message(CustomerResult result)
{
    switch (result) {
    case CUSTOMER_RESULT_OK:
        return "操作成功。";
    case CUSTOMER_RESULT_INVALID_ARGUMENT:
        return "操作参数无效。";
    case CUSTOMER_RESULT_NOT_FOUND:
        return "客户编号不存在。";
    case CUSTOMER_RESULT_MEMORY_ALLOCATION:
        return "系统内存不足，无法保存客户信息。";
    case CUSTOMER_RESULT_INVALID_NAME:
        return "姓名不能为空，且长度不能超出限制。";
    case CUSTOMER_RESULT_INVALID_GENDER:
        return "性别只能填写 f 或 m。";
    case CUSTOMER_RESULT_INVALID_AGE:
        return "年龄必须在 1 到 150 岁之间。";
    case CUSTOMER_RESULT_INVALID_PHONE:
        return "电话号码格式不正确。";
    case CUSTOMER_RESULT_INVALID_EMAIL:
        return "邮箱格式不正确。";
    default:
        return "发生未知业务错误。";
    }
}
