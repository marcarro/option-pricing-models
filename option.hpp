#ifndef OPTION_HPP
#define OPTION_HPP
#pragma once

#include <cstdint>

class Option {
protected:
  double m_spot;    // spot price
  double m_strike;  // strike price
  double m_sigma;   // volatility
  double m_rate;    // risk-free interest rate
  double m_div;     // dividend yield
  double m_T;       // option maturity
public:
  Option(double spot, double strike, double sigma, double rate, double div, double T);
  double get_binomial(const uint32_t& n_steps);
};

class Euro_Option : public Option {
public: 
  Euro_Option(double spot, double strike, double sigma, double rate, double div, double T);
};

class Euro_Call : public Euro_Option {
public:
  Euro_Call(double spot, double strike, double sigma, double rate, double div, double T);
  double get_black_scholes();
  double get_monte_carlo(const uint32_t& n_sim);
};

class Euro_Put : public Euro_Option {
public:
  Euro_Put(double spot, double strike, double sigma, double rate, double div, double T);
  double get_black_scholes();
  double get_monte_carlo(const uint32_t& n_sim);
};

class US_Option : public Option {
public:
  US_Option(double spot, double strike, double sigma, double rate, double div, double T);
};

class US_Call : public US_Option {
public:
  US_Call(double spot, double strike, double sigma, double rate, double div, double T);
  double get_monte_carlo(const uint32_t& n_sim);
};

class US_Put : public US_Option {
public:
  US_Put(double spot, double strike, double sigma, double rate, double div, double T);
  double get_monte_carlo(const uint32_t& n_sim);
};

#endif