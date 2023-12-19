// Conversion.h
#ifndef CONVERSION_H
#define CONVERSION_H

#include <string>

class Conversion {
public:
    static std::string binaryToASCII(const std::string& binaryStr);
   /* static void saveToFile(const std::string& asciiStr, const std::string& filename);*/
    // static std::string binaryToHex(const std::string& binaryStr);
    // static std::string hexToASCII(const std::string& hexStr);
    static std::string convertBinToHex(std::string& binaryStr);
    static std::string hexStringToAscii(const std::string& hexStr);
    static void saveToFile(const std::string& filename, const std::string& data);

    static void storeHexInTlgFile(const std::string& hexOutput, const std::string& filename);

    /*static std::string hexPairsToAscii(const std::string& hexadecimalString);*/

};

#endif // CONVERSION_H
