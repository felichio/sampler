#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP
#include <iostream>
#include <cstdint>

namespace R
{
    template<typename T>
    class RbufferManager; // forward declaration
    template <typename T>
    class Statistics
    {
        public:
            Statistics(RbufferManager<T>& rbufferManager);
            
            void react_stream();

        private:

            RbufferManager<T>& m_RbufferManager;

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
#include "RbufferManager.hpp"
#endif
