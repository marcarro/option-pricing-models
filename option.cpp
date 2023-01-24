#include "option.hpp"

#include <math.h>
#include <random>
#include <vector>
#include <boost/math/distributions/normal.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>

Option::Option(double spot, double strike, double sigma, double rate, double div, double T):
m_spot { spot }, m_strike { strike }, m_sigma { sigma }, m_rate { rate }, m_div { div }, m_T { T } {}

double Option::get_binomial(const uint32_t& n_steps) {
  double dt = m_T / n_steps;
  double u = exp(m_sigma * sqrt(dt));
  double d = exp(-m_sigma * sqrt(dt));
  double q = (exp((m_rate - m_div) * dt) - d) / (u - d);
  std::vector<std::vector<double>> underlying_tree(n_steps + 1, std::vector<double>(n_steps + 1));

  for (int i = 0; i <= n_steps; i++) {
    for (int j = 0; j <= i; j++) {
      underlying_tree[i][j] = m_spot * pow(u, j) * pow(d, i - j);
    }
  }

  std::vector<std::vector<double>> option_tree(n_steps + 1, std::vector<double>(n_steps + 1));
  for (int i = 0; i <= n_steps; i++) {
    if (typeid(this).name() == "Euro_Call" || typeid(this).name() == "US_Call")
      option_tree[n_steps][i] = std::max(underlying_tree[n_steps][i] - m_strike, 0.0);
    else
      option_tree[n_steps][i] = std::max(m_strike - underlying_tree[n_steps][i], 0.0);
  }

  for (int i = n_steps - 1; i >= 0; i--) {
    for (int j = 0; j <= i; j++) {
      option_tree[i][j] = exp(-m_rate * dt) * (q * option_tree[i + 1][j + 1] + (1 - q) * option_tree[i + 1][j]) ;
    }
  }
  
  return option_tree[0][0];
}

Euro_Option::Euro_Option(double spot, double strike, double sigma, double rate, double div, double T):
Option(spot, strike, sigma, rate, div, T) {}

Euro_Call::Euro_Call(double spot, double strike, double sigma, double rate, double div, double T):
Euro_Option(spot, strike, sigma, rate, div, T) {}

double Euro_Call::get_black_scholes() {
  double d1 = (log(m_spot / m_strike) + (m_rate - m_div + 0.5 * m_sigma * m_sigma) * m_T) / (m_sigma * sqrt(m_T));
  double d2 = d1 - m_sigma * sqrt(m_T);
  
  boost::math::normal norm(0.0, 1.0);
  return m_spot * exp(-m_div * m_T) * boost::math::cdf(norm, d1) - m_strike * exp(-m_rate * m_T) * boost::math::cdf(norm, d2);;
}

double Euro_Call::get_monte_carlo(const uint32_t& n_sim) {
  double drift = (m_rate - 0.5 * m_sigma * m_sigma) * m_T;
  double diffusion = m_sigma * sqrt(m_T);

  double payoff_sum = 0.0;

  boost::random::mt19937 generator;
  boost::random::normal_distribution<> distribution;

  for (int i = 0; i < n_sim; i++) {
    double Z = distribution(generator);
    double S_forward = m_spot * exp(drift + diffusion * Z);
    payoff_sum += std::max(S_forward - m_strike, 0.0);
  }

  return (payoff_sum / n_sim) * exp(-m_rate * m_T);
}

Euro_Put::Euro_Put(double spot, double strike, double sigma, double rate, double div, double T):
Euro_Option(spot, strike, sigma, rate, div, T) {}

double Euro_Put::get_black_scholes() {
  double d1 = (log(m_spot / m_strike) + (m_rate - m_div + 0.5 * m_sigma * m_sigma) * m_T) / (m_sigma * sqrt(m_T));
  double d2 = d1 - m_sigma * sqrt(m_T);
  
  boost::math::normal norm(0.0, 1.0);

  double call = m_spot * exp(-m_div * m_T) * boost::math::cdf(norm, d1) - m_strike * exp(-m_rate * m_T) * boost::math::cdf(norm, d2);
  double put = call - m_spot * exp(-m_div * m_T) + m_strike * exp(-m_rate * m_T);

  return put;
}

double Euro_Put::get_monte_carlo(const uint32_t& n_sim) {
  double drift = (m_rate - 0.5 * m_sigma * m_sigma) * m_T;
  double diffusion = m_sigma * sqrt(m_T);

  double payoff_sum = 0.0;

  boost::random::mt19937 generator;
  boost::random::normal_distribution<> distribution;

  for (int i = 0; i < n_sim; i++) {
    double Z = distribution(generator);
    double S_forward = m_spot * exp(drift + diffusion * Z);
    payoff_sum += std::max(m_strike - S_forward, 0.0);
  }

  return (payoff_sum / n_sim) * exp(-m_rate * m_T);
}

US_Option::US_Option(double spot, double strike, double sigma, double rate, double div, double T):
Option(spot, strike, sigma, rate, div, T) {};

US_Call::US_Call(double spot, double strike, double sigma, double rate, double div, double T):
US_Option(spot, strike, sigma, rate, div, T) {};

double US_Call::get_monte_carlo(const uint32_t& n_sim) {
  double drift = (m_rate - 0.5 * m_sigma * m_sigma) * m_T;
  double diffusion = m_sigma * sqrt(m_T);

  double payoff_sum = 0.0;

  boost::random::mt19937 generator;
  boost::random::normal_distribution<> distribution;

  for (int i = 0; i < n_sim; i++) {
    double Z = distribution(generator);
    double S_forward = m_spot * exp(drift + diffusion * Z);
    double current_sum = std::max(m_strike - S_forward, 0.0);
    payoff_sum += std::max(current_sum, m_strike - m_spot);
  }

  return (payoff_sum / n_sim) * exp(-m_rate * m_T);
}

US_Put::US_Put(double spot, double strike, double sigma, double rate, double div, double T):
US_Option(spot, strike, sigma, rate, div, T) {};

double US_Put::get_monte_carlo(const uint32_t& n_sim) {
  double drift = (m_rate - 0.5 * m_sigma * m_sigma) * m_T;
  double diffusion = m_sigma * sqrt(m_T);

  double payoff_sum = 0.0;

  boost::random::mt19937 generator;
  boost::random::normal_distribution<> distribution;

  for (int i = 0; i < n_sim; i++) {
    double Z = distribution(generator);
    double S_forward = m_spot * exp(drift + diffusion * Z);
    double current_sum = std::max(S_forward - m_strike, 0.0);
    payoff_sum += std::max(current_sum, m_spot - m_strike);
  }

  return (payoff_sum / n_sim) * exp(-m_rate * m_T);
}