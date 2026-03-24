#define BOOST_TEST_MODULE QueueStackTests
#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "queue.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(empty_queue_throws_on_drop)
{
  kitserov::Queue< int > q;
  BOOST_CHECK(q.isEmpty());
  BOOST_CHECK_THROW(q.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(push_pop_single)
{
  kitserov::Queue< int > q;
  q.push(42);
  BOOST_CHECK(!q.isEmpty());
  BOOST_CHECK_EQUAL(q.drop(), 42);
  BOOST_CHECK(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(push_pop_fifo_order)
{
  kitserov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK_EQUAL(q.drop(), 1);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 3);
  BOOST_CHECK(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  kitserov::Queue< int > q1;
  q1.push(10);
  q1.push(20);
  kitserov::Queue< int > q2(q1);
  BOOST_CHECK_EQUAL(q2.drop(), 10);
  BOOST_CHECK_EQUAL(q2.drop(), 20);
  BOOST_CHECK(q2.isEmpty());
  BOOST_CHECK(!q1.isEmpty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  kitserov::Queue< int > q1;
  q1.push(5);
  q1.push(6);
  kitserov::Queue< int > q2;
  q2.push(7);
  q2 = q1;
  BOOST_CHECK_EQUAL(q2.drop(), 5);
  BOOST_CHECK_EQUAL(q2.drop(), 6);
  BOOST_CHECK(q2.isEmpty());
  BOOST_CHECK(!q1.isEmpty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  kitserov::Queue< int > q1;
  q1.push(100);
  q1.push(200);
  kitserov::Queue< int > q2(std::move(q1));
  BOOST_CHECK_EQUAL(q2.drop(), 100);
  BOOST_CHECK_EQUAL(q2.drop(), 200);
  BOOST_CHECK(q2.isEmpty());
  BOOST_CHECK(q1.isEmpty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  kitserov::Queue< int > q1;
  q1.push(100);
  q1.push(200);
  kitserov::Queue< int > q2;
  q2.push(300);
  q2 = std::move(q1);
  BOOST_CHECK_EQUAL(q2.drop(), 100);
  BOOST_CHECK_EQUAL(q2.drop(), 200);
  BOOST_CHECK(q2.isEmpty());
  BOOST_CHECK(q1.isEmpty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  kitserov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  kitserov::Queue< int > q2;
  q2.push(3);
  q1.swap(q2);
  BOOST_CHECK_EQUAL(q1.drop(), 3);
  BOOST_CHECK(q1.isEmpty());
  BOOST_CHECK_EQUAL(q2.drop(), 1);
  BOOST_CHECK_EQUAL(q2.drop(), 2);
  BOOST_CHECK(q2.isEmpty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  kitserov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.clear();
  BOOST_CHECK(q.isEmpty());
  BOOST_CHECK_THROW(q.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(empty_stack_throws_on_drop)
{
  kitserov::Stack< int > s;
  BOOST_CHECK(s.isEmpty());
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(push_pop_single)
{
  kitserov::Stack< int > s;
  s.push(42);
  BOOST_CHECK(!s.isEmpty());
  BOOST_CHECK_EQUAL(s.drop(), 42);
  BOOST_CHECK(s.isEmpty());
}

BOOST_AUTO_TEST_CASE(push_pop_lifo_order)
{
  kitserov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK_EQUAL(s.drop(), 3);
  BOOST_CHECK_EQUAL(s.drop(), 2);
  BOOST_CHECK_EQUAL(s.drop(), 1);
  BOOST_CHECK(s.isEmpty());
}

BOOST_AUTO_TEST_CASE(peek)
{
  kitserov::Stack< int > s;
  s.push(10);
  s.push(20);
  BOOST_CHECK_EQUAL(s.peek(), 20);
  BOOST_CHECK_EQUAL(s.peek(), 20);
  s.drop();
  BOOST_CHECK_EQUAL(s.peek(), 10);
}

BOOST_AUTO_TEST_CASE(peek_throws_when_empty)
{
  kitserov::Stack< int > s;
  BOOST_CHECK_THROW(s.peek(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  kitserov::Stack< int > s1;
  s1.push(100);
  s1.push(200);
  kitserov::Stack< int > s2(s1);
  BOOST_CHECK_EQUAL(s2.drop(), 200);
  BOOST_CHECK_EQUAL(s2.drop(), 100);
  BOOST_CHECK(s2.isEmpty());
  BOOST_CHECK(!s1.isEmpty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  kitserov::Stack< int > s1;
  s1.push(5);
  s1.push(6);
  kitserov::Stack< int > s2;
  s2.push(7);
  s2 = s1;
  BOOST_CHECK_EQUAL(s2.drop(), 6);
  BOOST_CHECK_EQUAL(s2.drop(), 5);
  BOOST_CHECK(s2.isEmpty());
  BOOST_CHECK(!s1.isEmpty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  kitserov::Stack< int > s1;
  s1.push(100);
  s1.push(200);
  kitserov::Stack< int > s2(std::move(s1));
  BOOST_CHECK_EQUAL(s2.drop(), 200);
  BOOST_CHECK_EQUAL(s2.drop(), 100);
  BOOST_CHECK(s2.isEmpty());
  BOOST_CHECK(s1.isEmpty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  kitserov::Stack< int > s1;
  s1.push(100);
  s1.push(200);
  kitserov::Stack< int > s2;
  s2.push(300);
  s2 = std::move(s1);
  BOOST_CHECK_EQUAL(s2.drop(), 200);
  BOOST_CHECK_EQUAL(s2.drop(), 100);
  BOOST_CHECK(s2.isEmpty());
  BOOST_CHECK(s1.isEmpty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  kitserov::Stack< int > s1;
  s1.push(1);
  s1.push(2);
  kitserov::Stack< int > s2;
  s2.push(3);
  s1.swap(s2);
  BOOST_CHECK_EQUAL(s1.drop(), 3);
  BOOST_CHECK(s1.isEmpty());
  BOOST_CHECK_EQUAL(s2.drop(), 2);
  BOOST_CHECK_EQUAL(s2.drop(), 1);
  BOOST_CHECK(s2.isEmpty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  kitserov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.clear();
  BOOST_CHECK(s.isEmpty());
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()