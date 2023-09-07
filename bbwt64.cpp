//https://godbolt.org/z/a5nfrvhM1
#include <vector>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <ctime>
#include <stdexcept>



inline __attribute__((always_inline)) __attribute__((const)) uint16_t av_bswap16(uint16_t x)
{
    x= (x>>8) | (x<<8);
    return x;
}

inline __attribute__((always_inline)) __attribute__((const)) uint32_t av_bswap32(uint32_t x)
{
    return ((((x) << 8 & 0xff00) | ((x) >> 8 & 0x00ff)) << 16 | ((((x) >> 16) << 8 & 0xff00) | (((x) >> 16) >> 8 & 0x00ff)));
}

inline uint64_t __attribute__((const)) av_bswap64(uint64_t x)
{
    return (uint64_t)av_bswap32(x) << 32 | av_bswap32(x >> 32);
}

inline uint64_t ror1_ip(uint64_t &x)
{
   return (x >> 1) | (x << 63);
}

void print_base2( uint64_t value ) {
    uint64_t mask = 1ULL << 63;
    for (  int i=0; i<64; i++ ) {
        std::cout << (!!(value & mask) ? "1" : "0");
        if ( (i&7) == 7)
            std::cout << "-";
        mask >>= 1;
    }
    
}



std::pair<uint64_t, uint8_t> bwt_encode(uint64_t value) {

    std::vector<uint64_t> v64(64);
    auto val = value;
    for (auto &v : v64) {
        v = val;
        val = ror1_ip(val);
    }        
    std::stable_sort(v64.begin(), v64.end());

    uint64_t res = 0; 
    int index = 0;
    uint8_t res_index = 255;
    for (auto v : v64) {
        if (v == value) {
            res_index = index;
        }
        res = res + res + (v&1);
        index++;
    }        

    return std::make_pair(res, res_index);
}



bool comp_tuples(const std::pair<bool, int>& a, const std::pair<bool, int> &b) {
	return (a.first < b.first);
}

uint64_t bwt_decode(uint64_t value, int j) {

    std::vector<std::pair<bool,int>> v64;


        auto mask = 1ULL << 63;
        for (int i = 0; i < 64; ++i) {
            v64.emplace_back( value & mask, i );
            mask >>= 1;
        }

    std::stable_sort(v64.begin(), v64.end(), comp_tuples);
        
    

    uint64_t res = 0;
    for (int i=0; i < 64; i++ ) {
        res = res + res + v64.at(j).first;
        j = v64.at(j).second;
    }
    return res;

}

    inline uint32_t rand_xor() {
        static uint32_t seed = 0x55555555;
        seed ^= (seed << 13);
        seed ^= (seed >> 17);
        seed ^= (seed << 5);
        return seed;
        }

    inline  uint64_t rand_xor64() {
       return  static_cast<uint64_t>(rand_xor()) << 32 | rand_xor();
    }


int main() {
    for (int i = 0; i < 100; ++i) {
        const uint64_t value_orig = rand_xor64();
        // print_base2(value_orig); 
        // std::cout << std::endl;
        auto [value, index] = bwt_encode(value_orig);
        print_base2(value);  std::cout << "+" << (int)index;
        std::cout << std::endl;
        value = bwt_decode(value, index);
        if (value != value_orig) {
            throw std::runtime_error("test failed!!!");
        }
        // print_base2(value);
        // std::cout << std::endl;
    }
    std::cout << "test passed!" << std::endl;

}
