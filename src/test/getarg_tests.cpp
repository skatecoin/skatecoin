#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-SK8");
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", false));
    BOOST_CHECK(GetBoolArg("-SK8", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-SK8o"));
    BOOST_CHECK(!GetBoolArg("-SK8o", false));
    BOOST_CHECK(GetBoolArg("-SK8o", true));

    ResetArgs("-SK8=0");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", false));
    BOOST_CHECK(!GetBoolArg("-SK8", true));

    ResetArgs("-SK8=1");
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", false));
    BOOST_CHECK(GetBoolArg("-SK8", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noSK8");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", false));
    BOOST_CHECK(!GetBoolArg("-SK8", true));

    ResetArgs("-noSK8=1");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", false));
    BOOST_CHECK(!GetBoolArg("-SK8", true));

    ResetArgs("-SK8 -noSK8");  // -SK8 should win
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", false));
    BOOST_CHECK(GetBoolArg("-SK8", true));

    ResetArgs("-SK8=1 -noSK8=1");  // -SK8 should win
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", false));
    BOOST_CHECK(GetBoolArg("-SK8", true));

    ResetArgs("-SK8=0 -noSK8=0");  // -SK8 should win
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", false));
    BOOST_CHECK(!GetBoolArg("-SK8", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--SK8=1");
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", false));
    BOOST_CHECK(GetBoolArg("-SK8", true));

    ResetArgs("--noSK8=1");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", false));
    BOOST_CHECK(!GetBoolArg("-SK8", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-SK8", "eleven"), "eleven");

    ResetArgs("-SK8 -bar");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-SK8", "eleven"), "");

    ResetArgs("-SK8=");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-SK8", "eleven"), "");

    ResetArgs("-SK8=11");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-SK8", "eleven"), "11");

    ResetArgs("-SK8=eleven");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-SK8", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-SK8", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-SK8", 0), 0);

    ResetArgs("-SK8 -bar");
    BOOST_CHECK_EQUAL(GetArg("-SK8", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-SK8=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-SK8", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-SK8=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-SK8", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--SK8");
    BOOST_CHECK_EQUAL(GetBoolArg("-SK8"), true);

    ResetArgs("--SK8=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-SK8", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noSK8");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", true));
    BOOST_CHECK(!GetBoolArg("-SK8", false));

    ResetArgs("-noSK8=1");
    BOOST_CHECK(!GetBoolArg("-SK8"));
    BOOST_CHECK(!GetBoolArg("-SK8", true));
    BOOST_CHECK(!GetBoolArg("-SK8", false));

    ResetArgs("-noSK8=0");
    BOOST_CHECK(GetBoolArg("-SK8"));
    BOOST_CHECK(GetBoolArg("-SK8", true));
    BOOST_CHECK(GetBoolArg("-SK8", false));

    ResetArgs("-SK8 --noSK8");
    BOOST_CHECK(GetBoolArg("-SK8"));

    ResetArgs("-noSK8 -SK8"); // SK8 always wins:
    BOOST_CHECK(GetBoolArg("-SK8"));
}

BOOST_AUTO_TEST_SUITE_END()
