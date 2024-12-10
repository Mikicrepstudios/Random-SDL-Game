#include "mf/logic.h"

namespace logic {
    bool chancegen(int chance) {
        /**
         * @brief This function got chance to give true
         * @param chance What chance to return true (1 in {chance})
         */

        int number = GenRanNum(1, chance);

        if(number == 1) return true;
        else return false;
    }
}