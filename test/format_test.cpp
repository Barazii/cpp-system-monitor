#include <boost/test/unit_test.hpp>
#include "format.h"

BOOST_AUTO_TEST_SUITE(FormatSuite)

BOOST_AUTO_TEST_CASE(elapsed_time_basic)
{
    // 1 day, 2 hours, 3 minutes, 4 seconds => 01:02:03:04
    long seconds = 1L*24*60*60 + 2L*60*60 + 3L*60 + 4L;
    BOOST_CHECK_EQUAL(Format::ElapsedTime(seconds), "01:02:03:04");
}

BOOST_AUTO_TEST_CASE(elapsed_time_zeros)
{
    BOOST_CHECK_EQUAL(Format::ElapsedTime(0L), "00:00:00:00");
}

BOOST_AUTO_TEST_SUITE_END()