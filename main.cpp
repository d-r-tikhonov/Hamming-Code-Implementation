#include <iostream>
#include "hamming_code.h"

int main()
{
    uint32_t n_bits = 16;
    uint32_t input_message = 0x4235;
    std::string encoded_message = hamming_code::encode(input_message, n_bits);
    std::cout << "Encoded message: 0x" << std::hex << std::stoul(encoded_message, 0, 2) << std::endl; // prints "Encoded message: 0x8a3ac"
    std::string decoded_message = hamming_code::decode(std::stoul(encoded_message, 0, 2), encoded_message.length());
    std::cout << "Decoded message: 0x" << std::hex << std::stoul(decoded_message, 0, 2) << std::endl; // prints "Decoded message: 0x4235"

    return 0;
}