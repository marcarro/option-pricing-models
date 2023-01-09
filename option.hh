#pragma once

#include <math.h>
#include <boost/math/distributions/normal.hpp>

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

double Option::black_scholes() {
  double d1 = (log(_price / _strike) + 
              (_rate - _div + 0.5 * _vol * _vol) * _T) / _vol / sqrt(_T);
  double d2 = d1 - _vol * sqrt(_T);
  
  boost::math::normal norm(0.0, 1.0);

  double call = _price * exp(-_div * _T) * boost::math::cdf(norm, d1) - _strike * exp(-_rate * _T) * boost::math::cdf(norm, d2);
  double put = call - _price * exp(-_div * _T) + _strike * exp(-_rate * _T);

  if (_type == 'C')
    return call;
  else
    return put;
}