#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP
#include <iostream>
#include <cstdint>

namespace R
{
    template<typename T>
    class Rbuffer; // forward declaration
    template <typename T>
    class Statistics
    {
        public:
            Statistics(Rbuffer<T>& rbuffer);
            
            void react_stream();

        private:

            Rbuffer<T>& m_rbuffer;

            double m_median_off_cur;
            double m_median_off_pre;
            double m_median_on_cur;
            double m_median_on_pre;
            double m_pvariance_off_cur;
            double m_pvariance_off_pre;
            double m_pvariance_on_cur;
            double m_pvariance_on_pre;

            void recalculate_median();
            void recalculate_pvariance();


    }; 


} // R
#include "Rbuffer.hpp"
#endif
