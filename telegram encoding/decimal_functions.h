#ifndef DECIMAL_FUNCTIONS_H
#define DECIMAL_FUNCTIONS_H

#include <string>
#include <vector>
#include <utility>

std::pair<std::string, int>
processDecimalValues(const std::vector<int> &decimalValues,
                     const std::vector<int> &requiredLengths, int infoBits);

#endif // DECIMAL_FUNCTIONS_H