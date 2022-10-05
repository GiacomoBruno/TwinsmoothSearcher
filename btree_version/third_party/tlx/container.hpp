/*******************************************************************************
 * tlx/container.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_CONTAINER_HEADER
#define TLX_CONTAINER_HEADER

//! \defgroup tlx_container Containers and Data Structures
//! Containers and Data Structures

/*[[[perl
print "#include <$_>\n" foreach sort glob("tlx/container/"."*.hpp");
]]]*/
#include "container/btree.hpp"
#include "container/btree_map.hpp"
#include "container/btree_multimap.hpp"
#include "container/btree_multiset.hpp"
#include "container/btree_set.hpp"
#include "container/d_ary_addressable_int_heap.hpp"
#include "container/d_ary_heap.hpp"
#include "container/loser_tree.hpp"
#include "container/lru_cache.hpp"
#include "container/radix_heap.hpp"
#include "container/ring_buffer.hpp"
#include "container/simple_vector.hpp"
#include "container/splay_tree.hpp"
#include "container/string_view.hpp"
// [[[end]]]

#endif // !TLX_CONTAINER_HEADER

/******************************************************************************/
