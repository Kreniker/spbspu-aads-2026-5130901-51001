#define BOOST_TEST_MODULE S1
#define BOOST_TEST_NO_LIB
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include "utils.hpp"

BOOST_AUTO_TEST_CASE(default_constructed_list_is_empty)
{
  kitserov::List< int > lst;
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
  BOOST_CHECK(lst.cbegin() == lst.cend());
}

BOOST_AUTO_TEST_CASE(add_element_front)
{
  kitserov::List< int > lst;
  int val = 42;
  kitserov::LIter< int > it = lst.add(val);
  BOOST_TEST(lst.get_size() == 1);
  BOOST_TEST(*lst.begin() == 42);
  BOOST_TEST(lst.front() == 42);
  BOOST_TEST(lst.back() == 42);
  BOOST_TEST(*it == 42);
}

BOOST_AUTO_TEST_CASE(insert_tail_element)
{
  kitserov::List< int > lst;
  int val1 = 1;
  int val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  BOOST_TEST(lst.get_size() == 2);
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(lst.back() == 2);
}

BOOST_AUTO_TEST_CASE(insert_at_position)
{
  kitserov::List< int > lst;
  int val1 = 10;
  int val2 = 20;
  int val3 = 30;
  lst.add(val1);
  kitserov::LIter< int > pos = lst.begin();
  lst.insert(val2, pos);
  lst.insert(val3, ++pos);
  BOOST_TEST(lst.get_size() == 3);
  kitserov::LIter< int > it = lst.begin();
  BOOST_TEST(*it == 10);
  ++it;
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(*it == 30);
}

BOOST_AUTO_TEST_CASE(access_by_index)
{
  kitserov::List< int > lst;
  int vals[] = {5, 6, 7};
  for (int& v : vals) {
    lst.insert_tail(v);
  }
  BOOST_TEST(*lst[0] == 5);
  BOOST_TEST(*lst[1] == 6);
  BOOST_TEST(*lst[2] == 7);
  const kitserov::List<int>& clst = lst;
  BOOST_TEST(*clst[0] == 5);
}

BOOST_AUTO_TEST_CASE(index_out_of_range_throws)
{
  kitserov::List< int > lst;
  int val = 1;
  lst.add(val);
  BOOST_CHECK_THROW(lst[1], std::out_of_range);
  const kitserov::List< int >& clst = lst;
  BOOST_CHECK_THROW(clst[1], std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  kitserov::List< int > lst;
  int val1 = 1;
  int val2 = 2;
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
  kitserov::List< int > lst;
  int val1 = 1;
  int val2 = 2;
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
  kitserov::List< int > lst;
  int val1 = 1;
  int val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  lst.clear();
  BOOST_TEST(lst.get_size() == 0);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  kitserov::List< int > lst1;
  int val = 100;
  lst1.add(val);
  kitserov::List< int > lst2 = std::move(lst1);
  BOOST_TEST(lst2.get_size() == 1);
  BOOST_TEST(lst2.front() == 100);
  BOOST_TEST(lst1.get_size() == 0);
  BOOST_CHECK(lst1.begin() == lst1.end());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  kitserov::List< int > lst1;
  kitserov::List< int > lst2;
  int val1 = 10;
  int val2 = 20;
  lst1.add(val1);
  lst2.add(val2);
  lst2 = std::move(lst1);
  BOOST_TEST(lst2.get_size() == 1);
  BOOST_TEST(lst2.front() == 10);
  BOOST_TEST(lst1.get_size() == 0);
}

BOOST_AUTO_TEST_CASE(iterator_modification)
{
  kitserov::List< int > lst;
  int vals[] = {1, 2, 3};
  for (int& v : vals) {
    lst.insert_tail(v);
  }
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    *it += 10;
  }
  kitserov::LIter< int > it = lst.begin();
  BOOST_TEST(*it == 11);
  ++it;
  BOOST_TEST(*it == 12);
  ++it;
  BOOST_TEST(*it == 13);
}

BOOST_AUTO_TEST_CASE(const_iterators)
{
  kitserov::List< int > lst;
  int val = 42;
  lst.add(val);
  const kitserov::List< int >& clst = lst;
  kitserov::LCIter< int > cit = clst.cbegin();
  BOOST_TEST(*cit == 42);
  auto cit2 = clst.begin();
  BOOST_TEST(*cit2 == 42);
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  kitserov::List< int > lst;
  int val1 = 1;
  int val2 = 2;
  lst.add(val1);
  lst.insert_tail(val2);
  kitserov::LIter< int > it1 = lst.begin();
  kitserov::LIter< int > it2 = lst.begin();
  BOOST_CHECK(it1 == it2);
  ++it2;
  BOOST_CHECK(it1 != it2);
  kitserov::LIter< int > it3 = lst.end();
  BOOST_CHECK(it3 != it1);
}

BOOST_AUTO_TEST_CASE(clear_range)
{
  kitserov::List< int > lst;
  int vals[] = {1, 2, 3, 4, 5};
  for (int& v : vals) {
    lst.insert_tail(v);
  }
  kitserov::LIter< int > from = lst.begin();
  ++from;
  kitserov::LIter< int > last = lst[4];
  lst.clear(from, last);
  BOOST_TEST(lst.get_size() == 2);
  kitserov::LIter< int > it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 5);
}
