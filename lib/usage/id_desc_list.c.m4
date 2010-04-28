dnl
dnl lib/usage/id_desc_list.c template.
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
 * @brief HID report descriptor - usage ID description lists
 *
 * vim:nomodifiable
 *
 * ************* DO NOT EDIT ***********************
 * This file is autogenerated from id_desc_list.c.m4
 * *************************************************
 *
 * Copyright (C) 2010 Nikolai Kondrashov
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
 * @(#) $Id: page.c 103 2010-01-18 21:04:26Z spb_nick $
 */

#include <strings.h>
#include "hidrd/util/tkn.h"
#include "hidrd/usage/all.h"
#include "hidrd/usage/id_desc_list.h"

bool
hidrd_usage_id_desc_list_valid(const hidrd_usage_id_desc   *list,
                               size_t                       num)
{
    if (list == NULL && num > 0)
        return false;

    for (; num > 0; list++, num--)
        if (!hidrd_usage_id_desc_valid(list))
            return false;

    return true;
}


const hidrd_usage_id_desc *
hidrd_usage_id_desc_list_lkp_by_value(const hidrd_usage_id_desc    *list,
                                      size_t                        num,
                                      hidrd_usage_id                value)
{
    assert(hidrd_usage_id_desc_list_valid(list, num));

    for (; num > 0; num--, list++)
        if (list->value == value)
            return list;

    return NULL;
}


#ifdef HIDRD_WITH_TOKENS
const hidrd_usage_id_desc *
hidrd_usage_id_desc_list_lkp_by_token(const hidrd_usage_id_desc    *list,
                                      size_t                        num,
                                      const char                   *token)
{
    assert(hidrd_usage_id_desc_list_valid(list, num));
    assert(hidrd_tkn_valid(token));

    for (; num > 0; num--, list++)
        if (strcasecmp(list->token, token) == 0)
            return list;

    return NULL;
}
#endif


#ifdef HIDRD_WITH_TOKENS
#define _U_TOKEN(_token)    .token = _token,
#else
#define _U_TOKEN(_token)
#endif

#ifdef HIDRD_WITH_NAMES
#define _U_NAME(_name)      .name = _name,
#else
#define _U_NAME(_name)
#endif

#define _U(_TOKEN, _token, _name, _type_set) \
    {.value = (hidrd_usage_id)HIDRD_USAGE_##_TOKEN, \
     .type_set  = _type_set,                        \
     _U_TOKEN(#_token) _U_NAME(_name)}

'dnl
pushdef(`TYPE_SET_ITER',
`ifelse(len(`$1'), 0, `',
`HIDRD_USAGE_TYPE_`'uppercase($1)`'dnl
ifelse(len(`$2'), 0, `', ` | TYPE_SET_ITER(shift($@))')')')dnl
pushdef(`TYPE_SET',
`ifelse(len(`$1'), 0, `HIDRD_USAGE_TYPE_SET_EMPTY', `TYPE_SET_ITER($@)')')dnl
dnl
pushdef(`PAGE',
`const hidrd_usage_id_desc   dnl
hidrd_usage_id_desc_list_`'lowercase($2)[PAGE_ID_NUM(`$2')] = {
#define _PU(_TOKEN, _token, _name, _type_set) \
    _U(uppercase($2)_##_TOKEN, _token, _name, _type_set)

pushdef(`ID',
`    _PU(uppercase($'`2), $'`2,
        "$'`4",
        TYPE_SET($'`3)),
')dnl
sinclude(`db/usage/id_'lowercase($2)`.m4')dnl
popdef(`ID')dnl

#undef _PU
};

')dnl
include(`db/usage/page.m4')dnl
popdef(`PAGE')dnl
popdef(`TYPE_SET')dnl
popdef(`TYPE_SET_ITER')dnl
`#undef _U

#undef _U_NAME
#undef _U_TOKEN
'dnl
