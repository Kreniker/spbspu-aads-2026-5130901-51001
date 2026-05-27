#include <utility>
#include <boost/test/unit_test.hpp>
#include "bitree.hpp"

using namespace kitserov;
using Tree = BSTree< int, int >;

BOOST_AUTO_TEST_CASE(bitree_start_empty)
{
  Tree t;
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
  BOOST_CHECK_EQUAL(t.height(), 0);
  BOOST_CHECK(t.begin() == t.end());
}

BOOST_AUTO_TEST_CASE(bitree_push_single)
{
  Tree t;
  t.push(5, 50);
  BOOST_CHECK(!t.empty());
  BOOST_CHECK_EQUAL(t.size(), 1);
  BOOST_CHECK_EQUAL(t.height(), 1);
  BOOST_CHECK_EQUAL(t.get(5), 50);
}

BOOST_AUTO_TEST_CASE(bitree_push_multiple)
{
  Tree t;
  t.push(5, 50);
  t.push(3, 30);
  t.push(7, 70);
  t.push(1, 10);
  t.push(9, 90);

  BOOST_CHECK_EQUAL(t.height(), 3);
  BOOST_CHECK_EQUAL(t.size(), 5);
  BOOST_CHECK_EQUAL(t.get(5), 50);
  BOOST_CHECK_EQUAL(t.get(3), 30);
  BOOST_CHECK_EQUAL(t.get(7), 70);
  BOOST_CHECK_EQUAL(t.get(1), 10);
  BOOST_CHECK_EQUAL(t.get(9), 90);
}

BOOST_AUTO_TEST_CASE(bitree_update_value)
{
  Tree t;
  t.push(5, 50);
  BOOST_CHECK_EQUAL(t.get(5), 50);
  t.push(5, 100);
  BOOST_CHECK_EQUAL(t.get(5), 100);
  BOOST_CHECK_EQUAL(t.size(), 1);
}

BOOST_AUTO_TEST_CASE(bitree_contains)
{
  Tree t;
  t.push(5, 50);
  t.push(3, 30);

  BOOST_CHECK(t.contains(5));
  BOOST_CHECK(t.contains(3));
  BOOST_CHECK(!t.contains(7));
  BOOST_CHECK(!t.contains(1));
}

BOOST_AUTO_TEST_CASE(bitree_get_missing_key)
{
  Tree t;
  t.push(5, 50);
  BOOST_CHECK_THROW(t.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(bitree_find)
{
  Tree t;
  t.push(5, 50);
  t.push(3, 30);
  t.push(7, 70);

  auto it = t.find(3);
  BOOST_CHECK(it != t.end());
  BOOST_CHECK_EQUAL((*it).second, 30);

  auto it_missing = t.find(99);
  BOOST_CHECK(it_missing == t.end());
}

BOOST_AUTO_TEST_CASE(bitree_iterator_traversal)
{
  Tree t;
  t.push(5, 50);
  t.push(3, 30);
  t.push(7, 70);
  t.push(1, 10);
  t.push(9, 90);

  std::vector< int > keys;
  for (auto it = t.begin(); it != t.end(); ++it) {
    keys.push_back(it->first);
  }

  BOOST_CHECK_EQUAL(keys.size(), 5);
  std::vector< int > expected = {1, 3, 5, 7, 9};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(bitree_drop)
{
  Tree t;
  t.push(5, 50);
  t.push(3, 30);
  t.push(7, 70);

  int removed = t.drop(3);
  BOOST_CHECK_EQUAL(removed, 30);
  BOOST_CHECK_EQUAL(t.size(), 2);
  BOOST_CHECK(!t.contains(3));
  BOOST_CHECK(t.contains(5));
  BOOST_CHECK(t.contains(7));
}


BOOST_AUTO_TEST_CASE(bitree_drop_missing_key)
{
  Tree t;
  t.push(5, 50);
  BOOST_CHECK_THROW(t.drop(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(bitree_copy_constructor)
{
  Tree t1;
  t1.push(5, 50);
  t1.push(3, 30);
  t1.push(7, 70);

  Tree t2(t1);
  BOOST_CHECK_EQUAL(t2.size(), 3);
  BOOST_CHECK_EQUAL(t2.get(5), 50);
  BOOST_CHECK_EQUAL(t2.get(3), 30);
  BOOST_CHECK_EQUAL(t2.get(7), 70);

  t1.push(10, 100);
  BOOST_CHECK_EQUAL(t2.size(), 3);
}

BOOST_AUTO_TEST_CASE(bitree_assignment)
{
  Tree t1, t2;
  t1.push(5, 50);
  t1.push(3, 30);

  t2 = t1;
  BOOST_CHECK_EQUAL(t2.size(), 2);
  BOOST_CHECK_EQUAL(t2.get(5), 50);
  BOOST_CHECK_EQUAL(t2.get(3), 30);
}
