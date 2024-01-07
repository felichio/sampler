#include "Statistics.hpp"



template <typename T>
R::Statistics<T>::Statistics(RbufferManager<T>& rbufferManager, uint8_t dimension)
    : m_RbufferManager{rbufferManager}
    , m_dimension{dimension}
    , m_median_off_cur(m_dimension, 0.0)
    , m_median_off_pre(m_dimension, 0.0)
    , m_median_on_cur(m_dimension, 0.0)
    , m_median_on_pre(m_dimension, 0.0)
    , m_pvariance_off_cur{0.0}
    , m_pvariance_off_pre{0.0}
    , m_pvariance_on_cur{0.0}
    , m_pvariance_on_pre{0.0}
    
{

}

// This method is triggered every time a value (dimension is considered) is inserted in stream
template <typename T>
void R::Statistics<T>::react_stream()
{
    std::cout << "stream reacted" << std::endl;
    double n = m_RbufferManager.get_stream_size();
    std::vector<T> &stream = m_RbufferManager.get_stream();
    std::vector<T> new_value(stream.end() - m_dimension, stream.end());
    recalculate_stream_median(new_value, n);
    recalculate_stream_pvariance(new_value, n);
}

template <typename T>
void R::Statistics<T>::recalculate_stream_median(std::vector<T> new_value, double n)
{
    std::cout << "calculating new median" << std::endl;
    std::cout << "n: " << n << std::endl;
    // calculate median for each dimension
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        std::cout << std::endl;
        std::cout << "dimension: " << static_cast<int>(dimension) << std::endl;
        m_median_on_cur[dimension] = ((n - 1) / n) * m_median_on_pre[dimension] + (1 / n) * new_value[dimension];
        std::cout << "stream last:------------ " << new_value[dimension] << std::endl;
        std::cout << "current median:---------   " << m_median_on_cur[dimension] << std::endl;
        std::cout << "previous median:---------   " << m_median_on_pre[dimension]<< std::endl;
        m_median_on_pre[dimension] = m_median_on_cur[dimension];
        std::cout << std::endl;
    }
}

template <typename T>
void R::Statistics<T>::recalculate_stream_pvariance(std::vector<T> new_value, double n)
{

}
// --------------------------------------------------------------- // 

template<typename T>
void R::Statistics<T>::react_buffer()
{
    std::cout << "buffer reacted" << std::endl;
    double n = m_RbufferManager.get_stream_size();
    std::vector<T> &buffer = m_RbufferManager.get_buffer();
    std::pair<typename std::vector<T>::iterator, std::vector<T>> &rejected = m_RbufferManager.get_rejected_buffer_data();
    std::vector<T> new_value;
    if (rejected.first == buffer.end())
    {
        new_value.insert(new_value.begin(), buffer.end() - m_dimension, buffer.end());
    }
    else
    {
        new_value.insert(new_value.begin(), rejected.first, rejected.first + m_dimension);

    }

    
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        std::cout << "dimension: " << static_cast<int>(dimension) << std::endl;
        std::cout << "buffer last:---------- " << new_value[dimension] << std::endl;
    }
}

template<typename T>
void R::Statistics<T>::recalculate_buffer_median(std::vector<T> new_value, double n)
{

}

template<typename T>
int64_t R::Statistics<T>::AlgorithmR(uint64_t t, size_t buffer_size)
{
    int64_t offset = -1;
    if (t <= buffer_size)
    {
        offset = t - 1;
    }
    else
    {
        uint64_t x = 0;
        do
        {
            x = std::rand();
        } while (x >= (RAND_MAX - RAND_MAX % t));
        x %= t;

        if (x < buffer_size)
        {
            offset = x;
        }
    }
    std::cout << "AlgorithmR offset: " << offset << std::endl;
    return offset;
}

template class R::Statistics<int64_t>;
template class R::Statistics<int32_t>;
