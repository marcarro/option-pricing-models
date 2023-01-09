#include <iostream>

#include "option.hh"

int main() {
  Option call_op(100, 100, 0.6, 0.05, 0, 1, 'C');
  std::cout << call_op.black_scholes() << std::endl;
}