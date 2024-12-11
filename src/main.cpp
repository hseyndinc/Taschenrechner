//
// Created by Hseyn Dinc on 11.12.24.
//

#include <iostream>
#include "../include/Calculator.h"

int main() {
  Calculator calc;
  std::cout << "Willkommen zum Taschenrechner !\n"<< std::endl;
  std::cout << "Test der Grundfunktionen:\n";
  std::cout <<"2+3 = " << calc.add(2,3) << '\n';
  std::cout << " 5-3 = " <<calc.subtract(5,3) << '\n';

  return 0;
}