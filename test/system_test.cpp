#include <boost/test/unit_test.hpp>
#include "system.h"

BOOST_AUTO_TEST_SUITE(SystemSuite)

BOOST_AUTO_TEST_CASE(basic_queries_do_not_crash)
{
    System sys;
    // These call into LinuxParser; we just ensure they run and return values
    BOOST_CHECK(sys.OperatingSystem().size() > 0);
    BOOST_CHECK(sys.Kernel().size() > 0);
    BOOST_CHECK(sys.UpTime() >= 0);
    BOOST_CHECK(sys.TotalProcesses() >= 0);
    BOOST_CHECK(sys.RunningProcesses() >= 0);
}

BOOST_AUTO_TEST_SUITE_END()