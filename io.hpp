#ifndef IO_HPP
#define IO_HPP
#pragma once

#include <array>
#include <memory>
#include "option.hpp"

void welcome_message();

void select_option(int& option);

void fill_parameters(std::array<double, 6>& arr);

std::unique_ptr<Option> get_option(const int& option, const std::array<double, 6>& arr);

void choose_model(int& model, int& option);


void display_price(std::unique_ptr<Option>& option, int& model);

#endif