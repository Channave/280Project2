#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------ The
// implementation of rotate_left is provided for you. REQUIRES: img points to a
// valid Image MODIFIES: *img EFFECTS:  The image is rotated 90 degrees to the
// left (counterclockwise).
void rotate_left(Image *img)
{

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r)
  {
    for (int c = 0; c < width; ++c)
    {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------ The
// implementation of rotate_right is provided for you. REQUIRES: img points to a
// valid Image. MODIFIES: *img EFFECTS:  The image is rotated 90 degrees to the
// right (clockwise).
void rotate_right(Image *img)
{

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r)
  {
    for (int c = 0; c < width; ++c)
    {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------ The
// implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2)
{
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows later on in the algorithm.
  return (dr * dr + dg * dg + db * db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// ------------------------------------------------------------------
// You may change code below this line!

// REQUIRES: img points to a valid Image. energy points to a Matrix. MODIFIES:
//           *energy EFFECTS:  energy serves as an "output parameter". The
//           Matrix pointed to by energy is initialized to be the same size as
//           the given Image, and then the energy matrix for that image is
//           computed and written into it. See the project spec for details on
//           computing the energy matrix.
void compute_energy_matrix(const Image *img, Matrix *energy)
{
  Matrix_init(energy, Image_width(img), Image_height(img));
  Matrix_fill(energy, 0);

  int max_val = 0;

  for (int r = 1; r < Image_height(img) - 1; r++)
  {
    for (int c = 1; c < Image_width(img) - 1; c++)
    {
      Pixel N = Image_get_pixel(img, r - 1, c);
      Pixel S = Image_get_pixel(img, r + 1, c);
      Pixel W = Image_get_pixel(img, r, c - 1);
      Pixel E = Image_get_pixel(img, r, c + 1);

      int val = squared_difference(N, S) + squared_difference(W, E);
      *Matrix_at(energy, r, c) = val;

      if (val > max_val)
      {
        max_val = val;
      }
    }
  }

  for (int r = 0; r < Image_height(img); r++)
  {
    *Matrix_at(energy, r, 0) = max_val;
    *Matrix_at(energy, r, Image_width(img) - 1) = max_val;
  }
  for (int c = 0; c < Image_width(img); c++)
  {
    *Matrix_at(energy, 0, c) = max_val;
    *Matrix_at(energy, Image_height(img) - 1, c) = max_val;
  }
}

// REQUIRES: energy points to a valid Matrix. * energy = mat cost points to a
//           Matrix. * cost = mat1 energy and cost aren't pointing to the same
//           Matrix mat =/ mat1 MODIFIES: *cost EFFECTS:  cost serves as an
//           "output parameter". modifying cost matrix The Matrix pointed to by
//           cost is initialized to be the same * cost mat...
//           cost->data.set_size(get_size of energy) size as the given energy
//           Matrix, and then the cost matrix is --- find cost and write into it
//           computed and written into it. See the project spec for details on
//           computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix *energy, Matrix *cost)
{
  int width = Matrix_width(energy);
  int height = Matrix_height(energy);

  Matrix_init(cost, width, height);

  for (int col = 0; col < width; ++col)
  {
    *Matrix_at(cost, 0, col) = *Matrix_at(energy, 0, col);
  }

  int total = width * height;
  for (int i = width; i < total; ++i)
  {
    int row = i / width;
    int col = i % width;

    int starter = col - 1;
    if (starter < 0)
    {
      starter = 0;
    }

    int end = col + 2;
    if (end > width)
    {
      end = width;
    }

    int low = Matrix_min_value_in_row(cost, row - 1, starter, end);
    *Matrix_at(cost, row, col) = *Matrix_at(energy, row, col) + low;
  }
}

// REQUIRES: cost points to a valid Matrix EFFECTS:  Returns the vertical seam
// with the minimal cost according to the given cost matrix, represented as a
// vector filled with the column numbers for each pixel along the seam, with
// index 0 representing the lowest numbered row (top of image). The length of
// the returned vector is equal to Matrix_height(cost). While determining the
// seam, if any pixels tie for lowest cost, the leftmost one (i.e. with the
// lowest column number) is used. See the project spec for details on computing
// the minimal seam. Note: When implementing the algorithm, compute the seam
// starting at the bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix *cost)
{
  int width = Matrix_width(cost);
  int height = Matrix_height(cost);
  vector<int> seam(height);

  int lr = height - 1;
  int low = 0;
  int choice = *Matrix_at(cost, lr, 0);
  for (int col = 1; col < width; col = col + 1)
  {
    int v = *Matrix_at(cost, lr, col);
    if (v < choice)
    {
      choice = v;
      low = col;
    }
  }
  seam[lr] = low;

  for (int row = height - 2; row >= 0; row = row - 1)
  {
    int curr = seam[row + 1];

    int lf = curr - 1;
    int middle = curr;
    int rc = curr + 1;

    bool has_left = (lf >= 0);
    bool has_right = (rc < width);

    int currcol;
    int value_after;

    if (has_left)
    {
      currcol = lf;
      value_after = *Matrix_at(cost, row, lf);
    }
    else
    {
      currcol = middle;
      value_after = *Matrix_at(cost, row, middle);
    }

    int middle_value = *Matrix_at(cost, row, middle);
    if (middle_value < value_after)
    {
      value_after = middle_value;
      currcol = middle;
    }

    if (has_right)
    {
      int rv = *Matrix_at(cost, row, rc);
      if (rv < value_after)
      {
        value_after = rv;
        currcol = rc;
      }
    }

    seam[row] = currcol;
  }

  return seam;
}

// REQUIRES: img points to a valid Image with width >= 2 seam.size() ==
//           Image_height(img) each element x in seam satisfies 0 <= x <
//           Image_width(img) MODIFIES: *img EFFECTS:  Removes the given
//           vertical seam from the Image. That is, one pixel will be removed
//           from every row in the image. The pixel removed from row r will be
//           the one with column equal to seam[r]. The width of the image will
//           be one less than before. See the project spec for details on
//           removing a vertical seam. NOTE:     Declare a new variable to hold
//           the smaller Image, and then do an assignment at the end to copy it
//           back into the original image.
void remove_vertical_seam(Image *img, const vector<int> &seam)
{
  int old_w = Image_width(img);
  int old_h = Image_height(img);
  int new_w = old_w - 1;

  Image small;
  Image_init(&small, new_w, old_h);

  int total = new_w * old_h;
  for (int i = 0; i < total; i = i + 1)
  {
    int r = i / new_w;
    int c = i % new_w;

    int remove = seam[r];
    int orig = c;
    if (c >= remove)
    {
      orig = c + 1;
    }

    Pixel color = Image_get_pixel(img, r, orig);
    Image_set_pixel(&small, r, c, color);
  }

  *img = small;
}

// REQUIRES: img points to a valid Image 0 < newWidth && newWidth <=
//           Image_width(img) MODIFIES: *img EFFECTS:  Reduces the width of the
//           given Image to be newWidth by using the seam carving algorithm. See
//           the spec for details. NOTE:     Use a vector to hold the seam, and
//           make sure that it has the right size. You can use .data() on a
//           vector to get the underlying array.
void seam_carve_width(Image *img, int newWidth)
{
  int w = Image_width(img);

  while (w > newWidth)
  {
    Matrix energy;
    Matrix cost;

    compute_energy_matrix(img, &energy);
    compute_vertical_cost_matrix(&energy, &cost);

    vector<int> seam = find_minimal_vertical_seam(&cost);
    remove_vertical_seam(img, seam);

    w = Image_width(img);
  }
}

// REQUIRES: img points to a valid Image 0 < newHeight && newHeight <=
//           Image_height(img) MODIFIES: *img EFFECTS:  Reduces the height of
//           the given Image to be newHeight. NOTE:     This is equivalent to
//           first rotating the Image 90 degrees left, then applying
//           seam_carve_width(img, newHeight), then rotating 90 degrees right.
void seam_carve_height(Image *img, int newHeight)
{
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image 0 < newWidth && newWidth <=
//           Image_width(img) 0 < newHeight && newHeight <= Image_height(img)
//           MODIFIES: *img EFFECTS:  Reduces the width and height of the given
//           Image to be newWidth and newHeight, respectively. NOTE:     This is
//           equivalent to applying seam_carve_width(img, newWidth) and then
//           applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight)
{
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}
