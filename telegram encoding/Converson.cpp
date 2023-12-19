// Conversion.cpp
#include "Conversion.h"
#include <bitset>
#include <fstream>
#include <iostream> 
#include <unordered_map>
//#include <bits/stdc++.h>
using namespace std;

void createMap(unordered_map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
}


std::string Conversion::binaryToASCII(const std::string &binaryStr) {
  // Calculate padding length
  int paddingLength = (8 - binaryStr.length() % 8) % 8;

  // Pad the binary string with zeros at the beginning
  std::string paddedBinary = std::string(paddingLength, '0') + binaryStr;

  std::string asciiStr = "";
  for (size_t i = 0; i < paddedBinary.length(); i += 8) {
    // Extract 8-bit substring
    std::string byteString = paddedBinary.substr(i, 8);

    // Convert binary string to char
    char character = static_cast<char>(std::bitset<8>(byteString).to_ulong());

    // Append character to ASCII string
    asciiStr += character;
  }
  return asciiStr;
}

//void Conversion::saveToFile(const std::string &asciiStr,
//                            const std::string &filename) {
//  std::ofstream file(filename + ".tlg");
//  if (file.is_open()) {
//    file << asciiStr;
//    file.close();
//  } else {
//    std::cerr << "Unable to open file for writing." << std::endl;
//  }
//}

string Conversion::convertBinToHex(string& bin) {
    int l = bin.size();

    // add minimum 0's at the end to make
    // the string length divisible by 4
    int padding = (4 - l % 4) % 4;
    bin.append(padding, '0');

    // create a map between binary and its
    // equivalent hex code
    unordered_map<string, char> bin_hex_map;
    createMap(&bin_hex_map);

    string hex = "";
    for (int i = 0; i < bin.size(); i += 4) {
        // extract from left, a substring of size 4 and add its hex code
        hex += bin_hex_map[bin.substr(i, 4)];
    }

    // return the hexadecimal number
    return hex;
}


void Conversion::storeHexInTlgFile(const std::string& hexOutput, const std::string& filename) {
    // Add .tlg extension to the filename
    std::string tlgFilename = filename + ".hex";

    // Open the file in binary mode
    std::ofstream tlgFile(tlgFilename, std::ios::binary);

    if (!tlgFile.is_open()) {
        std::cerr << "Error opening file: " << tlgFilename << std::endl;
        return;
    }

    // Write the hex output to the file
    tlgFile << hexOutput;

    // Close the file
    tlgFile.close();

    std::cout << "Hexadecimal content stored in " << tlgFilename << std::endl;
}

std::string Conversion::hexStringToAscii(const std::string& hexStr) {
    std::string asciiStr;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        std::string part = hexStr.substr(i, 2);
        char ch = static_cast<char>(std::stoul(part, nullptr, 16));
        asciiStr += ch;
    }
    return asciiStr;
}



void Conversion::saveToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::binary);

    if (file.is_open()) {
        file << data;
        file.close();
    }
    else {
        std::cerr << "Unable to open file." << std::endl;
    }
}









