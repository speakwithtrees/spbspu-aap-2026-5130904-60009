#include <iostream>
#include <limits>

namespace korchevskii {
  constexpr int success = 0;
  constexpr int unexpected_input_error = 1;
  constexpr int too_few_elements_error = 2;
  constexpr int counter_overflow_error = 3;

  class SequenceStatistics {
  public:
    int addElement(int value) noexcept;
    int writeResults(std::ostream& out, std::ostream& err) const;

  private:
    int largest_ = 0;
    int second_largest_ = std::numeric_limits< int >::min();
    int minimum_ = 0;
    unsigned long long count_ = 0;
    unsigned long long minimum_count_ = 0;
  };

  int processSequence(std::istream& in, std::ostream& out, std::ostream& err);
}

int korchevskii::SequenceStatistics::addElement(const int value) noexcept
{
  ++count_;
  if (count_ == 1) {
    largest_ = value;
    minimum_ = value;
    minimum_count_ = 1;
    return success;
  }

  if (value >= largest_) {
    second_largest_ = largest_;
    largest_ = value;
  } else if (value > second_largest_) {
    second_largest_ = value;
  }

  if (value == minimum_) {
    if (minimum_count_ == std::numeric_limits< unsigned long long >::max()) {
      return counter_overflow_error;
    }
    ++minimum_count_;
  } else if (value < minimum_) {
    minimum_ = value;
    minimum_count_ = 1;
  }
  return success;
}

int korchevskii::SequenceStatistics::writeResults(std::ostream& out, std::ostream& err) const
{
  int status = success;
  if (count_ < 2) {
    err << "[SUB-MAX] The sequence must contain at least 2 elements\n";
    status = too_few_elements_error;
  } else {
    out << second_largest_ << '\n';
  }

  out << minimum_count_ << '\n';
  return status;
}

int korchevskii::processSequence(std::istream& in, std::ostream& out, std::ostream& err)
{
  SequenceStatistics statistics;
  int input = 0;
  while ((in >> input) && (input != 0)) {
    const int status = statistics.addElement(input);
    if (status != success) {
      err << "Counter overflow\n";
      return status;
    }
  }

  if (in.fail()) {
    err << "Unexpected input\n";
    return unexpected_input_error;
  }
  return statistics.writeResults(out, err);
}

int main()
{
  return korchevskii::processSequence(std::cin, std::cout, std::cerr);
}
