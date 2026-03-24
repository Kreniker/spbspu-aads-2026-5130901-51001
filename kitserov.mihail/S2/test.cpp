#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(Queue_push_and_drop_integers)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK_EQUAL(q.drop(), 3);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 1);
}

BOOST_AUTO_TEST_CASE(Queue_push_and_drop_strings)
{
  Queue< std::string > q;
  q.push("foo");
  q.push("bar");

  BOOST_CHECK_EQUAL(q.drop(), std::string("bar"));
  BOOST_CHECK_EQUAL(q.drop(), std::string("foo"));
}

BOOST_AUTO_TEST_CASE(Queue_drop_throws_when_empty)
{
  Queue< int > q;
  BOOST_CHECK_THROW(q.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(Stack_push_and_drop_integers)
{
  Stack< int > s;
  s.push(10);
  s.push(20);
  s.push(30);

  BOOST_CHECK_EQUAL(s.drop(), 10);
  BOOST_CHECK_EQUAL(s.drop(), 20);
  BOOST_CHECK_EQUAL(s.drop(), 30);
}

BOOST_AUTO_TEST_CASE(Stack_push_and_drop_strings)
{
  Stack< std::string > s;
  s.push("abc");
  s.push("xyz");

  BOOST_CHECK_EQUAL(s.drop(), std::string("abc"));
  BOOST_CHECK_EQUAL(s.drop(), std::string("xyz"));
}

BOOST_AUTO_TEST_CASE(Stack_drop_throws_when_empty)
{
  Stack< int > s;
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()