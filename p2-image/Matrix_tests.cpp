#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <climits>

using namespace std;

// Here's a free test for you! Model yours after this one. Test functions have
// no interface and thus no RMEs, but add a comment like the one here to say
// what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks that Matrix_at returns that value
// for each element.
TEST(test_matrix_print)
{
  Matrix mat;
  Matrix_init(&mat, 1, 1);

  *Matrix_at(&mat, 0, 0) = 42;
  ostringstream expected;
  expected << "1 1\n"
           << "42 \n";
  ostringstream actual;
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}
TEST(test_fill_basic)
{
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for (int r = 0; r < height; ++r)
  {
    for (int c = 0; c < width; ++c)
    {
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}

// ADD YOUR TESTS HERE
TEST(test_init_size_basic)
{
  Matrix mat;
  Matrix_init(&mat, 3, 5);
  ASSERT_EQUAL(Matrix_width(&mat), 3);
  ASSERT_EQUAL(Matrix_height(&mat), 5);
  for (int r = 0; r < 5; ++r)
  {
    for (int c = 0; c < 3; ++c)
    {
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), 0);
    }
  }
}

TEST(test_set_get_edges)
{
  Matrix mat;
  Matrix_init(&mat, 3, 3);
  *Matrix_at(&mat, 0, 0) = -1;
  *Matrix_at(&mat, 0, 2) = 2;
  *Matrix_at(&mat, 2, 0) = 3;
  *Matrix_at(&mat, 2, 2) = -3;
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), -1);
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 2), 2);
  ASSERT_EQUAL(*Matrix_at(&mat, 2, 0), 3);
  ASSERT_EQUAL(*Matrix_at(&mat, 2, 2), -3);
}

TEST(test_matrix_print_2x3)
{
  Matrix mat;
  Matrix_init(&mat, 2, 3);
  *Matrix_at(&mat, 0, 0) = 1;
  *Matrix_at(&mat, 0, 1) = 2;
  *Matrix_at(&mat, 1, 0) = 3;
  *Matrix_at(&mat, 1, 1) = 4;
  *Matrix_at(&mat, 2, 0) = 5;
  *Matrix_at(&mat, 2, 1) = 6;
  ostringstream expected, actual;
  expected << "2 3\n"
           << "1 2 \n"
           << "3 4 \n"
           << "5 6 \n";
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}

TEST(test_fill_all_negative)
{
  Matrix mat;
  Matrix_init(&mat, 4, 2);
  Matrix_fill(&mat, -1);
  for (int r = 0; r < 2; ++r)
  {
    for (int c = 0; c < 4; ++c)
    {
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), -1);
    }
  }
}

TEST(test_fill_border_basic)
{
  Matrix mat;
  Matrix_init(&mat, 4, 3);
  Matrix_fill(&mat, 0);
  Matrix_fill_border(&mat, 9);
  for (int c = 0; c < 4; ++c)
  {
    ASSERT_EQUAL(*Matrix_at(&mat, 0, c), 9);
    ASSERT_EQUAL(*Matrix_at(&mat, 2, c), 9);
  }
  for (int r = 0; r < 3; ++r)
  {
    ASSERT_EQUAL(*Matrix_at(&mat, r, 0), 9);
    ASSERT_EQUAL(*Matrix_at(&mat, r, 3), 9);
  }
  ASSERT_EQUAL(*Matrix_at(&mat, 1, 1), 0);
  ASSERT_EQUAL(*Matrix_at(&mat, 1, 2), 0);
}

TEST(test_fill_border_1x1)
{
  Matrix mat;
  Matrix_init(&mat, 1, 1);
  Matrix_fill(&mat, -1);
  Matrix_fill_border(&mat, 5);
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), 5);
}

TEST(test_max_empty_0x0) {
    Matrix mat;
    Matrix_init(&mat, 0, 0);
    ASSERT_EQUAL(Matrix_max(&mat), 0);
}

TEST(test_max_empty_0xN) {
    Matrix mat;
    Matrix_init(&mat, 0, 5);
    ASSERT_EQUAL(Matrix_max(&mat), 0);
}

TEST(test_max_empty_Nx0) {
    Matrix mat;
    Matrix_init(&mat, 4, 0);
    ASSERT_EQUAL(Matrix_max(&mat), 0);
}


TEST(test_max_basic)
{
  Matrix mat;
  Matrix_init(&mat, 3, 3);
  Matrix_fill(&mat, -3);
  *Matrix_at(&mat, 0, 1) = -1;
  *Matrix_at(&mat, 2, 2) = 17;
  ASSERT_EQUAL(Matrix_max(&mat), 17);
}

TEST(test_max_single)
{
  Matrix mat; Matrix_init(&mat, 1, 1);
  *Matrix_at(&mat, 0, 0) = -7;
  ASSERT_EQUAL(Matrix_max(&mat), -7);
}

TEST(test_max_all_negative)
{
  Matrix mat; Matrix_init(&mat, 2, 3);
  int vals[] = {-8, -3, -9, -4, -7, -5}; // max = -3
  for (int i = 0; i < 6; ++i)
    *Matrix_at(&mat, i / Matrix_width(&mat), i % Matrix_width(&mat)) = vals[i];
  ASSERT_EQUAL(Matrix_max(&mat), -3);
}

TEST(test_matrix_max_all_int_min)
{
  Matrix m; Matrix_init(&m, 2, 2);
  *Matrix_at(&m, 0, 0) = INT_MIN;
  *Matrix_at(&m, 0, 1) = INT_MIN;
  *Matrix_at(&m, 1, 0) = INT_MIN;
  *Matrix_at(&m, 1, 1) = INT_MIN;
  ASSERT_EQUAL(Matrix_max(&m), INT_MIN);
}

TEST(test_max_contains_int_max)
{
  Matrix mat; Matrix_init(&mat, 3, 2);
  Matrix_fill(&mat, -1);
  *Matrix_at(&mat, 0, 0) = INT_MAX;
  ASSERT_EQUAL(Matrix_max(&mat), INT_MAX);
}

TEST(test_min_value_in_row_basic)
{
  Matrix mat;
  Matrix_init(&mat, 5, 2);
  int vals[5] = {3, -3, -1, 2, 8};
  for (int c = 0; c < 5; ++c)
    *Matrix_at(&mat, 0, c) = vals[c];
  ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 0, 0, 5), -3);
}

TEST(test_column_of_min_in_row_ties)
{
  Matrix mat;
  Matrix_init(&mat, 5, 1);
  int vals[5] = {4, -3, -3, 7, 9};
  for (int c = 0; c < 5; ++c)
    *Matrix_at(&mat, 0, c) = vals[c];
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 0, 5), 1);
}

TEST(test_min_and_column_range_start_end)
{
  Matrix mat;
  Matrix_init(&mat, 5, 1);
  int vals[5] = {9, -1, 8, 8, -3};
  for (int c = 0; c < 5; ++c)
    *Matrix_at(&mat, 0, c) = vals[c];
  ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 0, 0, 2), -1);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 0, 2), 1);
  ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 0, 4, 5), -3);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 4, 5), 4);
}

TEST(test_single_column_min_column_is_zero)
{
  Matrix mat;
  Matrix_init(&mat, 1, 4);
  *Matrix_at(&mat, 0, 0) = 4;
  *Matrix_at(&mat, 1, 0) = -3;
  *Matrix_at(&mat, 2, 0) = 9;
  *Matrix_at(&mat, 3, 0) = -1;
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 0, 1), 0);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 1, 0, 1), 0);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 2, 0, 1), 0);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 3, 0, 1), 0);
}

TEST(test_edges_read_write)
{
  Matrix mat;
  Matrix_init(&mat, 3, 3);
  *Matrix_at(&mat, 0, 0) = -1;
  *Matrix_at(&mat, 0, 2) = 1;
  *Matrix_at(&mat, 2, 0) = 2;
  *Matrix_at(&mat, 2, 2) = -3;
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), -1);
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 2), 1);
  ASSERT_EQUAL(*Matrix_at(&mat, 2, 0), 2);
  ASSERT_EQUAL(*Matrix_at(&mat, 2, 2), -3);
}
// You are encouraged to use any functions from Matrix_test_helpers.hpp as
// needed.

TEST_MAIN() // Do NOT put a semicolon here
