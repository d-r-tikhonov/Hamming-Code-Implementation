#ifndef HAMMING_CODE_H
#define HAMMING_CODE_H

#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include "utils.h"
#include "errors.h"

namespace hamming_code
{
    enum class ParityLocation
    {
        DEFAULT,
        MSB,
        LSB
    };

    uint64_t n_parity_bits_required(const uint64_t& n_bits)
    {
        uint64_t p_bits = 1;

        while(true)
        {
            uint64_t lhs = pow(2,p_bits);
            uint64_t rhs = p_bits + n_bits + 1;

            if(lhs < rhs)
                p_bits++;
            else
                break;
        }

        return p_bits;
    }

    std::vector<uint64_t> compute_parity_bit_positions(const uint32_t& parity_bits_amount)
    {
        std::vector<uint64_t> positions;

        for(size_t index = 0; index < parity_bits_amount; index++)
        {
            positions.push_back(pow(2,index) - 1);
        }

        return positions;
    }

    std::vector<uint32_t> compute_parity_bits(const std::string& binary_string, const std::vector<uint64_t>& parity_bit_positions, const bool& inclusive)
    {
        std::vector<unsigned> parity_bits(parity_bit_positions.size());
        std::vector<unsigned> range;

        size_t binary_string_length = binary_string.length();

        for(size_t index = 0; index < binary_string_length + 1; index++)
        {
            range.push_back(index);
        }

        size_t parity_bit_positions_size = parity_bit_positions.size();

        for (size_t index = 0; index < parity_bit_positions_size; index++)
        {
            unsigned mask = 1 << index;

            auto filter_func = [&](unsigned d)
            {
                if (inclusive == 1)
                {
                    return (mask & d) != 0;
                }
                else
                {
                    return (mask != d) && ((mask & d) != 0);
                }
            };

            std::vector<unsigned> redundant_bits_pos(range.size());
            auto it = std::copy_if(range.begin(), range.end(), redundant_bits_pos.begin(), filter_func);
            redundant_bits_pos.resize(std::distance(redundant_bits_pos.begin(), it));

            std::vector<unsigned> data_sel;
            auto binary_string_array = utils::split(binary_string);
            for (const auto& pos : redundant_bits_pos)
            {
                data_sel.push_back(binary_string_array[pos - 1] == '1' ? 1 : 0);
            }

            auto fxor = [&](int x, int y) {return x ^ y;};
            auto xor_result = std::accumulate(data_sel.begin(), data_sel.end(), 0, fxor);
            if(xor_result == 1)
            {
                parity_bits[index] = 1;
            }
        }

        return parity_bits;
    }

    std::vector<std::string> create_seed_string_array(const std::vector<uint64_t>& skip_positions, const unsigned& length, const std::string& word_to_split)
    {
        std::vector<std::string> seed_string_array{length, "x"};

        unsigned data_idx = 0;

        for(unsigned index = 0; index < length; index++)
        {
            if(std::find(skip_positions.begin(), skip_positions.end(), index) == skip_positions.end())
            {
                seed_string_array[index] = word_to_split[data_idx++];
            }
        }

        return seed_string_array;
    }

    std::string encode(const uint64_t& data, const uint32_t& n_bits, const ParityLocation& parity_loc = ParityLocation::DEFAULT)
    {
        std::string binary_string = utils::int2bin(data, n_bits);

        std::string binary_string_reversed = utils::reverse_string(binary_string);

        uint32_t parity_bits_amount = n_parity_bits_required(n_bits);
        std::vector<uint64_t> parity_bit_positions = compute_parity_bit_positions(parity_bits_amount);

        unsigned encoded_message_length = parity_bits_amount + n_bits;

        std::vector<std::string> seed_string_array = create_seed_string_array(parity_bit_positions, encoded_message_length, binary_string_reversed);
        std::string seed_string = utils::join(seed_string_array);

        ASSERT(seed_string_array.size() == encoded_message_length)
        if(seed_string_array.size() != encoded_message_length) 
        {
            std::stringstream e;
            e << "Length of seed string (=" << seed_string_array.size() << ") does not match the expected (=" << encoded_message_length << ")";
            throw std::runtime_error(e.str());
        }

        std::vector<uint32_t> parity_bits = compute_parity_bits(seed_string, parity_bit_positions, false);

        ASSERT(parity_bits.size() == parity_bits_amount)
        if(parity_bits.size() != parity_bits_amount) 
        {
            std::stringstream e;
            e << "Number of computed parity bits (=" << parity_bits.size() << ") does not match the expected number (=" << parity_bits_amount << ")";
            throw std::runtime_error(e.str());
        }

        std::vector<std::string> parity_bit_chars{parity_bits.size()};
        std::transform(parity_bits.begin(), parity_bits.end(), parity_bit_chars.begin(), [](int x){ return std::to_string(x); });

        std::string encoded_output_string{""};

        switch(parity_loc)
        {
            case ParityLocation::DEFAULT:
            {
                utils::replace_with(seed_string_array, parity_bit_chars, "x");
                encoded_output_string = utils::reverse_string(utils::join(seed_string_array));

                break;
            }

            case ParityLocation::MSB:
            {
                seed_string_array.erase(std::remove(seed_string_array.begin(), seed_string_array.end(), "x"), seed_string_array.end());
                for(const auto& p : parity_bit_chars)
                {
                    seed_string_array.push_back(p);
                }

                encoded_output_string = utils::reverse_string(utils::join(seed_string_array));

                break;
            }

            case ParityLocation::LSB:
            {
                seed_string_array.erase(std::remove(seed_string_array.begin(), seed_string_array.end(), "x"), seed_string_array.end());
                for(const auto& p : parity_bit_chars)
                {
                    encoded_output_string += p;
                }

                encoded_output_string += utils::join(seed_string_array);
                encoded_output_string = utils::reverse_string(encoded_output_string);

                break;
            }
        }

        return encoded_output_string;
    }

    std::string fill_parity_bits(const std::string& message_without_parity, const std::string& parity_bit_block_str, const std::vector<uint64_t>& parity_bit_positions)
    {
        std::string message_with_parity{""};

        ASSERT(parity_bit_block_str.length() == parity_bit_positions.size())
        if(parity_bit_block_str.length() != parity_bit_positions.size()) 
        {
            throw std::runtime_error("Parity bit block string (\"" + parity_bit_block_str + "\") size != parity bit positions vector (size = " + std::to_string(parity_bit_positions.size()) + ")");
        }

        std::string reversed_in = utils::reverse_string(message_without_parity);
        std::string reversed_parity = utils::reverse_string(parity_bit_block_str);

        unsigned message_length = message_without_parity.length() + parity_bit_positions.size();
        size_t data_idx = 0;
        size_t parity_idx = 0;

        for(size_t ipos = 0; ipos < message_length; ipos++) 
        {
            if(std::find(parity_bit_positions.begin(), parity_bit_positions.end(), ipos) == parity_bit_positions.end()) 
            {
                message_with_parity += reversed_in[data_idx++];
            } 
            else 
            {
                message_with_parity += reversed_parity[parity_idx++];
            }
        }

        return utils::reverse_string(message_with_parity);
    }

    std::string decode(const uint64_t& data, const uint32_t& n_bits, const ParityLocation& parity_loc = ParityLocation::DEFAULT, uint32_t n_parity_bits = 0) 
    {
        std::string binary_string = utils::int2bin(data, n_bits);

        if(n_parity_bits == 0 && parity_loc != ParityLocation::DEFAULT) 
        {
            throw std::logic_error("Cannot decode message: must specify number of parity bits for non-default encoding");
        } 
        else if(n_parity_bits > 0 && parity_loc == ParityLocation::DEFAULT) 
        {
            throw std::logic_error("Cannot decode message: cannot specify number of parity bits for default encoding");
        } 
        else if(n_parity_bits == 0) 
        {
            n_parity_bits = n_parity_bits_required(n_bits);
        }

        std::vector<uint64_t> parity_bit_positions = compute_parity_bit_positions(n_parity_bits);
                                        
        switch(parity_loc) 
        {
            case ParityLocation::MSB: 
            {
                std::string parity_bit_block = binary_string.substr(0, n_parity_bits);
                std::string data_without_parity = binary_string.substr(n_parity_bits, binary_string.length());
                binary_string = fill_parity_bits(data_without_parity, parity_bit_block, parity_bit_positions);
                break;
                                    
            
            }
            case ParityLocation::LSB: 
            {
                std::string parity_bit_block = binary_string.substr(binary_string.length() - n_parity_bits, binary_string.length());
                std::string data_without_parity = binary_string.substr(0, binary_string.length() - n_parity_bits);
                binary_string = fill_parity_bits(data_without_parity, parity_bit_block, parity_bit_positions);
                break;
            }
            case ParityLocation::DEFAULT: 
            {
                break;
            }
        }

        std::string binary_string_reversed = utils::reverse_string(binary_string);

        std::vector<uint32_t> parity_bits = compute_parity_bits(binary_string_reversed, parity_bit_positions, true);

        auto error_string = utils::join(parity_bits);
        std::reverse(error_string.begin(), error_string.end());
        unsigned error_position = std::stoul(error_string, nullptr, 2);

        std::string decoded_string = binary_string_reversed;

        if(error_position > 0) 
        {
            decoded_string[error_position - 1] = (decoded_string[error_position-1] == '0') ? '1' : '0';
        }

        auto decoded = utils::remove_chars_at(decoded_string, parity_bit_positions);

        std::reverse(decoded.begin(), decoded.end());

        return decoded;
    }
}

#endif /* HAMMING_CODE_H */