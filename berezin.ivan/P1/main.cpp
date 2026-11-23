#include <iostream>
#include <limits>

namespace berezin
{

  constexpr int success = 0;
  constexpr int invalid_input_error = 1;
  constexpr int inposible_calculations_error = 2;

  int grtLssDivRem()
  {
    int first = 0;
    if (!(std::cin >> first))
    {
      std::cerr << "Error: invalid input\n";
      return invalid_input_error;
    }
    if (first == 0)
    {
      std::cout << "0\n";
      std::cerr << "Error: not enough input values for DIV-REM\n";
      return inposible_calculations_error;
    }

    int second = 0;
    if (!(std::cin >> second))
    {
      std::cerr << "Error: invalid input\n";
      return invalid_input_error;
    }
    if (second == 0)
    {
      std::cout << "0\n";
      std::cerr << "Error: not enough input values for DIV-REM\n";
      return inposible_calculations_error;
    }

    unsigned int grt_lss_count = 0;
    unsigned int div_rem_count = (second % first) == 0;
    bool grt_lss_overflow = false;
    bool div_rem_overflow = false;

    int prev = first;
    int curr = second;
    int next = 0;

    while (std::cin >> next)
    {
      if (next == 0)
      {
        break;
      }
      if (!grt_lss_overflow)
      {
        const unsigned int inc = (curr < prev) && (curr > next);
        if (std::numeric_limits< unsigned int >::max() - inc < grt_lss_count)
        {
          grt_lss_overflow = true;
        }
        else
        {
          grt_lss_count += inc;
        }
      }
      if (!div_rem_overflow)
      {
        const unsigned int inc = (next % curr) == 0;
        if (std::numeric_limits< unsigned int >::max() - inc < div_rem_count)
        {
          div_rem_overflow = true;
        }
        else
        {
          div_rem_count += inc;
        }
      }
      prev = curr;
      curr = next;
    }

    if (!std::cin)
    {
      std::cerr << "Error: invalid input\n";
      return invalid_input_error;
    }

    int extra = 0;
    if (std::cin >> extra)
    {
      std::cerr << "Error: extra data after \"0\"\n";
      return invalid_input_error;
    }

    if (grt_lss_overflow)
    {
      std::cerr << "Error: overflow in GRT-LSS count\n";
    }
    else
    {
      std::cout << grt_lss_count << '\n';
    }

    if (div_rem_overflow)
    {
      std::cerr << "Error: overflow in DIV-REM count\n";
    }
    else
    {
      std::cout << div_rem_count << '\n';
    }

    if (grt_lss_overflow || div_rem_overflow)
    {
      return inposible_calculations_error;
    }

    return success;
  }

}

int main()
{
  return berezin::grtLssDivRem();
}
