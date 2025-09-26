#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one. Test functions have
// no interface and thus no RMEs, but add a comment like the one here to say
// what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks that Image_print produces the
// correct output.
TEST(test_print_basic)
{
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE init sets correct size
TEST(test_init_size_basic)
{
  Image img;
  Image_init(&img, 3, 5);
  ASSERT_EQUAL(Image_width(&img), 3);
  ASSERT_EQUAL(Image_height(&img), 5);
}

// init sets all pixels to zero
TEST(test_init_pixels_zero)
{
  Image img;
  Image_init(&img, 2, 3);
  for (int r = 0; r < Image_height(&img); ++r)
  {
    for (int c = 0; c < Image_width(&img); ++c)
    {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, r, c), Pixel{0, 0, 0}));
    }
  }
}

// set and get pixels including edges
TEST(test_set_get_basic)
{
  Image img;
  Image_init(&img, 3, 3);
  Pixel a{5, 10, 15}, b{20, 25, 30}, c{35, 40, 45}, d{50, 55, 60};

  Image_set_pixel(&img, 0, 0, a);
  Image_set_pixel(&img, 0, 2, b);
  Image_set_pixel(&img, 2, 0, c);
  Image_set_pixel(&img, 2, 2, d);

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 0, 0), a));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 0, 2), b));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 2, 0), c));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 2, 2), d));
}

// fill sets all pixels to one color
TEST(test_fill_color_basic)
{
  Image img, exp;
  Image_init(&img, 4, 2);
  Image_init(&exp, 4, 2);
  Pixel color{12, 34, 56};
  Image_fill(&img, color);
  Image_fill(&exp, color);
  ASSERT_TRUE(Image_equal(&img, &exp));
}

// fill overwrites previous pixels
TEST(test_fill_overwrites_previous)
{
  Image img, exp;
  Image_init(&img, 2, 2);
  Image_init(&exp, 2, 2);
  Image_set_pixel(&img, 0, 0, Pixel{7, 8, 9});
  Image_set_pixel(&img, 1, 1, Pixel{11, 12, 13});
  Pixel fillc{99, 88, 77};
  Image_fill(&img, fillc);
  Image_fill(&exp, fillc);
  ASSERT_TRUE(Image_equal(&img, &exp));
}

// fill handles empty image (0x0)
TEST(test_fill_empty_image)
{
  Image img, exp;
  Image_init(&img, 0, 0);
  Image_init(&exp, 0, 0);
  Pixel color{1, 2, 3};
  Image_fill(&img, color);
  Image_fill(&exp, color);
  ASSERT_TRUE(Image_equal(&img, &exp));
}

// fill works with 1x1 image
TEST(test_fill_one_pixel)
{
  Image img, exp;
  Image_init(&img, 1, 1);
  Image_init(&exp, 1, 1);
  Pixel color{42, 24, 12};
  Image_fill(&img, color);
  Image_fill(&exp, color);
  ASSERT_TRUE(Image_equal(&img, &exp));
}

// fill multiple times with different colors
TEST(test_fill_multiple_times)
{
  Image img, exp;
  Image_init(&img, 2, 2);
  Image_init(&exp, 2, 2);

  Pixel first{10, 20, 30};
  Pixel second{99, 88, 77};

  Image_fill(&img, first);
  Image_fill(&exp, first);
  ASSERT_TRUE(Image_equal(&img, &exp));

  Image_fill(&img, second);
  Image_fill(&exp, second);
  ASSERT_TRUE(Image_equal(&img, &exp));
}

// print outputs correct ppm for 1x1
TEST(test_print_1x1)
{
  Image img;
  Image_init(&img, 1, 1);
  Image_set_pixel(&img, 0, 0, Pixel{42, 5, 17});

  ostringstream out, correct;
  Image_print(&img, out);
  correct << "P3\n1 1\n255\n";
  correct << "42 5 17 \n";
  ASSERT_EQUAL(out.str(), correct.str());
}

// init from ppm input with easy varied values
TEST(test_read_ppm_basic)
{
  istringstream in(
      "P3\n"
      "3 2\n"
      "255\n"
      "3 4 5   6 7 8   9 10 11\n"
      "12 13 14   15 16 17   18 19 20\n");
  Image img, exp;
  Image_init(&img, in);

  Image_init(&exp, 3, 2);
  Image_set_pixel(&exp, 0, 0, Pixel{3, 4, 5});
  Image_set_pixel(&exp, 0, 1, Pixel{6, 7, 8});
  Image_set_pixel(&exp, 0, 2, Pixel{9, 10, 11});
  Image_set_pixel(&exp, 1, 0, Pixel{12, 13, 14});
  Image_set_pixel(&exp, 1, 1, Pixel{15, 16, 17});
  Image_set_pixel(&exp, 1, 2, Pixel{18, 19, 20});

  ASSERT_TRUE(Image_equal(&img, &exp));
}

// print after read matches input
TEST(test_print_matches_after_read)
{
  istringstream in(
      "P3\n"
      "2 1\n"
      "255\n"
      "21 22 23   24 25 26\n");
  Image img;
  Image_init(&img, in);

  ostringstream out, correct;
  Image_print(&img, out);
  correct << "P3\n2 1\n255\n";
  correct << "21 22 23 24 25 26 \n";
  ASSERT_EQUAL(out.str(), correct.str());
}

// set and get at image edges
TEST(test_edges_read_write)
{
  Image img;
  Image_init(&img, 3, 3);

  Pixel tl{1, 2, 3}, tr{4, 5, 6}, bl{7, 8, 9}, br{10, 11, 12};
  Image_set_pixel(&img, 0, 0, tl);
  Image_set_pixel(&img, 0, 2, tr);
  Image_set_pixel(&img, 2, 0, bl);
  Image_set_pixel(&img, 2, 2, br);

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 0, 0), tl));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 0, 2), tr));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 2, 0), bl));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 2, 2), br));
}

// width and height accessors
TEST(test_width_height_basic)
{
  Image img;
  Image_init(&img, 4, 3);
  ASSERT_EQUAL(Image_width(&img), 4);
  ASSERT_EQUAL(Image_height(&img), 3);
}
// You are encouraged to use any functions from Image_test_helpers.hpp as
// needed.

TEST_MAIN() // Do NOT put a semicolon here
