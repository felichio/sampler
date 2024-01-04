#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP
#include <iostream>
#include <cstdint>


namespace R
{
    class Statistics
    {
        public:
            Statistics(uint8_t dimension);
            

        private:
            uint8_t m_dimension;
    }; 


} // R

#endif
