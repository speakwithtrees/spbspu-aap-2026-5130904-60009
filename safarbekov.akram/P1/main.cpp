#include <iostream>
#include <limits>

namespace safarbekov
{
  const int exit_ok = 0;
  const int exit_invalid_input = 1;
  const int exit_cannot_calculate = 2;

  using counter_t = unsigned long long;
  const counter_t counter_max = std::numeric_limits< counter_t >::max();

  bool increment(counter_t & value)
  {
    if (value == counter_max)
    {
      return false;
    }
    ++value;
    return true;
  }

  int run()
  {
    counter_t grt_lss = 0;
    bool grt_lss_overflow = false;

    counter_t aft_max = 0;
    bool aft_max_overflow = false;
    bool has_elements = false;
    int max_value = 0;

    int prev = 0;
    int prev_prev = 0;
    bool has_prev = false;
    bool has_prev_prev = false;

    bool terminated = false;
    int current = 0;

    while (std::cin >> current)
    {
      if (current == 0)
      {
        terminated = true;
        break;
      }

      if (has_prev_prev && prev_prev > prev && prev > current)
      {
        if (!increment(grt_lss))
        {
          grt_lss_overflow = true;
        }
      }

      if (!has_elements || current > max_value)
      {
        max_value = current;
        aft_max = 0;
        aft_max_overflow = false;
        has_elements = true;
      }
      else
      {
        if (!increment(aft_max))
        {
          aft_max_overflow = true;
        }
      }

      prev_prev = prev;
      has_prev_prev = has_prev;
      prev = current;
      has_prev = true;
    }

    if (!terminated)
    {
      std::cerr << "Error: input is not a valid sequence\n";
      return exit_invalid_input;
    }

    int result = exit_ok;

    if (grt_lss_overflow)
    {
      std::cerr << "Error: cannot calculate GRT-LSS\n";
      result = exit_cannot_calculate;
    }
    else
    {
      std::cout << grt_lss << '\n';
    }

    if (!has_elements || aft_max_overflow)
    {
      std::cerr << "Error: cannot calculate AFT-MAX\n";
      result = exit_cannot_calculate;
    }
    else
    {
      std::cout << aft_max << '\n';
    }

    return result;
  }
}

int main()
{
  return safarbekov::run();
}
