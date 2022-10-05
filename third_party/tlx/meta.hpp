/*******************************************************************************
 * tlx/meta.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_HEADER
#define TLX_META_HEADER

//! \defgroup tlx_meta Meta-Template Programming
//! Tools for easier meta-template programming

/*[[[perl
print "#include <$_>\n" foreach sort glob("tlx/meta/"."*.hpp");
]]]*/
#include "meta/apply_tuple.hpp"
#include "meta/call_for_range.hpp"
#include "meta/call_foreach.hpp"
#include "meta/call_foreach_tuple.hpp"
#include "meta/call_foreach_tuple_with_index.hpp"
#include "meta/call_foreach_with_index.hpp"
#include "meta/enable_if.hpp"
#include "meta/fold_left.hpp"
#include "meta/fold_left_tuple.hpp"
#include "meta/fold_right.hpp"
#include "meta/fold_right_tuple.hpp"
#include "meta/function_chain.hpp"
#include "meta/function_stack.hpp"
#include "meta/has_member.hpp"
#include "meta/has_method.hpp"
#include "meta/index_sequence.hpp"
#include "meta/is_std_array.hpp"
#include "meta/is_std_pair.hpp"
#include "meta/is_std_tuple.hpp"
#include "meta/is_std_vector.hpp"
#include "meta/log2.hpp"
#include "meta/no_operation.hpp"
#include "meta/static_index.hpp"
#include "meta/vexpand.hpp"
#include "meta/vmap_for_range.hpp"
#include "meta/vmap_foreach.hpp"
#include "meta/vmap_foreach_tuple.hpp"
#include "meta/vmap_foreach_tuple_with_index.hpp"
#include "meta/vmap_foreach_with_index.hpp"
// [[[end]]]

#endif // !TLX_META_HEADER

/******************************************************************************/
