#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/patterns.h>

#include <sstream>
#include <string>

using namespace dealii;



template <typename T>
std::vector<std::pair<std::string, T>>
get_possibilities()
{
  return {};
}

namespace dealii
{
  namespace Patterns
  {
    namespace Tools
    {
      template <class T>
      struct ConvertEnumBase
      {
        static std::unique_ptr<Patterns::PatternBase>
        to_pattern()
        {
          std::stringstream ss;

          const auto possibilities = get_possibilities<T>();

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

          for (const auto &i : get_possibilities<T>())
            if (t == i.second)
              return i.first;

          Assert(false, ExcNotImplemented());

          return get_possibilities<T>()[0].first;
        }

        static T
        to_value(
          const std::string &          s,
          const Patterns::PatternBase &pattern = *Convert<T>::to_pattern())
        {
          (void)pattern;

          for (const auto &i : get_possibilities<T>())
            if (s == i.first)
              return i.second;

          Assert(false, ExcNotImplemented());

          return get_possibilities<T>()[0].second;
        }
      };
    } // namespace Tools
  }   // namespace Patterns
} // namespace dealii

enum class PreconditionerType
{
  Identity,
  AMG
};

template <>
std::vector<std::pair<std::string, PreconditionerType>>
get_possibilities()
{
  return {{"Identity", PreconditionerType::Identity},
          {"AMG", PreconditionerType::AMG}};
}

namespace dealii::Patterns::Tools
{
  template <class T>
  struct Convert<
    T,
    typename std::enable_if<std::is_same<T, PreconditionerType>::value>::type>
    : public ConvertEnumBase<T>
  {};
} // namespace dealii::Patterns::Tools


int
main()
{
  ParameterHandler prm;

  PreconditionerType preconditioner;
  prm.add_parameter("test", preconditioner);

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);

  prm.parse_input("test.json");

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);
}
