#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <vector>
#include "errors.h"

namespace utils
{
    std::string int2bin(const uint64_t& data, const uint32_t& n_bits)
    {
        std::string binary_string = "";
        uint64_t mask = 1;

        for(uint32_t index = 0; index < n_bits; index++)
        {
            if((mask & data) >= 1)
            {
                binary_string = "1" + binary_string;
            }
            else
            {
                binary_string = "0" + binary_string;
            }

            mask = mask << 1;
        }

        return binary_string;
    }

    template <class T>
    std::string join(const std::vector<T>& vec)
    {
        std::stringstream sx;

        for(const auto& x : vec)
        {
            sx << x;
        }

        return sx.str();
    }
    template std::string join<std::string>(const std::vector<std::string>&);
    template std::string join<int>(const std::vector<int>&);

    std::string reverse_string(const std::string& input)
    {
        std::string out = input;
        std::reverse_copy(input.begin(), input.end(), out.begin());

        return out;
    }

    std::vector<std::string> replace_with(std::vector<std::string>& seed, const std::vector<std::string>& fill, const std::string& flag = "x") // fix
    {
        ASSERT(flag.length() == 1)

        std::vector<std::string> reversed_fill{fill.size()};
        std::reverse_copy(fill.begin(), fill.end(), reversed_fill.begin());
        
        auto fp = [&reversed_fill, flag](std::string ch)
        {
            if(ch == flag)
            {
                auto new_value = reversed_fill.back();
                reversed_fill.pop_back();

                return new_value;
            }
            else
            {
                return ch;
            }
        };

        std::transform(seed.begin(), seed.end(), seed.begin(), fp);

        return seed;
    }

    std::string remove_chars_at(const std::string& init, const std::vector<uint64_t>& positions)
    {
        std::stringstream output; //fix

        for(size_t index = 0; index < init.size(); index++)
        {
            if(std::find(positions.begin(), positions.end(), index) == positions.end())
            {
                output << init[index];
            }
        }

        return output.str();
    }

    std::vector<char> split(const std::string& s)
    {
        std::vector<char> out(s.begin(), s.end());

        return out;
    }
}

#endif /* UTILS_H_ */