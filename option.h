#pragma once

struct option {
  double price;   // asset price
  double strike;  // strike price
  double vol;     // volatility
  double r;       // risk-free interest rate
  double div;     // dividend yield
  double T;       // option maturity
  char type;      // option type
};