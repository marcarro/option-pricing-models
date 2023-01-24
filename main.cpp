#include <iostream>
#include "option.hpp"

int main() {
  // Euro_Put my_option(100, 100, 0.2, 0.025, 0, 2.0);
  // std::cout << "Black Scholes: " << my_option.get_black_scholes() << std::endl;
  // std::cout << "Monte Carlo: "   << my_option.get_monte_carlo(100000) << std::endl;
  // std::cout << "Binomial Tree: " << my_option.get_binomial(2500) << std::endl;
  US_Put my_option(40, 39, 0.4, 0.01, 0, 0.25);
  std::cout << "BIN USPUT: " << my_option.get_binomial(2500) << std::endl; 
}