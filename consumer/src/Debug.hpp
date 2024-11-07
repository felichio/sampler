#ifndef __DEBUG_HPP
#define __DEBUG_HPP


#if DEBUG == 1
#define DEBUG_CON 1
#else
#define DEBUG_CON 0
#endif



#define debug_print(str) \
    do {if (DEBUG_CON) std::cout << str; } while (0)


#endif