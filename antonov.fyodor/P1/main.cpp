#include <iostream>
#include <limits>

namespace antonov
{

  enum StatusCode
  {
    status_ok = 0,
    status_invalid_input = 1,
    status_cannot_calculate = 2
  };

  static const char invalid_input[] = "Error: invalid input\n";
  static const char div_rem_impossible[] = "Error: cannot calculate DIV-REM\n";
  static const char div_rem_overflow_message[] = "Error: overflow in DIV-REM count\n";
  static const char sgn_chg_overflow_message[] = "Error: overflow in SGN-CHG count\n";

  static bool isNegativeOne(const int value)
  {
    return value < 0 && value + 1 == 0;
  }

  static bool dividesEvenly(const int previous, const int current)
  {
    if (previous == 0 || isNegativeOne(previous))
    {
      return previous != 0;
    }
    return current % previous == 0;
  }

  static bool increment(int &counter)
  {
    if (counter == std::numeric_limits< int >::max())
    {
      return false;
    }
    ++counter;
    return true;
  }

  static bool signsDiffer(const int previous, const int current)
  {
    return (previous < 0) != (current < 0);
  }

  int countCharacteristics()
  {
    int current = 0;
    int previous = 0;
    bool has_previous = false;
    bool has_pair = false;

    int div_rem_count = 0;
    int sgn_chg_count = 0;
    bool div_rem_overflow = false;
    bool sgn_chg_overflow = false;

    while (std::cin >> current)
    {
      if (current == 0)
      {
        break;
      }

      if (has_previous)
      {
        has_pair = true;
        if (!div_rem_overflow && dividesEvenly(previous, current) && !increment(div_rem_count))
        {
          div_rem_overflow = true;
        }
        if (!sgn_chg_overflow && signsDiffer(previous, current) && !increment(sgn_chg_count))
        {
          sgn_chg_overflow = true;
        }
      }

      previous = current;
      has_previous = true;
    }

    if (!std::cin)
    {
      std::cerr << invalid_input;
      return status_invalid_input;
    }

    int status = status_ok;
    if (!has_pair)
    {
      std::cerr << div_rem_impossible;
      status = status_cannot_calculate;
    }
    else if (div_rem_overflow)
    {
      std::cerr << div_rem_overflow_message;
      status = status_cannot_calculate;
    }
    else
    {
      std::cout << div_rem_count << '\n';
    }

    if (sgn_chg_overflow)
    {
      std::cerr << sgn_chg_overflow_message;
      status = status_cannot_calculate;
    }
    else
    {
      std::cout << sgn_chg_count << '\n';
    }

    return status;
  }

}

int main()
{
  return antonov::countCharacteristics();
}
