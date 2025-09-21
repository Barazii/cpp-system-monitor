#include <boost/test/unit_test.hpp>
#include "linux_parser.h"

BOOST_AUTO_TEST_SUITE(LinuxParserSuite)

BOOST_AUTO_TEST_CASE(cpu_utilization_vector_has_values)
{
    auto v = LinuxParser::CpuUtilization();
    // Must include at least 8 slots which are specifically accessed for calculating active and idle jiffies. 
    BOOST_CHECK_GE(v.size(), 8);
}

BOOST_AUTO_TEST_CASE(memory_utilization_range)
{
    float m = LinuxParser::MemoryUtilization();
    // On a running system, expect 0 <= m <= 1
    BOOST_CHECK_GE(m, 0.0f);
    BOOST_CHECK_LE(m, 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()