/*******************************************************************************
 * tlx/math.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_HEADER
#define TLX_MATH_HEADER

//! \defgroup tlx_math Math Functions
//! Simple math functions

/*[[[perl
print "#include <$_>\n" foreach sort glob("tlx/math/"."*.hpp");
]]]*/
#include "math/abs_diff.hpp"
#include "math/aggregate.hpp"
#include "math/aggregate_min_max.hpp"
#include "math/bswap.hpp"
#include "math/bswap_be.hpp"
#include "math/bswap_le.hpp"
#include "math/clz.hpp"
#include "math/ctz.hpp"
#include "math/div_ceil.hpp"
#include "math/ffs.hpp"
#include "math/integer_log2.hpp"
#include "math/is_power_of_two.hpp"
#include "math/polynomial_regression.hpp"
#include "math/popcount.hpp"
#include "math/power_to_the.hpp"
#include "math/rol.hpp"
#include "math/ror.hpp"
#include "math/round_to_power_of_two.hpp"
#include "math/round_up.hpp"
#include "math/sgn.hpp"
// [[[end]]]

#endif // !TLX_MATH_HEADER

/******************************************************************************/
