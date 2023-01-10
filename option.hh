#pragma once

#include <math.h>
#include <random>
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
  double monte_carlo_gbm(uint32_t n_sim);
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

double Option::monte_carlo_gbm(uint32_t n_sim) {
  double drift = (_rate - 0.5 * _vol * _vol) * _T;
  double diffusion = _vol * sqrt(_T);

  double payoff_sum = 0.0;

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0.0, 1.0);

  for (int i = 0; i < n_sim; i++) {
    double Z = distribution(generator);
    double S_forward = _price * exp(drift + diffusion * Z);

    if (_type == 'C')
      payoff_sum += std::max(S_forward - _strike, 0.0);
    else
      payoff_sum += std::max(_strike - S_forward, 0.0);
  }

  return (payoff_sum / n_sim) * exp(-_rate * _T);

}