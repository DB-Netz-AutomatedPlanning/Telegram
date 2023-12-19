#include "encoding.h"
#include <algorithm>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;

std::string Encoding::determine_U(const std::string& binaryString) {
    const int blockSize = 10;
    int numBlocks = binaryString.size() / blockSize;
    std::vector<uint32_t> binaryBlocks;

    // Partition the binary string into 10-bit blocks
    for (int i = 0; i < numBlocks; ++i) {
        std::string block = binaryString.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * blockSize, blockSize);
        binaryBlocks.push_back(std::bitset<blockSize>(block).to_ulong());
        cout << "binary block before first block change: " << binaryBlocks[i] << endl;
        
    }
    string firstBlock = bitset<blockSize>(binaryBlocks[0]).to_string();
    cout << "first block" << firstBlock << endl;
      
    // Calculate the sum of all blocks except the first, modulo 2^10
    uint32_t sum = 0;
    for (int i = 0; i < numBlocks; ++i) {
        sum += binaryBlocks[i];
    }
    sum %= 1024;

    // Replace the first block with the calculated sum
    std::string modifiedBinaryString = std::bitset<blockSize>(sum).to_string();
    for (int i = 1; i < numBlocks; ++i) {
        modifiedBinaryString += std::bitset<blockSize>(binaryBlocks[i]).to_string();
        
    }

    return modifiedBinaryString;
}

uint32_t Encoding::determine_S(const std::string& sb) {

    uint32_t sbValue = std::bitset<12>(sb).to_ulong();
    // The multiplication result automatically fits in a 32-bit unsigned integer
    uint32_t S = sbValue * 2801775573UL;
    cout << "S integer" << S << endl;
    string binaryS = bitset<32>(S).to_string();
    cout << "S binary" << binaryS << endl;

    return S;


}

std::string Encoding::lfsr(const std::string& bitStream, uint32_t initialSeed) {
    // Define the feedback polynomial H
    static const uint32_t H = (1u << 31) | (1u << 30) | (1u << 29) | (1u << 27) | (1u << 25) | 1;

    // Initialize the LFSR state (S) with the initial seed
    uint32_t S = initialSeed;
    std::string scrambledBits;

    for (char bit : bitStream) {
        // Get the most significant bit of the LFSR
        char t = (S >> 31) & 1; // Ensures we get only the last bit
        
        // XOR the bit with the LFSR output bit to get the scrambled bit
        char scrambledBit = ((bit - '0') ^ t) + '0'; // Convert to char

        // Shift the LFSR left by one position and apply H if needed
        S <<= 1;
        if (scrambledBit == '1') {
            S ^= H;
        }

        // Append the scrambled bit to the result
        scrambledBits += scrambledBit;
    }

    return scrambledBits;
}

string Encoding::setControlBits(const std::string& cb) {
    
    return cb;
}

void Encoding::bitTransformation(const std::string& lfsrOutput,
    std::vector<std::string>& binaryBlocks,
    std::vector<std::string>& substitutionList,
    std::vector<std::string>& octToBin) {
    uint32_t numBlocks = lfsrOutput.length() / 10;

    std::vector<std::string> values = {
        "00101", "00102", "00103", "00104", "00105", "00106", "00107", "00110",
        "00111", "00112", "00113", "00114", "00115", "00116", "00117", "00120",
        "00121", "00122", "00123", "00124", "00125", "00126", "00127", "00130",
        "00131", "00132", "00133", "00134", "00135", "00141", "00142", "00143",
        "00144", "00145", "00146", "00147", "00150", "00151", "00152", "00153",
        "00154", "00155", "00156", "00157", "00160", "00161", "00162", "00163",
        "00164", "00165", "00166", "00167", "00170", "00171", "00172", "00173",
        "00174", "00175", "00176", "00201", "00206", "00211", "00214", "00216",
        "00217", "00220", "00222", "00223", "00224", "00225", "00226", "00231",
        "00233", "00244", "00245", "00246", "00253", "00257", "00260", "00261",
        "00272", "00273", "00274", "00275", "00276", "00301", "00303", "00315",
        "00317", "00320", "00321", "00332", "00334", "00341", "00342", "00343",
        "00344", "00346", "00352", "00353", "00357", "00360", "00374", "00376",
        "00401", "00403", "00404", "00405", "00406", "00407", "00410", "00411",
        "00412", "00413", "00416", "00417", "00420", "00424", "00425", "00426",
        "00427", "00432", "00433", "00442", "00443", "00445", "00456", "00457",
        "00460", "00461", "00464", "00465", "00470", "00471", "00472", "00474",
        "00475", "00476", "00501", "00502", "00503", "00504", "00505", "00506",
        "00507", "00516", "00517", "00520", "00521", "00522", "00523", "00524",
        "00525", "00530", "00531", "00532", "00533", "00534", "00535", "00544",
        "00545", "00546", "00547", "00550", "00551", "00552", "00553", "00554",
        "00555", "00556", "00557", "00560", "00561", "00562", "00563", "00571",
        "00573", "00576", "00601", "00602", "00604", "00605", "00610", "00611",
        "00612", "00613", "00614", "00615", "00616", "00617", "00620", "00621",
        "00622", "00623", "00624", "00625", "00626", "00627", "00630", "00634",
        "00635", "00644", "00645", "00646", "00647", "00650", "00651", "00652",
        "00653", "00654", "00655", "00656", "00657", "00660", "00661", "00662",
        "00663", "00666", "00667", "00672", "00674", "00675", "00676", "00701",
        "00712", "00713", "00716", "00717", "00720", "00721", "00722", "00723",
        "00730", "00731", "00732", "00733", "00734", "00735", "00742", "00743",
        "00744", "00745", "00746", "00747", "00750", "00751", "00752", "00753",
        "00754", "00755", "00756", "00757", "00760", "00761", "00764", "00765",
        "00766", "00767", "00772", "00773", "00776", "01001", "01004", "01005",
        "01016", "01017", "01020", "01021", "01022", "01023", "01024", "01025",
        "01030", "01031", "01032", "01033", "01034", "01035", "01043", "01044",
        "01045", "01046", "01047", "01054", "01057", "01060", "01061", "01062",
        "01075", "01076", "01101", "01102", "01103", "01110", "01114", "01115",
        "01116", "01117", "01120", "01121", "01122", "01123", "01124", "01125",
        "01126", "01127", "01130", "01131", "01132", "01133", "01142", "01143",
        "01144", "01145", "01146", "01147", "01151", "01152", "01153", "01154",
        "01155", "01156", "01157", "01160", "01164", "01166", "01167", "01176",
        "01201", "01214", "01217", "01220", "01221", "01222", "01223", "01224",
        "01225", "01226", "01227", "01230", "01231", "01232", "01233", "01243",
        "01244", "01245", "01253", "01254", "01255", "01256", "01257", "01260",
        "01261", "01272", "01273", "01274", "01275", "01276", "01301", "01302",
        "01303", "01305", "01306", "01307", "01317", "01320", "01321", "01332",
        "01334", "01335", "01342", "01343", "01344", "01345", "01350", "01351",
        "01352", "01353", "01355", "01356", "01357", "01360", "01361", "01364",
        "01365", "01370", "01371", "01372", "01373", "01374", "01376", "01401",
        "01403", "01406", "01407", "01414", "01415", "01416", "01417", "01420",
        "01424", "01425", "01431", "01433", "01434", "01435", "01443", "01445",
        "01456", "01457", "01460", "01462", "01474", "01475", "01476", "01501",
        "01502", "01503", "01504", "01505", "01516", "01517", "01520", "01524",
        "01532", "01533", "01544", "01546", "01550", "01551", "01552", "01553",
        "01554", "01557", "01560", "01561", "01562", "01563", "01566", "01567",
        "01576", "01601", "01603", "01604", "01605", "01606", "01607", "01610",
        "01611", "01612", "01613", "01614", "01615", "01616", "01617", "01620",
        "01621", "01622", "01623", "01624", "01625", "01626", "01630", "01631",
        "01632", "01633", "01635", "01643", "01644", "01645", "01650", "01651",
        "01652", "01653", "01654", "01655", "01656", "01657", "01660", "01661",
        "01672", "01674", "01675", "01676", "01701", "01720", "01744", "01745",
        "01746", "01747", "01750", "01751", "01752", "01753", "01754", "01755",
        "01756", "01757", "01760", "01761", "01762", "01763", "01764", "01765",
        "01766", "01767", "01770", "01771", "01772", "01773", "01774", "01775",
        "02002", "02003", "02004", "02005", "02006", "02007", "02010", "02011",
        "02012", "02013", "02014", "02015", "02016", "02017", "02020", "02021",
        "02022", "02023", "02024", "02025", "02026", "02027", "02030", "02031",
        "02032", "02033", "02057", "02076", "02101", "02102", "02103", "02105",
        "02116", "02117", "02120", "02121", "02122", "02123", "02124", "02125",
        "02126", "02127", "02132", "02133", "02134", "02142", "02144", "02145",
        "02146", "02147", "02151", "02152", "02153", "02154", "02155", "02156",
        "02157", "02160", "02161", "02162", "02163", "02164", "02165", "02166",
        "02167", "02170", "02171", "02172", "02173", "02174", "02176", "02201",
        "02210", "02211", "02214", "02215", "02216", "02217", "02220", "02223",
        "02224", "02225", "02226", "02227", "02231", "02233", "02244", "02245",
        "02253", "02257", "02260", "02261", "02272", "02273", "02274", "02275",
        "02276", "02301", "02302", "02303", "02315", "02317", "02320", "02321",
        "02332", "02334", "02342", "02343", "02344", "02346", "02352", "02353",
        "02357", "02360", "02361", "02362", "02363", "02370", "02371", "02374",
        "02376", "02401", "02403", "02404", "02405", "02406", "02407", "02412",
        "02413", "02416", "02417", "02420", "02421", "02422", "02424", "02425",
        "02426", "02427", "02432", "02433", "02434", "02435", "02442", "02443",
        "02445", "02456", "02457", "02460", "02470", "02471", "02472", "02474",
        "02475", "02476", "02501", "02502", "02503", "02504", "02505", "02516",
        "02517", "02520", "02521", "02522", "02523", "02524", "02532", "02533",
        "02534", "02544", "02545", "02546", "02547", "02550", "02551", "02552",
        "02553", "02554", "02555", "02556", "02557", "02560", "02563", "02576",
        "02601", "02610", "02611", "02613", "02617", "02620", "02621", "02622",
        "02623", "02624", "02625", "02626", "02630", "02631", "02632", "02633",
        "02634", "02635", "02644", "02645", "02646", "02647", "02650", "02651",
        "02652", "02653", "02654", "02655", "02656", "02657", "02660", "02661",
        "02662", "02663", "02667", "02674", "02675", "02676", "02701", "02702",
        "02715", "02716", "02717", "02720", "02723", "02730", "02731", "02732",
        "02733", "02734", "02742", "02743", "02744", "02745", "02746", "02747",
        "02752", "02753", "02754", "02755", "02756", "02757", "02760", "02761",
        "02772", "02773", "02776", "03001", "03004", "03005", "03010", "03011",
        "03012", "03013", "03016", "03017", "03020", "03021", "03022", "03023",
        "03024", "03025", "03026", "03027", "03030", "03031", "03032", "03033",
        "03034", "03035", "03042", "03043", "03044", "03045", "03046", "03047",
        "03054", "03055", "03056", "03057", "03060", "03061", "03064", "03065",
        "03076", "03101", "03102", "03103", "03105", "03110", "03111", "03114",
        "03115", "03116", "03117", "03120", "03121", "03122", "03123", "03124",
        "03125", "03126", "03127", "03130", "03131", "03132", "03133", "03142",
        "03143", "03147", "03150", "03151", "03152", "03153", "03154", "03155",
        "03156", "03157", "03160", "03161", "03162", "03163", "03164", "03165",
        "03166", "03167", "03172", "03173", "03175", "03176", "03201", "03204",
        "03206", "03214", "03215", "03216", "03217", "03220", "03221", "03222",
        "03223", "03224", "03225", "03226", "03227", "03230", "03231", "03232",
        "03233", "03242", "03243", "03244", "03245", "03246", "03247", "03252",
        "03253", "03254", "03255", "03256", "03257", "03260", "03261", "03270",
        "03271", "03272", "03273", "03274", "03275", "03276", "03301", "03302",
        "03303", "03305", "03306", "03307", "03312", "03313", "03316", "03317",
        "03320", "03321", "03332", "03334", "03335", "03344", "03345", "03350",
        "03351", "03352", "03353", "03357", "03360", "03361", "03364", "03365",
        "03366", "03367", "03370", "03371", "03372", "03373", "03374", "03376",
        "03401", "03403", "03417", "03420", "03424", "03425", "03431", "03433",
        "03434", "03435", "03436", "03443", "03445", "03456", "03457", "03460",
        "03462", "03474", "03476", "03501", "03502", "03503", "03504", "03505",
        "03516", "03517", "03520", "03524", "03531", "03532", "03533", "03544",
        "03546", "03551", "03552", "03553", "03554", "03555", "03557", "03560",
        "03561", "03563", "03566", "03571", "03576", "03601", "03602", "03603",
        "03604", "03605", "03606", "03607", "03610", "03611", "03612", "03613",
        "03614", "03615", "03616", "03617", "03620", "03621", "03622", "03623",
        "03624", "03625", "03626", "03627", "03630", "03631", "03632", "03633",
        "03634", "03635", "03636", "03642", "03643", "03644", "03645", "03646",
        "03647", "03650", "03651", "03652", "03653", "03654", "03655", "03656",
        "03657", "03660", "03661", "03662", "03663", "03664", "03665", "03666",
        "03667", "03670", "03671", "03672", "03673", "03674", "03675", "03676" };

    // Partition lfsrOutput into 10-bit blocks and save them into the common
    // array
    for (int i = 0; i < numBlocks; ++i) {
        std::string block = lfsrOutput.substr(i * 10, 10);
        binaryBlocks.push_back(block);
    }

    for (const auto& block : binaryBlocks) {
        uint32_t decimalValue = std::stoi(block, nullptr, 2);
        if (decimalValue < values.size()) {
            std::string value = values[decimalValue];
            substitutionList.push_back(value);
        }
    }

    for (const auto& octalValue : substitutionList) {
        uint32_t decimalValue = std::stoi(octalValue, nullptr, 8);
        std::string binaryString = std::bitset<11>(decimalValue).to_string();
        octToBin.push_back(binaryString);
    }
};

// encoding.cpp
std::string Encoding::concatenateData(
    const std::string &concatenatedBinaryString, const std::string &controlBits,
    const std::string &scramblingBits, const std::string &extraBits,
    const std::string &checkBits) {
  std::string concatenatedData =
      concatenatedBinaryString + controlBits + scramblingBits + extraBits;

  // If checkBits is provided, append it to the concatenated data.
  if (!checkBits.empty()) {
    concatenatedData += checkBits;
  }
  return concatenatedData;
}


char Encoding::exor(char a, char b) {
    if (a == b)
        return '0';
    else
        return '1';
}

std::string Encoding::binaryStringXORDivision(std::string dividend, const std::string& divisor) {
    int divisorLength = divisor.length();
    int dividendLength = dividend.length();

    // Extend the dividend by the length of the divisor minus one (for CRC division)
    dividend += std::string(divisorLength - 1, '0');

    std::string rem = dividend.substr(0, divisorLength);

    for (int j = divisorLength; j <= dividend.length(); ++j) {
        if (rem[0] == '1') {
            // Perform XOR with the divisor
            for (int i = 0; i < divisorLength; ++i) {
                rem[i] = exor(rem[i], divisor[i]);
            }
        }

        // Shift remainder and append next bit of dividend
        rem = rem.substr(1, divisorLength - 1) + (j < dividend.length() ? dividend[j] : '0');
    }

    return rem.substr(0, divisorLength - 1);
}

std::string Encoding::XorAdd(const std::string& binStr1, const std::string& binStr2) {
    // Determine the maximum length of the two strings
    size_t maxLength = std::max(binStr1.length(), binStr2.length());

    // Strings to hold the padded binary representations
    std::string paddedBinStr1 = binStr1;
    std::string paddedBinStr2 = binStr2;

    // Pad the shorter string with leading zeros
    paddedBinStr1.insert(0, maxLength - binStr1.length(), '0');
    paddedBinStr2.insert(0, maxLength - binStr2.length(), '0');

    // String to store the result
    std::string result;

    // Perform bitwise XOR
    for (size_t i = 0; i < maxLength; ++i) {
        char xorBit = ((paddedBinStr1[i] - '0') ^ (paddedBinStr2[i] - '0')) + '0';
        result += xorBit;
    }

    return result;
}

std::tuple<std::string, std::string, std::string, std::string, std::string,
           std::string>
Encoding::checkBits(const std::string &concatenatedData) {
  std::string fs = "10110101011";
  std::string gs = "1001111101111001000011000010111111101111011111001010010010100011110001001011";



  std::cout << "gs length: " << gs.length() << std::endl;

  std::string remainder;
  std::string sum;
  std::string productFsGs;

  /*productFsGs = gf2Multiplication(gs,fs);*/
  productFsGs = "10101110111001010011011000011101110101011110010101100100000010000110110110110101100101";
  std::cout << "product: " << productFsGs<< std::endl;
  std::cout << "product length:  " << productFsGs.length() << std::endl;
  remainder = binaryStringXORDivision(concatenatedData, productFsGs);
  sum = XorAdd(remainder, gs);
  std::cout << "Remainder: " << remainder << std::endl;
  std::cout << "Remainder length: "
            << remainder.length() << std::endl;

  std::cout << "Sum: " << sum << std::endl;
  std::cout << "Sum length: " << sum.length() << std::endl;


  std::cout << "check bits length " << sum.length() << std::endl;

  return std::make_tuple(concatenatedData, fs, gs, productFsGs,
                         remainder, sum);
}
