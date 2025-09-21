#include <boost/test/unit_test.hpp>
#include "processor.h"

BOOST_AUTO_TEST_SUITE(ProcessorSuite)

BOOST_AUTO_TEST_CASE(utilization_in_range)
{
    Processor cpu;
    // CPU utilization is computed from deltas between two snapshots. Thats why 2 calls.
    (void)cpu.Utilization();
    float u = cpu.Utilization();
    BOOST_CHECK(u >= 0.0f);
    BOOST_CHECK(u <= 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()