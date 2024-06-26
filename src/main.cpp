#include <iostream>
#include <ctime>
#include "hamming_code.hpp"

std::string replaceRandomChar(std::string message) 
{
    srand(time(0));
    int randomIndex = rand() % message.length();

    ASSERT (message[randomIndex] == '0' || message[randomIndex] == '1')

    if (message[randomIndex] == '0') 
    {
        message[randomIndex] = '1';
    } 
    else if (message[randomIndex] == '1') 
    {
        message[randomIndex] = '0';
    }

    return message;
}

int main()
{
    uint32_t n_bits = 16;
    uint32_t input_message = 0x4235;

    std::string encoded_message = hamming_code::encode(input_message, n_bits);
    std::cout << "Encoded message: 0x" << std::hex << std::stoul(encoded_message, 0, 2) << std::endl;

    std::string decoded_message = hamming_code::decode(std::stoul(encoded_message, 0, 2), encoded_message.length());
    std::cout << "Decoded message: 0x" << std::hex << std::stoul(decoded_message, 0, 2) << std::endl;

    std::string encoded_noise_message = replaceRandomChar(encoded_message);
    ASSERT(encoded_message.length() == encoded_noise_message.length())
    std::cout << "\nNoise encoded message: 0x" << std::hex << std::stoul(encoded_noise_message, 0, 2) << std::endl;

    std::string decoded_noise_message = hamming_code::decode(std::stoul(encoded_noise_message, 0, 2), encoded_noise_message.length());
    std::cout << "Decoded noise message: 0x" << std::hex << std::stoul(decoded_noise_message, 0, 2) << std::endl;

    uint32_t input_bin_message = 0b1101001100110101;
    encoded_message = hamming_code::encode(input_message, n_bits);
    std::cout << "\nEncoded message (0b" << utils::int2bin(input_bin_message, n_bits) << "): 0b" << encoded_message << std::endl;

    n_bits = 12;
    uint32_t message = 0xE4F;
    decoded_message = hamming_code::decode(message, n_bits);
    std::cout << "\nDecoded message (0x" << std::hex << message << "): 0x" << std::hex << std::stoul(decoded_message, 0, 2) << std::endl;

    return 0;
}