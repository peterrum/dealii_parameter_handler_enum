#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/patterns.h>

#include <sstream>
#include <string>

#include "enum.h"

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
      template <typename T>
      struct convert_enum_compatible
      {
      private:
        static void
        detect(...);

        template <typename U>
        static decltype(std::declval<U const>()._values())
        detect(const U &);

      public:
        static const bool value =
          !std::is_same<void, decltype(detect(std::declval<T>()))>::value;
      };

      template <class T>
      struct Convert<
        T,
        typename std::enable_if<convert_enum_compatible<T>::value>::type>
      {
        static std::unique_ptr<Patterns::PatternBase>
        to_pattern()
        {
          std::vector<std::string> values;

          for (const auto i : T::_values())
            values.push_back(i._to_string());

          std::stringstream ss;

          ss << values[0];

          for (unsigned int i = 1; i < values.size(); ++i)
            ss << "|" << values[i];

          return std::make_unique<Patterns::Selection>(ss.str());
        }

        static std::string
        to_string(
          const T &                    t,
          const Patterns::PatternBase &values = *Convert<T>::to_pattern())
        {
          (void)values;

          return t._to_string();
        }

        static T
        to_value(
          const std::string &          s,
          const Patterns::PatternBase &pattern = *Convert<T>::to_pattern())
        {
          (void)pattern;

          return T::_from_string(s.c_str());
        }
      };

    } // namespace Tools
  }   // namespace Patterns
} // namespace dealii

BETTER_ENUM(PreconditionerType, char, Identity, AMG, Jacobi)

int
main()
{
  ParameterHandler prm;

  PreconditionerType preconditioner = PreconditionerType::Identity;
  prm.add_parameter("preconditioner", preconditioner);

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);

  prm.parse_input("test.json");

  prm.print_parameters(std::cout, ParameterHandler::OutputStyle::PRM);
}
