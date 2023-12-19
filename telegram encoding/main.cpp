#include "decimal_functions.h"
#include "encoding.h"
#include "test_conditions.h"
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include "Conversion.h"
#include "tinyxml2.h"

using namespace std;

using namespace tinyxml2;

// Utility function to convert a vector of integers representing bits to a
// binary string
std::string bitsToString(const std::vector<int> &bits) {
  std::string s;
  for (int bit : bits) {
    s += std::to_string(bit);
  }
  return s;
}

//int main() {
  //// Example usage of the processDecimalValues function
  //std::vector<int> decimalValues = {1,  32,  0, 1,  1, 0,   255,  65,  4711,
  //                                  0,  65,  1, 71, 1, 220, 1500, 500, 1,
  //                                  20, 137, 1, 24, 0, 255}; // Predefined
  //                                                           // decimal values
  //std::vector<int> requiredLengths = {
  //    1,  7, 1, 3,  3,  2, 8, 10, 14, 1,  8, 2,
  //    13, 2, 8, 15, 15, 1, 7, 8,  2,  13, 1, 8}; // Predefined required lengths
  int infoBits = 210; // Predefined shaped_data_length


  int main() {
      XMLDocument doc;
      XMLError eResult = doc.LoadFile("C:/Users/MUJ/source/repos/Decoding/Telegram Encoding/Telegram encoding/data.xml"); // Specify the path to your XML file
      if (eResult != XML_SUCCESS) {
          cout << "Error loading XML" << endl;
          return 1;
      }

      XMLNode* pRoot = doc.FirstChild();
      if (pRoot == nullptr) {
          cout << "No root found" << endl;
          return XML_ERROR_FILE_READ_ERROR;
      }

      vector<int> decimalValues;
      vector<int> requiredLengths;
      for (XMLElement* pGroup = pRoot->FirstChildElement("Group"); pGroup != nullptr; pGroup = pGroup->NextSiblingElement("Group")) {
          for (XMLElement* pElement = pGroup->FirstChildElement(); pElement != nullptr; pElement = pElement->NextSiblingElement()) {
              int value;
              eResult = pElement->QueryIntText(&value);
              if (eResult == XML_SUCCESS) {
                  decimalValues.push_back(value);

                  // Extracting the length attribute
                  const char* lengthAttr = pElement->Attribute("length");
                  if (lengthAttr != nullptr) {
                      requiredLengths.push_back(atoi(lengthAttr));
                  }
                  else {
                      cout << "Error reading length attribute for: " << pElement->Value() << endl;
                  }
              }
              else {
                  cout << "Error reading element value: " << pElement->Value() << endl;
              }
          }
      }


  Encoding encoding;
  TestConditions testConditions;
  Conversion conversion;
  
  
  std::pair<std::string, int> result =
      processDecimalValues(decimalValues, requiredLengths, infoBits);
  std::string binaryNumber = result.first;
  infoBits = result.second;

  // Print decimal values and their lengths
  cout << "Decimal Values and Lengths:" << endl;
  for (size_t i = 0; i < decimalValues.size(); ++i) {
      cout << "Value: " << decimalValues[i] << ", Length: " << requiredLengths[i] << endl;
  }

   /*Print the results*/
   std::cout << "Binary number: " << result.first << std::endl;
   std::cout << "Info bits: " << binaryNumber.length() << std::endl;

  // bool validDataFound = false;

  std::string concatenatedBinaryString;
  std::string scrambledOutput;
  std::string controlBits;
  std::string scramblingBits;
  std::string concatenatedData;
  std::string data;
  std::string gsRemainder;
  std::string fsRemainder;
  std::string fs;
  std::string gs;
  std::string checkbits;
  std::string candidateTelegram;
  std::string extraBits;
  std::string Telegram;


  bool validDataFound = false;
  std::vector<int> scramblingBitsVector(12, 0);
  // scramblingBitsVector
  std::fill(scramblingBitsVector.end() - 6, scramblingBitsVector.end(), 1);

  for (bool validScramble = true; validScramble && !validDataFound;
    
    validScramble = std::next_permutation(scramblingBitsVector.begin(), scramblingBitsVector.end())) {

    concatenatedBinaryString.clear();
    scramblingBits = bitsToString(scramblingBitsVector);
    // Convert i to a 12-bit binary string
    /*scramblingBits = std::bitset<12>(i).to_string();*/

    string U = encoding.determine_U(binaryNumber);
    cout << "modified binary string after first block change: " << U << endl;
    cout << "length of modified binary string " << U.length() << endl;

    uint32_t S = encoding.determine_S(scramblingBits);

    string scrambledBits = encoding.lfsr(U, S);

    cout << "scrambled bits: " << scrambledBits << endl;
    cout << "scrambled bits length: " << scrambledBits.length() << endl;

   
    // Create a common array to store the binary blocks
    std::vector<std::string> binaryBlocks;

    // Create a vector to store the substitution list
    std::vector<std::string> substitutionList;
    std::vector<std::string> octToBin;

    std::vector<std::string> shapedDataBlocks;

    // Call the BitTransformation function
    encoding.bitTransformation(scrambledBits, binaryBlocks, substitutionList,
                               octToBin);

    // Print the binary blocks
    std::cout << "Binary blocks:" << std::endl;
    for (const std::string &block : binaryBlocks) {
      std::cout << block << std::endl;
    }

    std::cout << "Length of Binary Blocks: " << binaryBlocks.size()
              << std::endl;

    std::cout << std::endl;

    std::cout << "Substitution List:" << std::endl;
    for (const auto &value : substitutionList) {
      std::cout << value << std::endl;
    }

    std::cout << "octToBin:" << std::endl;
    for (const auto &binaryString : octToBin) {
      std::cout << binaryString << std::endl;
      shapedDataBlocks.push_back(binaryString);
    }

    std::cout << std::endl;
    std::cout << "Shaped Data Blocks:" << std::endl;
    for (const std::string &block : shapedDataBlocks) {
      std::cout << block << std::endl;
    }

    concatenatedBinaryString = std::accumulate(
        shapedDataBlocks.begin(), shapedDataBlocks.end(), std::string(""));

    // Call the function to check the binary string and store the words
    bool alphabetConditionResult =
        testConditions.check11BitWordsAndStore(concatenatedBinaryString);

    if (alphabetConditionResult) {
      std::cout << "Valid data found for scrambling bits: " << scramblingBits
                << std::endl;
      validDataFound = true;
      break; // Exit the loop if valid data is found
    }
  }

  if (!validDataFound) {
    std::cout << "No valid data found for any scrambling bits combination."
              << std::endl;
  }

  
  bool conditionMet =
      false; // Assuming this is declared somewhere to track overall success
  // Initialize vectors with the smallest lexicographical bit combinations (all
  // zeros)
  bool offSynchFlag = false;
  bool underSamplingFlag = false;
  /*std::vector<int> scramblingBitsVector(12, 0);*/
  std::vector<int> extraBitsVector(10, 0);

  //for (size_t i = extraBitsVector.size(); i < extraBitsVector.size(); ++i) {
  //    extraBitsVector[i] = (i % 2 == 0) ? 1 : 0; // Alternates between 1 and 0
  //}

  // Set the starting point to the first combination with six '1' bits for
  // scramblingBitsVector
 /* std::fill(scramblingBitsVector.end() - 6, scramblingBitsVector.end(), 1);*/
  // Set the starting point to the first combination with five '1' bits for
  // extraBitsVector
  std::fill(extraBitsVector.end() - 5, extraBitsVector.end(), 1);

  // Loop over all possible scrambling bits combinations
  for (bool validScramble = true; validScramble && !conditionMet;
    validScramble = std::next_permutation(scramblingBitsVector.begin(),   scramblingBitsVector.end())) {
    std::string scramblingBits = bitsToString(scramblingBitsVector);

    std::string concatenatedBinaryString;

    string U = encoding.determine_U(binaryNumber);
    cout << "modified binary string after first block change: " << U << endl;
    cout << "length of modified binary string " << U.length() << endl;

    uint32_t S = encoding.determine_S(scramblingBits);

    string scrambledBits = encoding.lfsr(U, S);

    cout << "scrambled bits: " << scrambledBits << endl;
    cout << "scrambled bits length: " << scrambledBits.length() << endl;

    string cb = "001";

    controlBits = encoding.setControlBits(cb);

    //int B = encoding.calculateB(scramblingBits);
    //auto scramblingResult = encoding.scrambling(binaryNumber, infoBits, B);
    //controlBits = std::get<2>(scramblingResult);
    //scrambledOutput = encoding.lfsr(std::get<0>(scramblingResult),
    //                                std::get<3>(scramblingResult));

    // Create a common array to store the binary blocks
    std::vector<std::string> binaryBlocks;

    // Create a vector to store the substitution list
    std::vector<std::string> substitutionList;
    std::vector<std::string> octToBin;

    std::vector<std::string> shapedDataBlocks;

    // Call the BitTransformation function
    encoding.bitTransformation(scrambledBits, binaryBlocks, substitutionList,
                               octToBin);

    std::cout << "Substitution List:" << std::endl;
    for (const auto &value : substitutionList) {
      std::cout << value << std::endl;
    }

    std::cout << "octToBin:" << std::endl;
    for (const auto &binaryString : octToBin) {
      std::cout << binaryString << std::endl;
      shapedDataBlocks.push_back(binaryString);
    }

    std::cout << std::endl;
    std::cout << "Shaped Data Blocks:" << std::endl;
    for (const std::string &block : shapedDataBlocks) {
      std::cout << block << std::endl;
    }

    concatenatedBinaryString = std::accumulate(
        shapedDataBlocks.begin(), shapedDataBlocks.end(), std::string(""));

    std::cout << std::endl;
    std::cout << "Shaped Data: " << concatenatedBinaryString << std::endl;
    std::cout << "Length of Concatenated Binary String: "
              << concatenatedBinaryString.length() << std::endl;

    /* bool validExtraBitsFound = false;*/ // Reset for each scrambling bits
    // combination

    // Inner loop for extra bits combinations using std::next_permutation
      // Inner for loop for extra bits combinations using std::next_permutation
    for (bool validExtra = true; validExtra && !conditionMet;
      validExtra = std::next_permutation(extraBitsVector.begin(), extraBitsVector.end())) {
      std::string extraBits = bitsToString(extraBitsVector);

      /*concatenatedBinaryString.clear();*/
      concatenatedData = encoding.concatenateData(
          concatenatedBinaryString, controlBits, scramblingBits, extraBits);

      std::cout << "Concatenated Data: " << concatenatedData << std::endl;
      cout << "concatenated data length: " << concatenatedData.length() << endl;

      // std::cout << std::endl;
      // std::cout << "concatenatedBinaryString: " << concatenatedBinaryString
      // << std::endl; std::cout << "concatenatedBinaryString length: " <<
      // concatenatedBinaryString.length() << std::endl; std::cout << "control
      // Bits: " << controlBits << std::endl; std::cout << "scrambling bits: "
      // << scramblingBits << std::endl; std::cout << "extraBits: " << extraBits
      // << std::endl;

      std::tuple<std::string, std::string, std::string, std::string,
                 std::string, std::string>
          checkedBits = encoding.checkBits(concatenatedData);
      data = std::get<0>(checkedBits);
      fs = std::get<1>(checkedBits);
      gs = std::get<2>(checkedBits);
      gsRemainder = std::get<3>(checkedBits);
      fsRemainder = std::get<4>(checkedBits);
      checkbits = std::get<5>(checkedBits);

      std::cout << "checkbits: " << checkbits << std::endl;

      candidateTelegram =
          encoding.concatenateData(concatenatedBinaryString, controlBits,
                                   scramblingBits, extraBits, checkbits);

      std::cout << "Candidate Telegram length" << candidateTelegram.length()
                << std::endl;

      // Call the function to check the binary string and store the words
      bool alphabetCandidateTelegram =
          testConditions.check11BitWordsAndStore(candidateTelegram);

      if (alphabetCandidateTelegram) {
        // validExtraBitsFound = true;
        conditionMet = true; // Set the overall condition to true
        std::cout << "Valid extra bits found: " << extraBits << std::endl;
        std::cout << std::endl;
        std::cout << "Encoded Message: " << candidateTelegram << std::endl;
        std::cout << "Length of Encoded Message: " << candidateTelegram.length()
                  << std::endl;
        // Exit only the inner loop if valid extra bits are found
      } else {
        std::cout << "Combination invalid alphabet test failed, trying next..." << std::endl;
      }
        
      bool offSynchCandidateTelegram = testConditions.IsOffSynchParsingConditionSatisfied(candidateTelegram);
      if (offSynchCandidateTelegram) {
        // validExtraBitsFound = true;
        offSynchFlag = true; // Set the overall condition to true
        std::cout << "Valid extra bits found for off synch: " << extraBits << std::endl;
        std::cout << std::endl;
        std::cout << "Encoded Message off synch: " << candidateTelegram << std::endl;
        std::cout << "Length of Encoded Message: " << candidateTelegram.length()
                  << std::endl;
        // Exit only the inner loop if valid extra bits are found
      } else {
        std::cout << "Combination invalid off synch test failed, trying next..." << std::endl;
      }

      bool underSamplingCondition = testConditions.checkUnderSamplingCondition(candidateTelegram);

      if (underSamplingCondition) {
        // validExtraBitsFound = true;
        underSamplingFlag = true; // Set the overall condition to true
        std::cout << "Valid extra bits found for under sampling: " << extraBits << std::endl;
        std::cout << std::endl;
        std::cout << "Encoded Message under Sampling: " << candidateTelegram << std::endl;
        std::cout << "Length of Encoded Message: " << candidateTelegram.length()
                  << std::endl;
        // Exit only the inner loop if valid extra bits are found
      } else {
        std::cout << "Combination invalid under sampling test failed, trying next..." << std::endl;
      }
        
      if (conditionMet && offSynchFlag && underSamplingFlag) {
      /*if (conditionMet) {*/
        break; // Break from the outer loop
      }
    }
    if (conditionMet && offSynchFlag && underSamplingFlag) {
    /*if (conditionMet && offSynchFlag) {*/
      break; // Break from the outer loop
    }
  }

  if (!conditionMet && !offSynchFlag && !underSamplingFlag) {
  /*if (!conditionMet && !offSynchFlag) {*/
    std::cerr << "Error: Unable to find a valid combination of scrambling and "
                 "extra bits."
              << std::endl;
    return 1;
  }
  Telegram = candidateTelegram;
  std::cout << "Telegram: " << Telegram << std::endl;
  std::cout << "scrambling bits: " << scramblingBits << std::endl;
  

  std::string hexValue = conversion.convertBinToHex(Telegram);
  std::cout << "Hex: " << hexValue << std::endl;
  

  std::string filename = "telegram_xml.tlg";

  std::string asciiStr = conversion.hexStringToAscii(hexValue);

  conversion.saveToFile(filename, asciiStr);

  std::cout << "Press enter to continue...";
  std::cin.get();

  return 0;
}
