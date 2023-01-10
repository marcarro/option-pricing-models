#include <iostream>

#include "option.hh"

int main() {
  Option call_op(100, 100, 0.2, 0.1, 0, 2, 'C');
  Option put_op(100, 80, 0.2, 0.1, 0, 2, 'P');
  const uint32_t n_sim = 10000000;
  std::cout << "Monte Carlo C " << call_op.monte_carlo_gbm(n_sim) << std::endl;
  std::cout << "Monte Carlo P " << put_op.monte_carlo_gbm(n_sim) << std::endl;
}