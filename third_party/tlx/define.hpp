/*******************************************************************************
 * tlx/define.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_HEADER
#define TLX_DEFINE_HEADER

//! \defgroup tlx_define Defines and Macros
//! Attribute macros and other defines

/*[[[perl
print "#include <$_>\n" foreach sort glob("tlx/define/"."*.hpp");
]]]*/
#include "define/attribute_always_inline.hpp"
#include "define/attribute_fallthrough.hpp"
#include "define/attribute_format_printf.hpp"
#include "define/attribute_packed.hpp"
#include "define/attribute_warn_unused_result.hpp"
#include "define/constexpr.hpp"
#include "define/deprecated.hpp"
#include "define/endian.hpp"
#include "define/likely.hpp"
#include "define/visibility_hidden.hpp"
// [[[end]]]

#endif // !TLX_DEFINE_HEADER

/******************************************************************************/
