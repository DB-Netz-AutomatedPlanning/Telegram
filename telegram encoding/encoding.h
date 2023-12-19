#ifndef ENCODING_H
#define ENCODING_H

#include <bitset>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>
#include <cstdint>

class Encoding {
public:
  /*static std::tuple<std::string, std::string, std::string,uint32_t>
  scrambling(const std::string &binaryNumber, uint32_t infoBits, uint32_t SB);*/

  static std::string setControlBits(const std::string& cb);
  static std::string determine_U(const std::string& binaryString);
  static uint32_t determine_S(const std::string& sb);

  static std::string lfsr(const std::string &bitStream,
      uint32_t initialSeed);

  static void bitTransformation(const std::string& lfsrOutput,
      std::vector<std::string>& binaryBlocks,
      std::vector<std::string>& substitutionList,
      std::vector<std::string>& octToBin);
    

  /*static std::string extraShapingBits();*/

  static std::string concatenateData(
      const std::string &concatenatedBinaryString,
      const std::string &controlBits, const std::string &scramblingBits,
      const std::string &extraBits, const std::string &checkBits = "");

  /*static double calculateWeightedSum(const std::string &bitString);*/

  /*static std::vector<uint32_t> convertTo85BitBinary(double number);*/

  static std::tuple<std::string, std::string, std::string, std::string,
                    std::string, std::string>
  checkBits(const std::string &concatenatedData);

  /*static uint32_t calculateB(const std::string &scramblingBits);*/

  // static std::string binarySubtract(std::string larger, std::string smaller);
  // static std::string binaryDivisionRemainder(std::string dividend,
  // std::string divisor);
  static std::string binaryAdd(std::string a, std::string b);

  static std::string binaryStringXORDivision(std::string dividend,
                                             const std::string &divisor);

  static std::string binaryMultiply(std::string a, std::string b);
  static char exor(char a, char b);
  static void crc(char data[], char key[]);
  static std::string gf2Multiplication(const std::string& a, const std::string& b);
  static std::string gf2Addition(const std::string& a, const std::string& b);
  static std::string XorAdd(const std::string& binStr1, const std::string& binStr2);

};

#endif // ENCODING_H