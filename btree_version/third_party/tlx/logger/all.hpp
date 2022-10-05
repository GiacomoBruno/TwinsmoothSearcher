/*******************************************************************************
 * tlx/logger/all.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_LOGGER_ALL_HEADER
#define TLX_LOGGER_ALL_HEADER

/*[[[perl
foreach (sort glob("tlx/logger/"."*.hpp")) {
  next if $_ eq "tlx/logger/all.hpp";
  print "#include <$_>\n";
}
]]]*/
#include "array.hpp"
#include "core.hpp"
#include "deque.hpp"
#include "map.hpp"
#include "set.hpp"
#include "tuple.hpp"
#include "unordered_map.hpp"
#include "unordered_set.hpp"
#include "wrap_unprintable.hpp"
// [[[end]]]

#endif // !TLX_LOGGER_ALL_HEADER

/******************************************************************************/
