#include <boost/test/unit_test.hpp>
#include "process.h"

BOOST_AUTO_TEST_SUITE(ProcessSuite)

BOOST_AUTO_TEST_CASE(getters_setters)
{
    Process p;
    p.setPid(123);
    p.setCommand("/bin/echo hello");
    p.setCpuUtilization(0.42f);
    p.setRam("2048");
    p.setUpTime(77);

    BOOST_CHECK_EQUAL(p.Pid(), 123);
    BOOST_CHECK_EQUAL(p.Command(), "/bin/echo hello");
    BOOST_CHECK_CLOSE(p.CpuUtilization(), 0.42f, 0.001f);
    BOOST_CHECK_EQUAL(p.Ram(), "2048");
    BOOST_CHECK_EQUAL(p.UpTime(), 77);
}

BOOST_AUTO_TEST_CASE(comparisons_by_cpu)
{
    Process a, b;
    a.setCpuUtilization(0.2f);
    b.setCpuUtilization(0.8f);

    BOOST_CHECK(a < b);
    BOOST_CHECK(b > a);
}

BOOST_AUTO_TEST_SUITE_END()