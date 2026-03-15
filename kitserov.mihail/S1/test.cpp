#define BOOST_TEST_MODULE S1
#define BOOST_TEST_NO_MAIN
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include "utils.hpp"

BOOST_AUTO_TEST_SUITE(S1)

BOOST_AUTO_TEST_CASE(default_constructed_list_is_empty)
{
  kitserov::List< size_t > lst;
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
  BOOST_CHECK(lst.cbegin() == lst.cend());
}

BOOST_AUTO_TEST_CASE(add_element_front)
{
  kitserov::List< size_t > lst;
  size_t val = 42;
  kitserov::LIter< size_t > it = lst.add(val);
  BOOST_TEST(lst.get_size() == 1);
  BOOST_TEST(*lst.begin() == 42);
  BOOST_TEST(lst.front() == 42);
  BOOST_TEST(lst.back() == 42);
  BOOST_TEST(*it == 42);
}

BOOST_AUTO_TEST_CASE(insert_tail_element)
{
  kitserov::List< size_t > lst;
  size_t val1 = 1;
  size_t val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  BOOST_TEST(lst.get_size() == 2);
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(lst.back() == 2);
}

BOOST_AUTO_TEST_CASE(insert_at_position)
{
  kitserov::List< size_t > lst;
  size_t val1 = 10;
  size_t val2 = 20;
  size_t val3 = 30;
  lst.add(val1);
  kitserov::LIter< size_t > pos = lst.begin();
  lst.insert(val2, pos);
  lst.insert(val3, ++pos);
  BOOST_TEST(lst.get_size() == 3);
  kitserov::LIter< size_t > it = lst.begin();
  BOOST_TEST(*it == 10);
  ++it;
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 30);
}

BOOST_AUTO_TEST_CASE(access_by_index)
{
  kitserov::List< size_t > lst;
  size_t vals[] = {5, 6, 7};
  for (size_t& v : vals) {
    lst.insert_tail(v);
  }
  BOOST_TEST(*lst[0] == 5);
  BOOST_TEST(*lst[1] == 6);
  BOOST_TEST(*lst[2] == 7);
  const kitserov::List< size_t >& clst = lst;
  BOOST_TEST(*clst[0] == 5);
}

BOOST_AUTO_TEST_CASE(index_out_of_range_throws)
{
  kitserov::List< size_t > lst;
  size_t val = 1;
  lst.add(val);
  BOOST_CHECK_THROW(lst[1], std::out_of_range);
  const kitserov::List< size_t >& clst = lst;
  BOOST_CHECK_THROW(clst[1], std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  kitserov::List< size_t > lst;
  size_t val1 = 1;
  size_t val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  lst.pop_front();
  BOOST_TEST(lst.get_size() == 1);
  BOOST_TEST(lst.front() == 2);
  lst.pop_front();
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK_THROW(lst.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  kitserov::List< size_t > lst;
  size_t val1 = 1;
  size_t val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  lst.pop_back();
  BOOST_TEST(lst.get_size() == 1);
  BOOST_TEST(lst.back() == 1);
  lst.pop_back();
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK_THROW(lst.pop_back(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(clear_list)
{
  kitserov::List< size_t > lst;
  size_t val1 = 1;
  size_t val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  lst.clear();
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  kitserov::List< size_t > lst1;
  size_t val = 100;
  lst1.add(val);
  kitserov::List< size_t > lst2 = std::move(lst1);
  BOOST_TEST(lst2.get_size() == 1);
  BOOST_TEST(lst2.front() == 100);
  BOOST_TEST(lst1.get_size() == 0);
  BOOST_CHECK(lst1.begin() == lst1.end());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  kitserov::List< size_t > lst1;
  kitserov::List< size_t > lst2;
  size_t val1 = 10;
  size_t val2 = 20;
  lst1.add(val1);
  lst2.add(val2);
  lst2 = std::move(lst1);
  BOOST_TEST(lst2.get_size() == 1);
  BOOST_TEST(lst2.front() == 10);
  BOOST_TEST(lst1.get_size() == 0);
}

BOOST_AUTO_TEST_CASE(iterator_modification)
{
  kitserov::List< size_t > lst;
  size_t vals[] = {1, 2, 3};
  for (size_t& v : vals) {
    lst.insert_tail(v);
  }
  for (kitserov::LIter< size_t > it = lst.begin(); it != lst.end(); ++it) {
    *it += 10;
  }
  kitserov::LIter< size_t > it = lst.begin();
  BOOST_TEST(*it == 11);
  ++it;
  BOOST_TEST(*it == 12);
  ++it;
  BOOST_TEST(*it == 13);
}

BOOST_AUTO_TEST_CASE(const_iterators)
{
  kitserov::List< size_t > lst;
  size_t val = 42;
  lst.add(val);
  const kitserov::List< size_t >& clst = lst;
  kitserov::LCIter< size_t > cit = clst.cbegin();
  BOOST_TEST(*cit == 42);
  kitserov::LCIter< size_t > cit2 = clst.begin();
  BOOST_TEST(*cit2 == 42);
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  kitserov::List< size_t > lst;
  size_t val1 = 1;
  size_t val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  kitserov::LIter< size_t > it1 = lst.begin();
  kitserov::LIter< size_t > it2 = lst.begin();
  BOOST_CHECK(it1 == it2);
  ++it2;
  BOOST_CHECK(it1 != it2);
  kitserov::LIter< size_t > it3 = lst.end();
  BOOST_CHECK(it3 != it1);
}

BOOST_AUTO_TEST_CASE(clear_range)
{
  kitserov::List< size_t > lst;
  size_t vals[] = {1, 2, 3, 4, 5};
  for (size_t& v : vals) {
    lst.insert_tail(v);
  }
  kitserov::LIter< size_t > from = lst.begin();
  ++from;
  kitserov::LIter< size_t > last = lst[4];
  lst.clear(from, last);
  BOOST_TEST(lst.get_size() == 2);
  kitserov::LIter< size_t > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 5);
}
BOOST_AUTO_TEST_SUITE_END()
