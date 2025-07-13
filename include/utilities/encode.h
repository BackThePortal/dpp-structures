//
// Created by backo on 03/07/25.
//

#ifndef DPP_STRUCTURES_ENCODE_H
#define DPP_STRUCTURES_ENCODE_H

#include <string>
#include <vector>
#include <ios>
#include <iomanip>

// source: https://gist.github.com/darelf/0f96e1d313e1d0da5051e1a6eff8d329
/*
Base64 translates 24 bits into 4 ASCII characters at a time. First,
3 8-bit bytes are treated as 4 6-bit groups. Those 4 groups are
translated into ASCII characters. That is, each 6-bit number is treated
as an index into the ASCII character array.
If the final set of bits is less 8 or 16 instead of 24, traditional base64
would add a padding character. However, if the length of the data is
known, then padding can be eliminated.
One difference between the "standard" Base64 is two characters are different.
See RFC 4648 for details.
This is how we end up with the Base64 URL encoding.
*/

namespace dpp_structures {

    constexpr char base64_url_alphabet[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
    };

    constexpr char base64_alphabet[64] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    template<const char Alphabet[64], typename T>
    std::string base64_encode(const T& in) {
        std::string out;
        int val = 0, valb = -6;
        size_t len = in.size();
        unsigned int i = 0;
        for (i = 0; i < len; i++) {
            unsigned char c = in[i];
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(Alphabet[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) {
            out.push_back(Alphabet[((val << 8) >> (valb + 8)) & 0x3F]);
        }
        return out;
    }

    template<char Alphabet[64]>
    std::string base64_decode(const std::vector<unsigned char>& in) {
        std::string out;
        std::vector<int> T(256, -1);
        int i;
        for (i = 0; i < 64; i++) T[Alphabet[i]] = i;

        int val = 0, valb = -8;
        for (i = 0; i < in.size(); i++) {
            unsigned char c = in[i];
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    // source: https://stackoverflow.com/a/17708801
    std::string url_encode(const std::string &value);
}
#endif //DPP_STRUCTURES_ENCODE_H
