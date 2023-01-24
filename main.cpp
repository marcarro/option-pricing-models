#include <array>
#include "option.hpp"
#include "io.hpp"

int main() {
  welcome_message();

  int option_kind{0};
  select_option(option_kind);

  std::array<double, 6> option_params;
  fill_parameters(option_params);

  std::unique_ptr<Option> my_option = get_option(option_kind, option_params);
  
  int model{0};
  choose_model(model, option_kind);

  display_price(my_option, model);
}