#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>

namespace korchevskii {
  const int maxFixedElements = 10000;
  const int fixedArrayMode = 1;
  const int dynamicArrayMode = 2;

  bool parseMode(const char* text, int& mode)
  {
    if ((text[0] == '1') && (text[1] == '\0')) {
      mode = fixedArrayMode;
      return true;
    }
    if ((text[0] == '2') && (text[1] == '\0')) {
      mode = dynamicArrayMode;
      return true;
    }
    return false;
  }

  bool pathsAreEqual(const char* first_path, const char* second_path)
  {
    while ((*first_path != '\0') && (*first_path == *second_path)) {
      ++first_path;
      ++second_path;
    }
    return *first_path == *second_path;
  }

  bool readDimensions(std::istream& input, std::size_t& rows, std::size_t& columns, std::size_t& element_count)
  {
    long long input_rows = 0;
    long long input_columns = 0;
    if (!(input >> input_rows >> input_columns)) {
      return false;
    }
    if ((input_rows < 0) || (input_columns < 0)) {
      return false;
    }
    if ((static_cast< unsigned long long >(input_rows) > std::numeric_limits< std::size_t >::max())
        || (static_cast< unsigned long long >(input_columns) > std::numeric_limits< std::size_t >::max())) {
      return false;
    }
    rows = static_cast< std::size_t >(input_rows);
    columns = static_cast< std::size_t >(input_columns);
    if ((rows == 0) != (columns == 0)) {
      return false;
    }
    if ((columns != 0) && (rows > (std::numeric_limits< std::size_t >::max() / columns))) {
      return false;
    }
    element_count = rows * columns;
    return true;
  }

  bool readElements(std::istream& input, std::size_t rows, std::size_t columns, int** matrix)
  {
    for (std::size_t row = 0; row < rows; ++row) {
      for (std::size_t column = 0; column < columns; ++column) {
        if (!(input >> matrix[row][column])) {
          return false;
        }
      }
    }
    input >> std::ws;
    return input.eof() && !input.bad();
  }

  std::size_t countLocalMax(const int* const* values, std::size_t rows, std::size_t columns)
  {
    std::size_t count = 0;
    if ((rows < 3) || (columns < 3)) {
      return count;
    }
    for (std::size_t row = 1; row < (rows - 1); ++row) {
      for (std::size_t column = 1; column < (columns - 1); ++column) {
        const int value = values[row][column];
        bool is_local_maximum = true;
        for (int row_offset = -1; row_offset <= 1; ++row_offset) {
          for (int column_offset = -1; column_offset <= 1; ++column_offset) {
            if ((row_offset == 0) && (column_offset == 0)) {
              continue;
            }
            const std::size_t neighborRow = static_cast< std::size_t >(static_cast< long long >(row) + row_offset);
            const std::size_t neighborColumn
                = static_cast< std::size_t >(static_cast< long long >(column) + column_offset);
            if (value <= values[neighborRow][neighborColumn]) {
              is_local_maximum = false;
            }
          }
        }
        if (is_local_maximum) {
          ++count;
        }
      }
    }
    return count;
  }

  long long minSumOnMainDiagonals(const int* const* values, std::size_t rows, std::size_t columns)
  {
    bool has_diagonal = false;
    long long min_sum = 0;
    for (std::size_t start_column = 0; start_column < columns; ++start_column) {
      long long sum = 0;
      std::size_t row = 0;
      std::size_t column = start_column;
      while (row < rows) {
        sum += values[row][column];
        ++row;
        if (column == 0) {
          break;
        }
        --column;
      }
      if ((!has_diagonal) || (sum < min_sum)) {
        min_sum = sum;
        has_diagonal = true;
      }
    }
    for (std::size_t start_row = 1; start_row < rows; ++start_row) {
      long long sum = 0;
      std::size_t row = start_row;
      std::size_t column = columns - 1;
      while (row < rows) {
        sum += values[row][column];
        ++row;
        if (column == 0) {
          break;
        }
        --column;
      }
      if ((!has_diagonal) || (sum < min_sum)) {
        min_sum = sum;
        has_diagonal = true;
      }
    }
    return has_diagonal ? min_sum : 0;
  }

  bool processMatrix(std::istream& input, std::ostream& output, std::size_t rows, std::size_t columns, int** values)
  {
    if (!readElements(input, rows, columns, values)) {
      std::cerr << "Error: the input file does not contain a valid matrix.\n";
      return false;
    }
    output << countLocalMax(values, rows, columns) << '\n';
    output << minSumOnMainDiagonals(values, rows, columns) << '\n';
    output.flush();
    if (!output.good()) {
      std::cerr << "Error: cannot write to the output file.\n";
      return false;
    }
    return true;
  }

  int** allocateMatrix(std::size_t rows, std::size_t columns, std::size_t element_count)
  {
    if (element_count == 0) {
      return nullptr;
    }
    const std::size_t maxSize = std::numeric_limits< std::size_t >::max();
    if ((rows > (maxSize / sizeof(int*))) || (element_count > (maxSize / sizeof(int)))) {
      return nullptr;
    }
    int* elements = nullptr;
    int** matrix = nullptr;
    try {
      elements = new int[element_count];
      matrix = new int*[rows];
    } catch (...) {
      delete[] matrix;
      delete[] elements;
      return nullptr;
    }
    for (std::size_t row = 0; row < rows; ++row) {
      matrix[row] = elements + (row * columns);
    }
    return matrix;
  }

  void deleteValues(int** matrix)
  {
    if (matrix != nullptr) {
      delete[] matrix[0];
      delete[] matrix;
    }
  }

  bool run(std::istream& input, std::ostream& output, int mode)
  {
    std::size_t rows = 0;
    std::size_t columns = 0;
    std::size_t element_count = 0;
    if (!readDimensions(input, rows, columns, element_count)) {
      std::cerr << "Error: the input file does not contain valid dimensions.\n";
      return false;
    }
    if ((mode == fixedArrayMode) && (element_count > static_cast< std::size_t >(maxFixedElements))) {
      std::cerr << "Error: a fixed array can contain at most 10000 elements.\n";
      return false;
    }
    if (mode == fixedArrayMode) {
      int elements[maxFixedElements] = {};
      int* matrix[maxFixedElements] = {};
      for (std::size_t row = 0; row < rows; ++row) {
        matrix[row] = elements + (row * columns);
      }
      return processMatrix(input, output, rows, columns, matrix);
    }
    int** const matrix = allocateMatrix(rows, columns, element_count);
    if ((element_count != 0) && (matrix == nullptr)) {
      std::cerr << "Error: not enough memory to store the matrix.\n";
      return false;
    }
    const bool result = processMatrix(input, output, rows, columns, matrix);
    deleteValues(matrix);
    return result;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Error: expected a mode, an input file, and an output file.\n";
    return 1;
  }
  const char* const mode_text = argv[1];
  const char* const input_path = argv[2];
  const char* const output_path = argv[3];
  if (korchevskii::pathsAreEqual(input_path, output_path)) {
    std::cerr << "Error: input and output files must have different paths.\n";
    return 1;
  }
  int mode = 0;
  if (!korchevskii::parseMode(mode_text, mode)) {
    std::cerr << "Error: mode must be 1 (fixed array) or 2 (dynamic array).\n";
    return 1;
  }
  std::ifstream input{input_path};
  if (!input) {
    std::cerr << "Error: cannot open the input file.\n";
    return 1;
  }
  std::ofstream output{output_path};
  if (!output) {
    std::cerr << "Error: cannot open the output file.\n";
    return 1;
  }
  return korchevskii::run(input, output, mode) ? 0 : 2;
}
