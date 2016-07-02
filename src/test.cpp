#define BOOST_TEST_MODULE boost_test_macro_overview
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <utility>

BOOST_AUTO_TEST_CASE( test )
{
  boost::test_tools::output_test_stream output;
  int i=2;
  output << "i=" << i;

  namespace tt = boost::test_tools;
  int a = 1;
  int b = 2;
  BOOST_TEST(a != b - 1);
  BOOST_TEST(a + 1 < b);
  BOOST_TEST(b -1 > a, a << " < " << b - 1 << " does not hold");
  BOOST_TEST(a == b, tt::bitwise());
  BOOST_TEST(a + 0.1 == b - 0.8, tt::tolerance(0.01));
}


typedef std::pair<int,float> pair_type;

BOOST_TEST_DONT_PRINT_LOG_VALUE( pair_type )

BOOST_AUTO_TEST_CASE( test_list )
{
  pair_type p1( 2, 5.5f );
  pair_type p2( 2, 5.501f );

  BOOST_CHECK_EQUAL( p1, p2 );
}


BOOST_AUTO_TEST_CASE( test_init )
{
  int current_time = 0; // real call is required here

  BOOST_TEST_MESSAGE( "Testing initialization :" );
  BOOST_TEST_MESSAGE( "Current time:" << current_time );
}

void foo( int ) {}

BOOST_AUTO_TEST_CASE( test_case )
{
  int* p = 0;

  BOOST_TEST_PASSPOINT();
  ++p;

  BOOST_TEST_PASSPOINT();
  ++p;

  BOOST_TEST_PASSPOINT();
  foo( *p );
}
