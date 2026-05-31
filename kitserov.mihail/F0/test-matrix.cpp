#include <sstream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "matrix.hpp"

using namespace kitserov;

BOOST_AUTO_TEST_CASE(default_matrix_is_empty)
{
  Matrix< int > matrix;

  BOOST_CHECK_EQUAL(matrix.rows(), 0);
  BOOST_CHECK_EQUAL(matrix.cols(), 0);

  std::ostringstream out;
  out << matrix;
  BOOST_CHECK_EQUAL(out.str(), "");
}

BOOST_AUTO_TEST_CASE(matrix_fills_with_initial_value)
{
  Matrix< int > matrix(2, 3, 7);

  BOOST_CHECK_EQUAL(matrix.rows(), 2);
  BOOST_CHECK_EQUAL(matrix.cols(), 3);
  BOOST_CHECK_EQUAL(matrix(0, 0), 7);
  BOOST_CHECK_EQUAL(matrix(0, 1), 7);
  BOOST_CHECK_EQUAL(matrix(0, 2), 7);
  BOOST_CHECK_EQUAL(matrix(1, 0), 7);
  BOOST_CHECK_EQUAL(matrix(1, 1), 7);
  BOOST_CHECK_EQUAL(matrix(1, 2), 7);
}

BOOST_AUTO_TEST_CASE(matrix_allows_read_and_write_access)
{
  Matrix< int > matrix(3, 2, 0);

  matrix(0, 1) = 4;
  matrix(2, 0) = -8;

  const Matrix< int >& constMatrix = matrix;

  BOOST_CHECK_EQUAL(matrix(0, 1), 4);
  BOOST_CHECK_EQUAL(matrix(2, 0), -8);
  BOOST_CHECK_EQUAL(constMatrix(0, 1), 4);
  BOOST_CHECK_EQUAL(constMatrix(2, 0), -8);
}

BOOST_AUTO_TEST_CASE(matrix_stream_output)
{
  Matrix< int > matrix(2, 2, 1);
  matrix(1, 0) = 3;
  matrix(1, 1) = 5;

  std::ostringstream out;
  out << matrix;

  BOOST_CHECK_EQUAL(out.str(), "1 1\n3 5");
}

BOOST_AUTO_TEST_CASE(matrix_throws_on_out_of_range_access)
{
  Matrix< int > matrix(2, 2, 0);

  BOOST_CHECK_THROW(matrix(2, 0), std::out_of_range);
  BOOST_CHECK_THROW(matrix(0, 2), std::out_of_range);

  const Matrix< int >& constMatrix = matrix;
  BOOST_CHECK_THROW(constMatrix(3, 1), std::out_of_range);
}
