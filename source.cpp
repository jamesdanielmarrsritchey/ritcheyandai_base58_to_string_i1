/*
To compile and install this program on Debian, you can use the g++ compiler and make utility. Here are the steps:

1. Install g++ if it's not already installed:
   sudo apt-get install g++

2. Compile the program:
   g++ -o base58 source.cpp

3. The executable 'base58' will be created in the current directory. You can run it with:
   ./base58 --base58_string "your base58 string here"

Note: This program uses the getopt library, which is a part of the GNU C Library (glibc). It should be available on any standard Linux distribution.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>
#include <iostream>

std::string base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

std::string from_base58(std::string input) {
    std::vector<unsigned char> b58(input.begin(), input.end());
    std::string result;
    unsigned int carry;
    int size = b58.size() * 733 / 1000 + 1;
    std::vector<unsigned char> bin(size);
    for (auto& big : b58) {
        carry = base58_chars.find(big);
        for (auto it = bin.rbegin(); it != bin.rend(); it++) {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }
    }
    auto it = std::find_if(bin.begin(), bin.end(), [](unsigned char x) { return x != 0; });
    for (; it != bin.end(); it++) {
        result += *it;
    }
    return result;
}

int main(int argc, char *argv[]) {
    int c;
    std::string input;
    while (1) {
        static struct option long_options[] = {
            {"base58_string", required_argument, 0, 's'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        c = getopt_long(argc, argv, "s:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
            case 's':
                input = optarg;
                break;
            default:
                return 1;
        }
    }
    std::cout << from_base58(input);
    return 0;
}