#include <iostream>
#include <string>

namespace core {
    void printver(int type) {
        /**
         * @brief You MUST use this function when you use Mikicrep Framework, it gives us credits for framwork and prints version for easier support
         * @param ver MF Version
         * @param type Type of version... Check below comments
         */
        /*
        Type types
        1 - Standard release
        2 - Modified release - Use this if you modify MF code to your needs
        3 - DEV version - This is used to tell that you are using version from beta channel
        */
        std::string ver = "1.7.0";

        std::cout << "-------Mikicrep Framework-------" << std::endl;

        if(type == 1)
            std::cout << "-------Ver: " << ver << "---------------" << std::endl;
        else if(type == 2)
            std::cout << "-------Ver: " << ver << "M--------------" << std::endl;
        else if(type == 3)
              std::cout << "-------Ver: D E V---------------" << std::endl;

        std::cout << "Copyright Mikicrep Studios 2024-2025" << std::endl;
    }
}