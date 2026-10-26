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

  bool pathsAreEqual(const char* firstPath, const char* secondPath)
  {
    while ((*firstPath != '\0') && (*firstPath == *secondPath)) {
      ++firstPath;
      ++secondPath;
    }
    return *firstPath == *secondPath;
  }

  bool readDimensions(std::istream& input, std::size_t& rows, std::size_t& columns, std::size_t& elementCount)
  {
    long long inputRows = 0;
    long long inputColumns = 0;
    if (!(input >> inputRows >> inputColumns)) {
      return false;
    }
    if ((inputRows < 0) || (inputColumns < 0)) {
      return false;
    }
    if ((static_cast< unsigned long long >(inputRows) > std::numeric_limits< std::size_t >::max())
        || (static_cast< unsigned long long >(inputColumns) > std::numeric_limits< std::size_t >::max())) {
      return false;
    }
    rows = static_cast< std::size_t >(inputRows);
    columns = static_cast< std::size_t >(inputColumns);
    if ((rows == 0) != (columns == 0)) {
      return false;
    }
    if ((columns != 0) && (rows > (std::numeric_limits< std::size_t >::max() / columns))) {
      return false;
    }
    elementCount = rows * columns;
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
        bool isLocalMaximum = true;
        for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
          for (int columnOffset = -1; columnOffset <= 1; ++columnOffset) {
            if ((rowOffset == 0) && (columnOffset == 0)) {
              continue;
            }
            const std::size_t neighborRow = static_cast< std::size_t >(static_cast< long long >(row) + rowOffset);
            const std::size_t neighborColumn
                = static_cast< std::size_t >(static_cast< long long >(column) + columnOffset);
            if (value <= values[neighborRow][neighborColumn]) {
              isLocalMaximum = false;
            }
          }
        }
        if (isLocalMaximum) {
          ++count;
        }
      }
    }
    return count;
  }

  long long minSumOnMainDiagonals(const int* const* values, std::size_t rows, std::size_t columns)
  {
    bool hasDiagonal = false;
    long long minSum = 0;
    for (std::size_t startColumn = 0; startColumn < columns; ++startColumn) {
      long long sum = 0;
      std::size_t row = 0;
      std::size_t column = startColumn;
      while (row < rows) {
        sum += values[row][column];
        ++row;
        if (column == 0) {
          break;
        }
        --column;
      }
      if ((!hasDiagonal) || (sum < minSum)) {
        minSum = sum;
        hasDiagonal = true;
      }
    }
    for (std::size_t startRow = 1; startRow < rows; ++startRow) {
      long long sum = 0;
      std::size_t row = startRow;
      std::size_t column = columns - 1;
      while (row < rows) {
        sum += values[row][column];
        ++row;
        if (column == 0) {
          break;
        }
        --column;
      }
      if ((!hasDiagonal) || (sum < minSum)) {
        minSum = sum;
        hasDiagonal = true;
      }
    }
    return hasDiagonal ? minSum : 0;
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

  int** allocateMatrix(std::size_t rows, std::size_t columns, std::size_t elementCount)
  {
    if (elementCount == 0) {
      return nullptr;
    }
    const std::size_t maxSize = std::numeric_limits< std::size_t >::max();
    if ((rows > (maxSize / sizeof(int*))) || (elementCount > (maxSize / sizeof(int)))) {
      return nullptr;
    }
    int* elements = nullptr;
    int** matrix = nullptr;
    try {
      elements = new int[elementCount];
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
    std::size_t elementCount = 0;
    if (!readDimensions(input, rows, columns, elementCount)) {
      std::cerr << "Error: the input file does not contain valid dimensions.\n";
      return false;
    }
    if ((mode == fixedArrayMode) && (elementCount > static_cast< std::size_t >(maxFixedElements))) {
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
    int** matrix = allocateMatrix(rows, columns, elementCount);
    if ((elementCount != 0) && (matrix == nullptr)) {
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
  const char* modeText = argv[1];
  const char* inputPath = argv[2];
  const char* outputPath = argv[3];
  if (korchevskii::pathsAreEqual(inputPath, outputPath)) {
    std::cerr << "Error: input and output files must have different paths.\n";
    return 1;
  }
  int mode = 0;
  if (!korchevskii::parseMode(modeText, mode)) {
    std::cerr << "Error: mode must be 1 (fixed array) or 2 (dynamic array).\n";
    return 1;
  }
  std::ifstream input{inputPath};
  if (!input) {
    std::cerr << "Error: cannot open the input file.\n";
    return 1;
  }
  std::ofstream output{outputPath};
  if (!output) {
    std::cerr << "Error: cannot open the output file.\n";
    return 1;
  }
  return korchevskii::run(input, output, mode) ? 0 : 2;
}
