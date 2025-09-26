#include <cassert>
#include "Matrix.hpp"

// REQUIRES: mat points to a Matrix 0 < width && 0 < height MODIFIES: *mat
//           EFFECTS:  Initializes *mat as a Matrix with the given width and
//           height, with all elements initialized to 0.
void Matrix_init(Matrix *mat, int width, int height)
{
  mat->width = width;
  mat->height = height;
  mat->data.assign(width * height, 0);
}

// REQUIRES: mat points to a valid Matrix MODIFIES: os EFFECTS:  First, prints
// the width and height for the Matrix to os: WIDTH [space] HEIGHT [newline]
// Then prints the rows of the Matrix to os with one row per line. Each element
// is followed by a space and each row is followed by a newline. This means
// there will be an "extra" space at the end of each line.
void Matrix_print(const Matrix *mat, std::ostream &os)
{
  os << Matrix_width(mat) << " " << Matrix_height(mat) << '\n';
  for (int row = 0; row < mat->height; row++)
  {
    for (int col = 0; col < mat->width; col++)
    {
      os << *Matrix_at(mat, row, col) << " ";
    }
    os << '\n';
  }
}

// REQUIRES: mat points to a valid Matrix EFFECTS:  Returns the width of the
// Matrix.
int Matrix_width(const Matrix *mat)
{
  return mat->width;
}

// REQUIRES: mat points to a valid Matrix EFFECTS:  Returns the height of the
// Matrix.
int Matrix_height(const Matrix *mat)
{
  return mat->height;
}

// REQUIRES: mat points to a valid Matrix 0 <= row && row < Matrix_height(mat) 0
//           <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an element in the
//            Matrix.) EFFECTS:  Returns a pointer to the element in the Matrix
//            at the given row and column.
int *Matrix_at(Matrix *mat, int row, int column)
{
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  int index = row * mat->width + column;
  return &(mat->data[index]);
}

// REQUIRES: mat points to a valid Matrix 0 <= row && row < Matrix_height(mat) 0
//           <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in the Matrix at the
//           given row and column.
const int *Matrix_at(const Matrix *mat, int row, int column)
{
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  int index = row * Matrix_width(mat) + column;
  return &(mat->data[index]);
}

// REQUIRES: mat points to a valid Matrix MODIFIES: *mat EFFECTS:  Sets each
// element of the Matrix to the given value.
void Matrix_fill(Matrix *mat, int value)
{
  mat->data.assign(Matrix_width(mat) * Matrix_height(mat), value);
}

// REQUIRES: mat points to a valid Matrix MODIFIES: *mat EFFECTS:  Sets each
// element on the border of the Matrix to the given value. These are all
// elements in the first/last row or the first/last column.
void Matrix_fill_border(Matrix *mat, int value)
{
  for (int i = 0; i < Matrix_height(mat); i++)
  {
    *Matrix_at(mat, i, 0) = value;                     // left side vertical
    *Matrix_at(mat, i, Matrix_width(mat) - 1) = value; // right side vertical
  }
  for (int i = 0; i < Matrix_width(mat); i++)
  {
    *Matrix_at(mat, 0, i) = value;                      // top horizontal
    *Matrix_at(mat, Matrix_height(mat) - 1, i) = value; // bottom horizontal
  }
}

// REQUIRES: mat points to a valid Matrix EFFECTS:  Returns the value of the
// maximum element in the Matrix
int Matrix_max(const Matrix *mat)
{
  if (Matrix_height(mat) == 0 || Matrix_width(mat) == 0)
    {
        std::cout << "Error: Matrix is empty" << std::endl;
        return 0;  // or some sentinel value depending on your assignment rules
    }
  int max_value = *Matrix_at(mat, 0, 0);
  for (int i = 1; i < Matrix_height(mat) * Matrix_width(mat); i++)
  {
    int value = *Matrix_at(mat, i / Matrix_width(mat), i % Matrix_width(mat));
    if (value > max_value)
    {
      max_value = value;
    }
  }
  return max_value;
}

// REQUIRES: mat points to a valid Matrix 0 <= row && row < Matrix_height(mat) 0
//           <= column_start && column_end <= Matrix_width(mat) column_start <
//           column_end EFFECTS:  Returns the column of the element with the
//           minimal value in a particular region. The region is defined as
//           elements in the given row and between column_start (inclusive) and
//           column_end (exclusive). If multiple elements are minimal, returns
//           the column of the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix *mat, int row,
                                      int column_start, int column_end)
{
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_start < column_end);
  assert(column_end <= Matrix_width(mat));
  int index_start = row * Matrix_width(mat) + column_start;
  int index_end = row * Matrix_width(mat) + column_end;
  int min_value = *Matrix_at(mat, row, column_start);
  int min_index = index_start;
  for (int i = index_start; i < index_end; i++)
  {
    int val = *Matrix_at(mat, i / Matrix_width(mat), i % Matrix_width(mat));
    if (val < min_value)
    {
      min_value = val;
      min_index = i;
    }
  }
  return min_index % Matrix_width(mat);
}

// REQUIRES: mat points to a valid Matrix 0 <= row && row < Matrix_height(mat) 0
//           <= column_start && column_end <= Matrix_width(mat) column_start <
//           column_end EFFECTS:  Returns the minimal value in a particular
//           region. The region is defined as elements in the given row and
//           between column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix *mat, int row,
                            int column_start, int column_end)
{
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_start < column_end);
  assert(column_end <= Matrix_width(mat));

  int index_start = row * Matrix_width(mat) + column_start;
  int index_end = row * Matrix_width(mat) + column_end;

  int min_value = *Matrix_at(mat, row, column_start);

  for (int i = index_start + 1; i < index_end; i++)
  {
    int val = *Matrix_at(mat, i / Matrix_width(mat), i % Matrix_width(mat));
    if (val < min_value)
    {
      min_value = val;
    }
  }

  return min_value;
}
