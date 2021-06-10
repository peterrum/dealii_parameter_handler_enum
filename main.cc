#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/patterns.h>

#include <sstream>
#include <string>

using namespace dealii;


namespace dealii
{
  namespace Patterns
  {
    namespace Tools
    {
      template <class T, class U>
      struct ConvertEnumBase
      {
        static std::unique_ptr<Patterns::PatternBase>
        to_pattern()
        {
          std::stringstream ss;

          const auto possibilities = U::get_possibilities();

          ss << possibilities[0].first;

          for (unsigned int i = 1; i < possibilities.size(); ++i)
            ss << "|" << possibilities[i].first;

          return std::make_unique<Patterns::Selection>(ss.str());
        }

        static std::string
        to_string(
          const T &                    t,
          const Patterns::PatternBase &pattern = *Convert<T>::to_pattern())
        {
          (void)pattern;

          for (const auto &i : U::get_possibilities())
            if (t == i.second)
              return i.first;

          Assert(false, ExcNotImplemented());

          return U::get_possibilities()[0].first;
        }

        static T
        to_value(
          const std::string &          s,
          const Patterns::PatternBase &pattern = *Convert<T>::to_pattern())
        {
          (void)pattern;

          for (const auto &i : U::get_possibilities())
            if (s == i.first)
              return i.second;

          Assert(false, ExcNotImplemented());

          return U::get_possibilities()[0].second;
        }
      };
    } // namespace Tools
  }   // namespace Patterns
} // namespace dealii

enum class Test
{
  A,
  B
};

namespace dealii::Patterns::Tools
{
  template <class T>
  struct Convert<T, typename std::enable_if<std::is_same<T, Test>::value>::type>
    : public ConvertEnumBase<T, Convert<T>>
  {
    static std::vector<std::pair<std::string, T>>
    get_possibilities()
    {
      return {{"A", Test::A}, {"B", Test::B}};
    }
  };
} // namespace dealii::Patterns::Tools


int
main()
{
  ParameterHandler prm;

  Test a;
  prm.add_parameter("test", a);

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);

  prm.parse_input("test.json");

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);
}
