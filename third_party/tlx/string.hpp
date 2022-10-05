/*******************************************************************************
 * tlx/string.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_HEADER
#define TLX_STRING_HEADER

//! \defgroup tlx_string String Algorithms
//! Simple string manipulations

/*[[[perl
my %exclude = ("tlx/string/appendline.hpp" => 1,
               "tlx/string/ssprintf_generic.hpp" => 1);
print "#include <$_>\n"
    foreach grep {!$exclude{$_}} sort glob("tlx/string/"."*.hpp");
]]]*/
#include "string/base64.hpp"
#include "string/bitdump.hpp"
#include "string/compare_icase.hpp"
#include "string/contains.hpp"
#include "string/contains_word.hpp"
#include "string/ends_with.hpp"
#include "string/equal_icase.hpp"
#include "string/erase_all.hpp"
#include "string/escape_html.hpp"
#include "string/escape_uri.hpp"
#include "string/expand_environment_variables.hpp"
#include "string/extract_between.hpp"
#include "string/format_iec_units.hpp"
#include "string/format_si_iec_units.hpp"
#include "string/format_si_units.hpp"
#include "string/hash_djb2.hpp"
#include "string/hash_sdbm.hpp"
#include "string/hexdump.hpp"
#include "string/index_of.hpp"
#include "string/join.hpp"
#include "string/join_generic.hpp"
#include "string/join_quoted.hpp"
#include "string/less_icase.hpp"
#include "string/levenshtein.hpp"
#include "string/pad.hpp"
#include "string/parse_si_iec_units.hpp"
#include "string/parse_uri.hpp"
#include "string/parse_uri_form_data.hpp"
#include "string/replace.hpp"
#include "string/split.hpp"
#include "string/split_quoted.hpp"
#include "string/split_view.hpp"
#include "string/split_words.hpp"
#include "string/ssprintf.hpp"
#include "string/starts_with.hpp"
#include "string/to_lower.hpp"
#include "string/to_upper.hpp"
#include "string/trim.hpp"
#include "string/union_words.hpp"
#include "string/word_wrap.hpp"
// [[[end]]]

#endif // !TLX_STRING_HEADER

/******************************************************************************/
