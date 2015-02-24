/*
 * iterator.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace rt {
namespace parsing {

typedef boost::spirit::line_pos_iterator<boost::spirit::istream_iterator> iterator_t;

}
}
