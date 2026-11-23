#include <iostream>
#include <limits>

namespace drugov
{
  const int error_invalid_input = 1;
  const int error_calculation = 2;

  int process()
  {
    int n = 0;
    int old_n = 0;

    if (not(std::cin >> n))
    {
      std::cerr << "invalid input\n";
      return error_invalid_input;
    }

    if (n == 0)
    {
      std::cout << 0 << "\n" << 0 << "\n";
      return 0;
    }

    int max_four = 1;
    int cur_four = 1;
    bool ok_four = true;
    int max_six = 1;
    int cur_six = 1;
    bool ok_six = true;

    old_n = n;
    std::cin >> n;

    while (n != 0)
    {
      if (!std::cin)
      {
        std::cerr << "invalid input\n";
        return error_invalid_input;
      }

      if (ok_four)
      {
        if (n == old_n)
        {
          if (cur_four == std::numeric_limits< decltype(cur_four) >::max())
          {
            std::cerr << "overflow error\n";
            ok_four = false;
          }
          else
          {
            cur_four++;
          }
        }
        else
        {
          cur_four = 1;
        }

        if (cur_four > max_four)
        {
          max_four = cur_four;
        }
      }

      if (ok_six)
      {
        if (n >= old_n)
        {
          if (cur_six == std::numeric_limits< decltype(cur_six) >::max())
          {
            std::cerr << "overflow error\n";
            ok_six = false;
          }
          else
          {
            cur_six++;
          }
        }
        else
        {
          cur_six = 1;
        }

        if (cur_six > max_six)
        {
          max_six = cur_six;
        }
      }

      old_n = n;
      std::cin >> n;
    }

    if (!std::cin)
    {
      std::cerr << "invalid input\n";
      return error_invalid_input;
    }

    if (ok_four)
    {
      std::cout << max_four << "\n";
    }
    if (ok_six)
    {
      std::cout << max_six << "\n";
    }

    if (not(ok_four and ok_six))
    {
      return error_calculation;
    }
    return 0;
  }
}

int main()
{
  return drugov::process();
}
