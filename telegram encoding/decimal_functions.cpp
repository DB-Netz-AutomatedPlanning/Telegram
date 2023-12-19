#include "decimal_functions.h"
#include <bitset>
#include <iostream>
#include <string>

std::pair<std::string, int>
processDecimalValues(const std::vector<int> &decimalValues,
                     const std::vector<int> &requiredLengths, int infoBits) {
  int numValues = decimalValues.size();
  std::string binaryNumber;

  // Convert decimal values to binary with padding
  for (int i = 0; i < numValues; ++i) {
    // Convert decimal to binary
    std::string binaryString = std::bitset<32>(decimalValues[i]).to_string();

    // Remove leading zeros
    binaryString.erase(0, binaryString.find_first_not_of('0'));

    // Check if padding is required
    int currentLength = binaryString.length();
    if (currentLength < requiredLengths[i]) {
      // Calculate the number of zeros to be added
      int zerosToAdd = requiredLengths[i] - currentLength;

      // Add leading zeros
      binaryString = std::string(zerosToAdd, '0') + binaryString;
    }

    // Append the binary value to the binaryNumber string
    binaryNumber += binaryString;
  }

  // Print the converted binary values
  std::cout << "Converted binary values:" << std::endl;
  for (int i = 0; i < numValues; ++i) {
    std::cout << "Decimal value #" << i + 1 << ": " << decimalValues[i]
              << std::endl;
  }

  // Print the concatenated binary number
  std::cout << "Concatenated binary number: " << binaryNumber << std::endl;
  std::cout << "Length of the number: " << binaryNumber.length() << std::endl;

  // Calculate the difference between shaped_data_length and
  // binaryNumber.length()
  int difference = infoBits - binaryNumber.length();

  // Pad binaryNumber with leading zeros if necessary
  if (difference > 0) {
    binaryNumber = binaryNumber + std::string(difference, '0');
  }

  // Call the scrambling function
  // scrambling(binaryNumber, infoBits);
  return std::make_pair(binaryNumber, infoBits);
}