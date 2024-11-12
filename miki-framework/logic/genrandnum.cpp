#include <random>

namespace logic { 
    int GenRanNum(int min, int max) {
        /**
         * @brief Returns random number
         * @param min Minimum number (includes)
         * @param max Maximum number (includes)
         */
        
        // Seed
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Set range
        std::uniform_int_distribution<int> distr(min, max);
        
        // Gen
        return distr(gen);
    }
}
