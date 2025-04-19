#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE format_test
#include <boost/test/unit_test.hpp>
#include "../include/format.h"
#include "linux_parser.h"

BOOST_AUTO_TEST_SUITE(FormatTests)

BOOST_AUTO_TEST_CASE(test_elapsed_time_zero)
{
    BOOST_CHECK_EQUAL(Format::ElapsedTime(0), "00:00:00");
}

BOOST_AUTO_TEST_CASE(test_linux_parser_uid)
{
    BOOST_CHECK_EQUAL(LinuxParser::User(3544), "0");
}

BOOST_AUTO_TEST_SUITE_END()