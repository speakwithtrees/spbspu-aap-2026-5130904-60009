#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace korchevskii {
  constexpr int success = 0;
  constexpr int memoryAllocationError = 1;
  constexpr int inputError = 2;
  constexpr int outputError = 2;

  int readLine(char** line) noexcept;
  std::size_t getStringLength(const char* text) noexcept;
  void findCommonSymbols(const char* first, const char* second, char* result) noexcept;
  int hasRepeatedSymbols(const char* text) noexcept;
}

int korchevskii::readLine(char** line) noexcept
{
  if (line == nullptr) {
    return inputError;
  }
  *line = nullptr;

  constexpr std::size_t initialCapacity = 8;
  constexpr std::size_t growthFactor = 2;
  std::size_t capacity = initialCapacity;
  std::size_t length = 0;
  char* buffer = static_cast< char* >(std::malloc(capacity));
  if (buffer == nullptr) {
    return memoryAllocationError;
  }

  char symbol = '\0';
  while (std::cin.get(symbol) && (symbol != '\n')) {
    if (symbol == '\0') {
      std::free(buffer);
      return inputError;
    }

    if (length == capacity - 1) {
      if (capacity > std::numeric_limits< std::size_t >::max() / growthFactor) {
        std::free(buffer);
        return memoryAllocationError;
      }

      const std::size_t newCapacity = capacity * growthFactor;
      char* const newBuffer = static_cast< char* >(std::malloc(newCapacity));
      if (newBuffer == nullptr) {
        std::free(buffer);
        return memoryAllocationError;
      }

      for (std::size_t i = 0; i < length; ++i) {
        newBuffer[i] = buffer[i];
      }
      std::free(buffer);
      buffer = newBuffer;
      capacity = newCapacity;
    }

    buffer[length] = symbol;
    ++length;
  }

  if (!std::cin && (std::cin.bad() || !std::cin.eof() || (length == 0))) {
    std::free(buffer);
    return inputError;
  }

  buffer[length] = '\0';
  *line = buffer;
  return success;
}

std::size_t korchevskii::getStringLength(const char* text) noexcept
{
  std::size_t length = 0;
  while (text[length] != '\0') {
    ++length;
  }
  return length;
}

void korchevskii::findCommonSymbols(const char* first, const char* second, char* result) noexcept
{
  const std::size_t firstLength = getStringLength(first);
  const std::size_t secondLength = getStringLength(second);

  std::size_t result_length = 0;
  for (std::size_t i = 0; i < firstLength; ++i) {
    std::size_t second_count = 0;
    std::size_t result_count = 0;

    for (std::size_t j = 0; j < secondLength; ++j) {
      if (first[i] == second[j]) {
        ++second_count;
      }
    }

    for (std::size_t j = 0; j < result_length; ++j) {
      if (first[i] == result[j]) {
        ++result_count;
      }
    }

    if (result_count < second_count) {
      result[result_length] = first[i];
      ++result_length;
    }
  }
  result[result_length] = '\0';
}

int korchevskii::hasRepeatedSymbols(const char* text) noexcept
{
  constexpr int repeatedSymbolFound = 1;
  constexpr int repeatedSymbolNotFound = 0;
  const std::size_t length = getStringLength(text);
  for (std::size_t i = 0; i < length; ++i) {
    for (std::size_t j = i + 1; j < length; ++j) {
      if (text[i] == text[j]) {
        return repeatedSymbolFound;
      }
    }
  }
  return repeatedSymbolNotFound;
}

int main()
{
  char* input = nullptr;
  const int status = korchevskii::readLine(&input);
  if (status != korchevskii::success) {
    if (status == korchevskii::memoryAllocationError) {
      std::cerr << "Failed to allocate memory for the input line\n";
    } else {
      std::cerr << "Failed to read the input line\n";
    }
    return status;
  }

  constexpr char comparison[] = "abc ef";
  char* const result = static_cast< char* >(std::malloc(sizeof(comparison)));
  if (result == nullptr) {
    std::free(input);
    std::cerr << "Failed to allocate memory for the result\n";
    return korchevskii::memoryAllocationError;
  }

  const int hasRepeats = korchevskii::hasRepeatedSymbols(input);
  korchevskii::findCommonSymbols(input, comparison, result);
  std::cout << hasRepeats << '\n' << result << '\n';

  std::free(result);
  std::free(input);

  if (!std::cout.flush()) {
    std::cerr << "Failed to write the result\n";
    return korchevskii::outputError;
  }
  return korchevskii::success;
}
