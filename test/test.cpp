#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE format_test
#include <boost/test/unit_test.hpp>
#include "linux_parser.h"
#include "system.h"

BOOST_AUTO_TEST_SUITE(FormatTests)

BOOST_AUTO_TEST_CASE(test_elapsed_time_zero)
{
    System system;
    system.Processes();
}

BOOST_AUTO_TEST_SUITE_END()