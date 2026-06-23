#include <utility>
#include <boost/test/unit_test.hpp>
#include "hash-cucushka.hpp"
#include "identity-hash.hpp"

using namespace kitserov;

BOOST_AUTO_TEST_CASE(hash_start_empty)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r;
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 8);
  BOOST_CHECK_EQUAL(r.capacity() % 2, 0);
}

BOOST_AUTO_TEST_CASE(check_initial_size_and_cap)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(5);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 6);
  BOOST_CHECK_EQUAL(r.capacity() % 2, 0);
}

BOOST_AUTO_TEST_CASE(copy)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > yar(4);
  r.copy(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 4);
}

BOOST_AUTO_TEST_CASE(swap)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > yar(4);
  r.swap(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 4);
  BOOST_CHECK_EQUAL(yar.capacity(), 6);
}

BOOST_AUTO_TEST_CASE(find)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  BOOST_CHECK_EQUAL(r.find(4), nullptr);
}

BOOST_AUTO_TEST_CASE(add)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.size(), 1);
  BOOST_CHECK_EQUAL(*(r.find(4)), 16);
}

BOOST_AUTO_TEST_CASE(hash_collision_uses_second_half)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(8);
  r.add(0, 0);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.size(), 2);
  BOOST_CHECK_EQUAL(*(r.find(0)), 0);
  BOOST_CHECK_EQUAL(*(r.find(4)), 16);
}

BOOST_AUTO_TEST_CASE(copy_equal)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > yar = r;
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 6);
  BOOST_CHECK_EQUAL(*(yar.find(4)), 16);
}

BOOST_AUTO_TEST_CASE(equal_with_rvalue)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > yar = std::move(r);
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 6);
  BOOST_CHECK_EQUAL(*(yar.find(4)), 16);
}

BOOST_AUTO_TEST_CASE(load_factor)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  BOOST_CHECK(r.loadFactor() == 1.0f / 6.0f);
}

BOOST_AUTO_TEST_CASE(square_staples)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r[4], 16);
}

BOOST_AUTO_TEST_CASE(hash_read)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(8);
  r.add(4, 16);
  r.add(4, 17);
  BOOST_CHECK_EQUAL(r[4], 17);
}

BOOST_AUTO_TEST_CASE(hash_erase)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(8);
  r.add(0, 0);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.size(), 2);
  r.erase(0);
  BOOST_CHECK_EQUAL(r.find(0), nullptr);
  BOOST_CHECK_EQUAL(*(r.find(4)), 16);
}

BOOST_AUTO_TEST_CASE(hash_rehash)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(0, 0);
  r.add(4, 16);
  r.add(1, 1);
  r.rehash(11);
  BOOST_CHECK_EQUAL(r.size(), 3);
  BOOST_CHECK_EQUAL(r.capacity(), 12);
  BOOST_CHECK_EQUAL(r.capacity() % 2, 0);
  BOOST_CHECK(r.loadFactor() == 3.0f / 12.0f);
  BOOST_CHECK(r.find(0));
  BOOST_CHECK(r.find(4));
  BOOST_CHECK(r.find(1));
}

BOOST_AUTO_TEST_CASE(hash_begin_and_end)
{
  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > r(6);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(*(r.begin()), 16);
  BOOST_CHECK(r.begin() == r.begin());
  BOOST_CHECK(r.begin() < r.end());

  HashCucushka< int, int, IdentityHash, IdentityHash, std::equal_to< int > > yar(6);
  BOOST_CHECK(yar.begin() == yar.end());

  yar.add(0, 0);
  yar.add(4, 16);
  yar.add(1, 1);
  auto it = yar.begin();
  BOOST_CHECK(*it == it.key() * it.key());
  ++it;
  BOOST_CHECK(*(it++));
  BOOST_CHECK(*it);
}
