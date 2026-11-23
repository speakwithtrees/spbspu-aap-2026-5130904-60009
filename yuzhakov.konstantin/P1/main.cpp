#include <iostream>
#include <limits>

namespace yuzhakov {
  constexpr int calculate_error = 2;
  constexpr int invalid_input = 1;
  constexpr int success = 0;

  int p1()
  {
    int s1 = 0;
    if (!(std::cin >> s1)) {
      std::cerr << "invalid input" << "\n";
      return invalid_input;
    }
    if (s1 == 0) {
      std::cout << "0" << "\n";
      std::cerr << "calculate error" << "\n";
      return calculate_error;
    }

    int s2 = 0;
    if (!(std::cin >> s2)) {
      std::cerr << "invalid input" << "\n";
      return invalid_input;
    }
    if (s2 == 0) {
      std::cout << "0" << "\n";
      std::cout << "0" << "\n";
      return success;
    }

    constexpr unsigned long long max_count = std::numeric_limits< unsigned long long >::max();
    unsigned long long c7 = 0, c13 = 0;
    bool c7_overflow = false;
    bool c13_overflow = false;
    int s3 = 0;

    while (std::cin >> s3) {
      if (s3 == 0) {
        break;
      }
      if (!c7_overflow) {
        if (s1 < s2 && s2 > s3) {
          if (c7 == max_count) {
            c7_overflow = true;
          } else {
            c7++;
          }
        }
      }
      long long const a = s1;
      long long const b = s2;
      long long const c = s3;
      if (!c13_overflow) {
        if ((c * c - b * b) == a * a) {
          if (c13 == max_count) {
            c13_overflow = true;
          } else {
            c13++;
          }
        }
      }
      s1 = s2;
      s2 = s3;
    }

    if (!(std::cin)) {
      std::cerr << "invalid input" << "\n";
      return invalid_input;
    }
    if (c7_overflow) {
      std::cerr << "c7 overflow" << "\n";
    } else {
      std::cout << c7 << "\n";
    }
    if (c13_overflow) {
      std::cerr << "c13 overflow" << "\n";
    } else {
      std::cout << c13 << "\n";
    }
    if (c7_overflow || c13_overflow) {
      return calculate_error;
    }
    return success;
  }
}

int main()
{
  return yuzhakov::p1();
}
