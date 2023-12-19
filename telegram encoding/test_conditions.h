#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <sstream>



class TestConditions {
  public:
    static std::unordered_set<std::string> substitutionTable;
    // Function to check each 11-bit word in the binary string and store them in an array
    // Function to check 11-bit words in a binary string and store them in an array
    static bool check11BitWordsAndStore(const std::string& binaryString) {
        if (binaryString.length() % 11 != 0) {
            std::cerr << "The binary string length is not a multiple of 11." << std::endl;
            return false;
        }
    
        std::vector<std::string> wordsArray;
        wordsArray.reserve(binaryString.length() / 11);
    
        for (size_t i = 0; i < binaryString.length(); i += 11) {
            wordsArray.push_back(binaryString.substr(i, 11));
        }

        // Print the elements of wordsArray
        std::cout << "Words Array:" << std::endl;
        for (size_t i = 0; i < wordsArray.size(); ++i) {
            std::cout << "Index " << i << ": " << wordsArray[i] << std::endl;
        }
      
        std::vector<std::string> octalArray = convertBinaryArrayToOctal(wordsArray);

        std::cout << "octal array size: " << octalArray.size() << std::endl;

        std::cout<< "octal array: " << std::endl;
        for (size_t i = 0; i < octalArray.size(); ++i) {
          std::cout << octalArray[i] << std::endl;
        }
      
        // for (const std::string& octalString : octalArray) {
        //   std::cout << "octalArray" << octalString << std::endl;

        // }
    
        // Iterate through the octalArray and check each element against the substitution table
        for (size_t i = 0; i < octalArray.size(); ++i) {
            if (substitutionTable.find(octalArray[i]) == substitutionTable.end()) {
                // Element is not present in the substitution table
                std::cerr << "Test failed: Element not found in substitution table - " << octalArray[i]
                          << " at index " << i << std::endl;
                return false;
            }
        }

        // All elements were present in the substitution table
        return true;
    }


    // Function to convert an 11-bit binary string to an octal string
    static std::string binaryToOctal(const std::string& binaryString) {
          // Ensure the binary string is not longer than 11 bits
          if (binaryString.length() > 11) {
              throw std::invalid_argument("The binary string is longer than 11 bits.");
          }
  
          // Convert binary string to a decimal number
          unsigned long decimalValue = std::bitset<11>(binaryString).to_ulong();
  
          // Convert decimal number to octal
          std::ostringstream octalStream;
          octalStream << std::oct << decimalValue;
  
          // Pad the octal number with zeros on the left side to ensure it is 4 digits
          std::string octalString = octalStream.str();
          if (octalString.length() < 5) {
              octalString = std::string(5 - octalString.length(), '0') + octalString;
          }
  
          return octalString;
      }

    // Function to convert an array of binary strings to an array of octal strings
    static std::vector<std::string> convertBinaryArrayToOctal(const std::vector<std::string>& binaryArray) {
          std::vector<std::string> octalArray;
          for (const std::string& binaryString : binaryArray) {
              try {
                  std::string octalString = binaryToOctal(binaryString);
                  octalArray.push_back(octalString);
              } catch (const std::exception& e) {
                  std::cerr << "Error converting binary to octal: " << e.what() << std::endl;
              }
          }
          return octalArray;
    }

    //static bool IsOffSynchParsingConditionSatisfied(const std::string& binaryString) {
    //  for (size_t i = 0; i < binaryString.length(); i++) {
    //      int index = binaryString.length() - i;
    //      if (index % 11 != 0) { // i should NOT be a multiple of 11
    //          int synchIndex = binaryString.length() - i;
    //          int currentLength = 0; // the length of consecutive valid words
    //          bool isSyncPoint = ((synchIndex + 1) % 11 == 0) || ((synchIndex - 1) % 11 == 0);

    //          for (size_t j = i; j <= binaryString.length() - 11; j += 11) {
    //              std::string currentWord = binaryString.substr(j, 11);

    //              std::string octalWord = binaryToOctal(currentWord);

    //                if (std::find(substitutionTable.begin(), substitutionTable.end(), octalWord) != substitutionTable.end()) {
    //                  currentLength++;

    //                  if (isSyncPoint && currentLength > 2) {
    //                        return false; // Return false if the limit is exceeded based on sync point
    //                  } else if (!isSyncPoint && currentLength > (binaryString.length() > 341 ? 10 : 6)) {
    //                      return false; // Return false if the limit is exceeded based on non-sync point
    //                  }
    //                } else {
    //                    currentLength = 0;
    //                }
    //            }
    //        } else {
    //            std::cout << "Skip: It's a multiple of 11" << std::endl;
    //        }
    //  }
    //  return true;
    //}
    static bool IsOffSynchParsingConditionSatisfied(const std::string& binaryString) {
        const size_t telegramSize = binaryString.length();
        for (size_t offset = 1; offset <= 10; ++offset) {
            int max_cvw = (offset == 1 || offset == 10) ? 2 : ((telegramSize > 341) ? 10 : 6);
            int n_cvw = 0;

            for (size_t i = offset; i <= telegramSize - 11; i += 11) {
                std::string currentWord = binaryString.substr(i, 11);
                std::string octalWord = binaryToOctal(currentWord);

                if (std::find(substitutionTable.begin(), substitutionTable.end(), octalWord) != substitutionTable.end()) {
                    n_cvw++;
                    if (n_cvw > max_cvw) {
                        return false; // Exceeded max consecutive valid words
                    }
                }
                else {
                    n_cvw = 0; // Reset the counter for consecutive valid words
                }
            }
        }
        return true;
    }

    //static bool checkUnderSamplingCondition(const std::string& binaryString) {
    //    const int maxLength = 30; // The maximum length of valid words run
    //    const size_t n = binaryString.size();

    //    // Iterate over k = 1, 2, 3, 4 for under-sampling by factors of j*2*k
    //    for (int k = 1; k <= 4; ++k) {
    //        std::string underSampledString;

    //        // Generate the under-sampled string in a cyclic manner
    //        for (size_t j = 0; underSampledString.length() < n; ++j) {
    //            size_t index = (j * 2 * k) % n; // Calculate the cyclic index
    //            underSampledString.push_back(binaryString[index]);
    //            /*std::cout <<"undersample"<< underSampledString << std::endl;
    //            std::cout << "index" << index << std::endl;*/

    //        }
    //
    //        // Check for valid words in the under-sampled string
    //        for (size_t i = 0; i <= underSampledString.length() - 11; i += 11) {
    //            std::string word = underSampledString.substr(i, 11);
    //
    //            if (check11BitWordsAndStore(word)) {
    //                int validRunLength = 1; // We've found one valid word
    //
    //                // Check the length of consecutive valid words
    //                for (size_t j = i + 11; j <= underSampledString.length() - 11; j += 11) {
    //                    word = underSampledString.substr(j, 11);
    //
    //                    if (check11BitWordsAndStore(word)) {
    //                        validRunLength++;
    //
    //                        // If the run length exceeds maxLength, the condition is not satisfied
    //                        if (validRunLength > maxLength) {
    //                            return false;
    //                        }
    //                    } else {
    //                        // If a word is not valid, reset the run length
    //                        validRunLength = 0;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //
    //    // If none of the under-sampled strings had a run of valid words longer than maxLength,
    //    // the condition is satisfied
    //    return true;
    //}

    static bool checkUnderSamplingCondition(const std::string& binaryString) {
        const int maxLength = 30;  // The maximum length of valid words run
        const size_t telegramSize = binaryString.size();

        // Iterate over factors of 2, 4, 8, and 16 (2^k for k = 1, 2, 3, and 4)
        for (int k = 1; k <= 4; k *= 2) {
            // Iterate for each starting bit i in the range [0, k)
            for (int i = 0; i < k; i++) {
                std::string underSampledString;
                underSampledString.reserve(telegramSize / k + 1);  // Reserve space to avoid reallocations

                // Generate the under-sampled string
                for (size_t j = 0; j < telegramSize; j++) {
                    size_t index = (j * k + i) % telegramSize;  // Calculate the index with wraparound
                    underSampledString.push_back(binaryString[index]);
                }

                // Check for maximum run of valid words in the under-sampled string
                int validRunLength = 0;  // Length of the current run of valid words
                for (size_t j = 0; j <= underSampledString.size() - 11; j += 11) {
                    std::string word = underSampledString.substr(j, 11);

                    if (check11BitWordsAndStore(word)) {
                        validRunLength++;

                        // If the run length exceeds maxLength, the condition is not satisfied
                        if (validRunLength > maxLength) {
                            return false;
                        }
                    }
                    else {
                        // Reset the run length if a word is not valid
                        validRunLength = 0;
                    }
                }
            }
        }

        // If none of the under-sampled strings had a run of valid words longer than maxLength,
        // the condition is satisfied
        return true;
    }

      
};