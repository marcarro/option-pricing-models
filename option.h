#pragma once

class Option {
private:
  double _price;   // asset price
  double _strike;  // strike price
  double _vol;     // volatility
  double _rate;    // risk-free interest rate
  double _div;     // dividend yield
  double _T;       // option maturity
  char _type;      // option type
public:
  Option(double price, double strike, double vol, double rate, double div, double T, char type):
  _price  { price },
  _strike { strike },
  _vol    { vol },
  _rate   { rate },
  _div    { div },
  _T      { T },
  _type   { type }
  {}
  double black_scholes();
};