#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP
#include <iostream>
#include <cstdint>
#include <vector>

namespace R
{
    template<typename T>
    class RbufferManager; // forward declaration
    template <typename T>
    class Statistics
    {
        public:
            Statistics(RbufferManager<T>& rbufferManager, uint8_t dimension);
            
            void react_stream();
            void react_buffer();

            static int64_t AlgorithmR(uint64_t t, size_t buffer_size);
        private:

            RbufferManager<T>& m_RbufferManager;
            uint8_t m_dimension;
            std::vector<double> m_median_off_cur;
            std::vector<double> m_median_off_pre;
            std::vector<double> m_median_on_cur;
            std::vector<double> m_median_on_pre;
            double m_pvariance_off_cur;
            double m_pvariance_off_pre;
            double m_pvariance_on_cur;
            double m_pvariance_on_pre;

            void recalculate_stream_median(std::vector<T> new_value, double n);
            void recalculate_stream_pvariance(std::vector<T> new_value, double n);
            void recalculate_buffer_median(std::vector<T> old_value, std::vector<T> new_value, double n);
            void recalculate_buffer_pvariance(std::vector<T> new_value, double n);


    }; 


} // R
#include "RbufferManager.hpp"
#endif
