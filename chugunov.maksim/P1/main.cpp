#include <iostream>
#include <limits>

namespace chugunov {

  const int IN_ERR = 1;
  const int CALC_ERR = 2;

  bool incCounter(int &counter)
  {
    if (counter == std::numeric_limits< int >::max()) {
      return false;
    }
    ++counter;
    return true;
  }

}

int main()
{
  int x = 0;
  int prev = 0;
  int count = 0;
  int max1 = 0, max2 = 0;
  bool have_max1 = false;
  bool have_max2 = false;
  int div_count = 0;
  bool div_overflow = false;

  while (std::cin >> x && x != 0) {
    if (!have_max1) {
      max1 = x;
      have_max1 = true;
    } else if (x > max1) {
      max2 = max1;
      have_max2 = true;
      max1 = x;
    } else if (x == max1) {
      max2 = x;
      have_max2 = true;
    } else if (!have_max2 || x > max2) {
      max2 = x;
      have_max2 = true;
    }

    if (count > 0 && (prev == -1 || x % prev == 0)) {
      if (!chugunov::incCounter(div_count)) {
        div_overflow = true;
      }
    }

    prev = x;
    if (count < 2) {
      count = count + 1;
    }
  }

  if (std::cin.fail()) {
    std::cerr << "not a seq" << std::endl;
    return chugunov::IN_ERR;
  }

  std::cin >> x;
  if (!std::cin.fail() || !std::cin.eof()) {
    std::cerr << "not a seq" << std::endl;
    return chugunov::IN_ERR;
  }

  int code = 0;
  if (count >= 2 && have_max2) {
    std::cout << max2 << std::endl;
  } else {
    std::cerr << "cant find second max" << std::endl;
    code = chugunov::CALC_ERR;
  }

  if (count < 2) {
    std::cerr << "cant count divs" << std::endl;
    code = chugunov::CALC_ERR;
  } else if (div_overflow) {
    std::cerr << "div count oveflow" << std::endl;
    code = chugunov::CALC_ERR;
  } else {
    std::cout << div_count << std::endl;
  }

  return code;
}
