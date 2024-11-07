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
            bool check_condition();
            void clear_state();

            static int64_t AlgorithmR(uint64_t t, size_t buffer_size);
        private:

            RbufferManager<T>& m_RbufferManager;
            uint8_t m_dimension;
            std::vector<double> m_mean_off_cur;
            std::vector<double> m_mean_off_pre;
            std::vector<double> m_mean_on_cur;
            std::vector<double> m_mean_on_pre;
            std::vector<double> m_pvariance_off_cur;
            std::vector<double> m_pvariance_off_pre;
            std::vector<double> m_pvariance_on_cur;
            std::vector<double> m_pvariance_on_pre;

            std::vector<double> m_coeffv_buffer;
            std::vector<double> m_coeffv_stream;

            void recalculate_stream_mean(std::vector<T> new_value, double n);
            void recalculate_stream_pvariance(std::vector<T> new_value, double n);
            void recalculate_buffer_mean(std::vector<T> old_value, std::vector<T> new_value, double n);
            void recalculate_buffer_pvariance(std::vector<T> old_value, std::vector<T> new_value, double n);

            void recalculate_buffer_coeffv();
            void recalculate_stream_coeffv();

            void print_vector(std::ostream& os, std::vector<T>& values);

    }; 


} // R
#include "RbufferManager.hpp"
#endif
