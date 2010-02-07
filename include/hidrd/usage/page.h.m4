dnl
dnl hidrd/usage/page.h template.
dnl
dnl Copyright (C) 2010 Nikolai Kondrashov
dnl
dnl This file is part of hidrd.
dnl
dnl Hidrd is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl Hidrd is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with hidrd; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
dnl
dnl
include(`m4/hidrd/util.m4')dnl
dnl
`/** @file
 * @brief HID report descriptor - usage pages
 *
 * vim:nomodifiable
 *
 * *** DO NOT EDIT *************************
 * This file is autogenerated from page.h.m4
 * *****************************************
 *
 * Copyright (C) 2009-2010 Nikolai Kondrashov
 *
 * This file is part of hidrd.
 *
 * Hidrd is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hidrd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hidrd; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @author Nikolai Kondrashov <spbnick@gmail.com>
 *
 * @(#) $Id: page.h 103 2010-01-18 21:04:26Z spb_nick $
 */

#ifndef __HIDRD_USAGE_PAGE_H__
#define __HIDRD_USAGE_PAGE_H__

#include <assert.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hidrd_usage_page {
    /** Undefined */
    HIDRD_USAGE_PAGE_UNDEFINED = 0x0000,
'dnl Delegated pages
pushdef(PAGE,dnl
    `/** capitalize_first(`$3') */'
    `HIDRD_USAGE_PAGE_`'uppercase($2) = 0x$1,
')dnl
include(`db/usage/page.m4')dnl
popdef(`PAGE')dnl
`    /** Invalid */
    HIDRD_USAGE_PAGE_INVALID = 0x10000
} hidrd_usage_page;

#define HIDRD_USAGE_PAGE_MIN    0x0000
#define HIDRD_USAGE_PAGE_MAX    0xFFFF

/**
 * Check if a page is valid.
 *
 * @param page  Page ID to check.
 *
 * @return True if the page is valid, false otherwise.
 */
static inline bool
hidrd_usage_page_valid(hidrd_usage_page page)
{
    hidrd_usage_page    min = HIDRD_USAGE_PAGE_MIN;
    hidrd_usage_page    max = HIDRD_USAGE_PAGE_MAX;

    return (page >= min) && (page <= max);
}


/**
 * Validate a usage page ID.
 *
 * @param page  Page ID to validate.
 *
 * @return Validated page ID.
 */
static inline hidrd_usage_page
hidrd_usage_page_validate(hidrd_usage_page page)
{
    assert(hidrd_usage_page_valid(page));
    return page;
}


/**
 * Check if a page is defined (not undefined).
 *
 * @param page  Page ID to check.
 *
 * @return True if the page is defined, false otherwise.
 */
static inline bool
hidrd_usage_page_defined(hidrd_usage_page page)
{
    assert(hidrd_usage_page_valid(page));
    return page != HIDRD_USAGE_PAGE_UNDEFINED;
}


'define(`PAGE_SET',
`/**
 * Check if a page is $2.
 *
 * @param page  Page ID to check.
 *
 * @return True if the page is $2, false otherwise.
 */
ifelse(eval(PAGE_SET_RANGE_NUM($1) > 1), 1,
extern bool hidrd_usage_page_$1(hidrd_usage_page page);,
static inline bool
hidrd_usage_page_$1(hidrd_usage_page page)
{
    assert(hidrd_usage_page_valid(page));
PAGE_SET_RANGE_CHECK($1)
}
)

')dnl
include(`db/usage/page_set.m4')dnl
undefine(`PAGE_SET')dnl
`
/**
 * Convert a usage page hexadecimal code string to a code.
 *
 * @param token Usage page hexadecimal code string.
 *
 * @return Usage page code; will be HIDRD_USAGE_PAGE_INVALID, if the
 *         hexadecimal code string is invalid.
 */
extern hidrd_usage_page hidrd_usage_page_from_hex(const char *hex);

/**
 * Convert a usage page code to a hexadecimal code string.
 *
 * @param page  Usage page code.
 *
 * @return Dynamically allocated hexadecimal page code string.
 */
extern char *hidrd_usage_page_to_hex(hidrd_usage_page page);

#ifdef HIDRD_WITH_TOKENS

/**
 * Convert a usage page code to a string token.
 *
 * @param page  Usage page code.
 *
 * @return Constant token string or NULL if the page has no token.
 */
extern const char *hidrd_usage_page_to_token(hidrd_usage_page page);

/**
 * Convert a usage page code to a string token or (if there is no token) to
 * a hexadecimal code string.
 *
 * @param page  Usage page code.
 *
 * @return Dynamically allocated usage page token or hexadecimal code
 *         string; NULL if failed to allocate memory.
 */
extern char *hidrd_usage_page_to_token_or_hex(hidrd_usage_page page);

/**
 * Convert a usage page string token to a code.
 *
 * @param token Usage page string token (case insensitive).
 *
 * @return Usage page code; will be HIDRD_USAGE_PAGE_INVALID, if the token
 *         is not recognized.
 */
extern hidrd_usage_page hidrd_usage_page_from_token(const char *token);

/**
 * Convert a usage page token or (if the token is not recognized)
 * hexadecimal code string to a code.
 *
 * @param token_or_hex  Usage page token or hexadecimal code string.
 *
 * @return Usage page code; will be HIDRD_USAGE_PAGE_INVALID, if the
 *         token is not recognized and the string is not a valid hexadecimal
 *         code.
 */
extern hidrd_usage_page hidrd_usage_page_from_token_or_hex(
                                        const char *token_or_hex);

#endif /* HIDRD_WITH_TOKENS */

#ifdef HIDRD_WITH_NAMES

/**
 * Retrieve usage page name string (close to specification).
 *
 * @param page  Usage page code.
 *
 * @return Usage page name string, or NULL if not found.
 */
extern const char *hidrd_usage_page_name(hidrd_usage_page page);

/**
 * Generate page description from page name and set membership.
 *
 * @param page  Usage page code to generate description for.
 *
 * @return Dynamically allocated page description, or NULL if failed to
 *         allocate memory; could be an empty string, if there is nothing to
 *         tell about the page.
 */
extern char *hidrd_usage_page_desc(hidrd_usage_page page);

#endif /* HIDRD_WITH_NAMES */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_USAGE_PAGE_H__ */
'dnl
