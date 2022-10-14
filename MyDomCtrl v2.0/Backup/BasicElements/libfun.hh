// Libreria di funzioni

#ifndef LIBFUN_H
#define LIBFUN_H

#include <string>
#include <iostream>

using std::string;

string IntToString(int);
float StringToFloat(string);
enum tipiModuli { BASE, SONDA, SONDAOW, VALVOLA };
enum modi { LETTURA, SCRITTURA };

#endif
