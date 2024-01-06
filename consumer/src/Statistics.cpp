#include "Statistics.hpp"



template <typename T>
R::Statistics<T>::Statistics(Rbuffer<T>& rbuffer)
    : m_rbuffer{rbuffer}
    , m_median_off_cur{0.0}
    , m_median_off_pre{0.0}
    , m_median_on_cur{0.0}
    , m_median_on_pre{0.0}
    , m_pvariance_off_cur{0.0}
    , m_pvariance_off_pre{0.0}
    , m_pvariance_on_cur{0.0}
    , m_pvariance_on_pre{0.0}
    
{

}

// This method is triggered every time a value is inserted in stream
template <typename T>
void R::Statistics<T>::react_stream()
{
    std::cout << "stream reacted" << std::endl;
    recalculate_median();
}

template <typename T>
void R::Statistics<T>::recalculate_median()
{
    std::cout << "calculating new median" << std::endl;
    double n = m_rbuffer.get_stream_size();
    std::vector<T> stream = m_rbuffer.get_stream();
    std::cout << "n: " << n << std::endl;
    m_median_on_cur = ((n - 1) / n) * m_median_on_pre + (1 / n) * stream.back();
    m_median_on_pre = m_median_on_cur;
    std::cout << "stream last:------------ " << stream.back() << std::endl;
    std::cout << "current median:---------   " << m_median_on_cur << std::endl;
    std::cout << "previous median:---------   " << m_median_on_pre << std::endl;
}

template <typename T>
void R::Statistics<T>::recalculate_pvariance()
{

}

template class R::Statistics<int64_t>;
template class R::Statistics<int32_t>;
