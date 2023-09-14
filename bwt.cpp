#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename Iterator>
inline size_t bwt_encode(Iterator begin, Iterator end) {
    using T = typename std::iterator_traits<Iterator>::value_type;

    std::vector<std::vector<T>> ror( std::distance(begin,end));

    auto vec = std::vector<T>(begin,end);
    ror[0] = vec;
    for (size_t i=1; i< ror.size();++i ) {
        std::rotate(vec.rbegin(), vec.rbegin() + 1, vec.rend());
        ror[i] = vec;
    }


    // for (auto & vec : ror) {
    // // Выводим на экран измененный вектор
    // for (const auto& elem : vec) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;
    // }

    // std::cout << "-----------" << std::endl;
    auto pointer0 = &ror[0][0];
    std::stable_sort(ror.begin(), ror.end());

    // for (auto & vec : ror) {
    // // Выводим на экран измененный вектор
    // for (const auto& elem : vec) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;
    // }
    // std::cout << "-----------" << std::endl;

    //auto res_index = std::distance(ror.begin(), it1);
    size_t res = 0;
    for (size_t i=0; i< ror.size();++i ) {
       if (&ror[i][0] == pointer0)
          res = i;
       *begin++ = ror[i].back();    
    }
    return res;
}


template <typename T>
bool comp_tuples(const std::pair<T,int>& a, const std::pair<T,int>&b) {
	return (a.first < b.first);
}

template <typename Iterator>
inline void bwt_decode(Iterator begin, Iterator end, size_t index) {
    using T = typename std::iterator_traits<Iterator>::value_type;

    std::vector<std::pair<T,int>> vec;
    
    for (auto it = begin; it != end; ++it) {
        vec.emplace_back( *it, std::distance(begin,it) );
    }

    std::stable_sort(vec.begin(), vec.end(), comp_tuples<T>);
    
    for (auto it = begin; it != end; ++it ) {
        const auto & pair = vec.at(index);
        *it = pair.first;
        index = pair.second;
    }
}

template <typename Iterator, typename T>
inline void bwt_decode(Iterator begin, Iterator end, T terminator) {

    std::vector<std::pair<T,int>> vec;
    size_t index = -1;
    
    for (auto it = begin; it != end; ++it) {
        const auto symbol = *it;
        const auto index_ = std::distance(begin,it);
        vec.emplace_back( symbol, index_ );
        if (symbol == terminator) {
            if (index != -1)
                throw std::logic_error("multuple terminators in stream!!!");
            index = index_;
        }
    }

    std::stable_sort(vec.begin(), vec.end(), comp_tuples<T>);
    
    for (auto it = begin; it != end; ++it ) {
        const auto & pair = vec.at(index);
        *it = pair.first;
        index = pair.second;
    }

}




int main() {
    //std::vector<int> vec {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3};
    std::string vec{ "^BANANA|" };
    
    
    auto idx = bwt_encode(vec.begin(), vec.end());
    std::cout << "index = " << idx << std::endl;
    
    // Выводим на экран измененный вектор
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }

    std::cout << std::endl << "--------" << std::endl;

    bwt_decode(vec.begin(), vec.end(), idx);

    // Выводим на экран измененный вектор
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    idx = bwt_encode(vec.begin(), vec.end());
    std::cout << "index = " << idx << std::endl;
    // Выводим на экран измененный вектор
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }


    std::cout << std::endl << "--------" << std::endl;

    bwt_decode(vec.begin(), vec.end(), '|');

    // Выводим на экран измененный вектор
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }

    
    return 0;
}
