#include <iostream>
#include <string>
#include "io.hpp"
using namespace std;

void welcome_message() {
  cout << string(10, '-') << "Welcome to the option pricer!" << string(10, '-') << endl;
  cout << "Please select your type of option:" << endl;
  cout << "1. American Call\n2. American Put\n3. European Call\n4. European Put" << endl;
}

void select_option(int& option) {
  while (option < 1 || option > 4) {
    std::cout << "Select: ";
    cin >> option;
    if (option < 1 || option > 4) cout << "Please select options 1 through 4!" << endl;
  }
}

void fill_parameters(array<double, 6>& arr) {
  cout << "Enter the spot price: "; cin >>              arr[0];
  cout << "Enter the strike price: "; cin >>            arr[1];
  cout << "Enter the volatility: "; cin >>              arr[2];
  cout << "Enter the risk-free interest rate: "; cin >> arr[3];
  cout << "Enter the dividend yield: "; cin >>          arr[4];
  cout << "Enter the option maturity: "; cin >>         arr[5];
}

unique_ptr<Option> get_option(const int& option, const std::array<double, 6>& arr) {
  unique_ptr<Option> ptr;
  switch (option) {
    case 1:
      ptr = unique_ptr<Option>(new US_Call(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]));
      break;
    case 2:
      ptr = unique_ptr<Option>(new US_Put(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]));
      break;
    case 3:
      ptr = unique_ptr<Option>(new Euro_Call(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]));
      break;
    case 4:
      ptr = unique_ptr<Option>(new Euro_Put(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]));
      break;
  }
  return ptr;
}

void pricing_model_message(int& option) {
  cout << "Choose a pricing model:" << endl;
  switch (option) {
    case 1:
    case 2:
      cout << "1. Binomial\n2. Monte Carlo" << endl;
      break;
    case 3:
    case 4:
      cout << "1. Binomial\n2. Monte Carlo\n3. Black Scholes" << endl;
  }
}

void verify_model_input(int& model, int& option) {
  if (option == 1 || option == 2) {
    while (model < 1 || model > 2) {
      cout << "Select: ";
      cin >> model;
      if (model < 1 || model > 2) cout << "Please select options 1 or 2!" << endl;
    }
  } else {
    while (model < 1 || model > 3) {
      cout << "Select: ";
      cin >> model;
      if (model < 1 || model > 3) cout << "Please select options 1 through 3!" << endl;
    }
  }
}

void choose_model(int& model, int& option) {
  pricing_model_message(option);
  verify_model_input(model, option);
}

void display_price(std::unique_ptr<Option>& option, int& model) {
  cout << "Price: ";
  if (model == 1) cout << option->get_binomial(2500);
  else if (model == 2) cout << option->get_monte_carlo(100'000);
  else cout << option->get_black_scholes();
  cout << endl;
}