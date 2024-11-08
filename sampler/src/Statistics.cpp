#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "Debug.hpp"
#include "Statistics.hpp"



template <typename T>
R::Statistics<T>::Statistics(RbufferManager<T>& rbufferManager, uint8_t dimension)
    : m_RbufferManager{rbufferManager}
    , m_dimension{dimension}
    , m_mean_off_cur(m_dimension, 0.0)
    , m_mean_off_pre(m_dimension, 0.0)
    , m_mean_on_cur(m_dimension, 0.0)
    , m_mean_on_pre(m_dimension, 0.0)
    , m_pvariance_off_cur(m_dimension, 0.0)
    , m_pvariance_off_pre(m_dimension, 0.0)
    , m_pvariance_on_cur(m_dimension, 0.0)
    , m_pvariance_on_pre(m_dimension, 0.0)
    , m_coeffv_buffer(m_dimension, 0.0)
    , m_coeffv_stream(m_dimension, 0.0)
    
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
    std::ofstream &m_file = m_RbufferManager.get_file();
    debug_print("stream values: ");
    m_file << "stream values: ";
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        debug_print(new_value[dimension] << " ");
        m_file << new_value[dimension] << " ";
    }
    debug_print(std::endl);
    m_file << std::endl;

    /* --- Trigger Statistics change --- */
    recalculate_stream_mean(new_value, n);
    recalculate_stream_pvariance(new_value, n);
    recalculate_stream_coeffv();
}

template <typename T>
void R::Statistics<T>::recalculate_stream_mean(std::vector<T> new_value, double n)
{
    debug_print("calculating new mean" << std::endl);
    std::cout << "n: " << n << std::endl;
    // calculate mean for each dimension
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        std::cout << std::endl;
        // std::cout << "dimension: " << static_cast<int>(dimension) << std::endl;
        m_mean_on_pre[dimension] = m_mean_on_cur[dimension];
        m_mean_on_cur[dimension] = ((n - 1) / n) * m_mean_on_pre[dimension] + (1 / n) * new_value[dimension];
        // std::cout << "stream last:------------ " << new_value[dimension] << std::endl;
        std::cout << "current mean:---------   " << m_mean_on_cur[dimension] << std::endl;
        std::cout << "previous mean:---------   " << m_mean_on_pre[dimension]<< std::endl;
        m_file << "curr stream mean dimension " << static_cast<uint32_t>(dimension) << ": " << std::setprecision(3) << std::fixed << m_mean_on_cur[dimension] << std::defaultfloat << std::endl;
        m_file << "prev stream mean dimension " << static_cast<uint32_t>(dimension) << ": " << std::fixed << m_mean_on_pre[dimension] << std::defaultfloat<< std::endl;
        std::cout << std::endl;
    }
}

template <typename T>
void R::Statistics<T>::recalculate_stream_pvariance(std::vector<T> new_value, double n)
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        m_pvariance_on_pre[dimension] = m_pvariance_on_cur[dimension];
        m_pvariance_on_cur[dimension] = (n - 1) / n * m_pvariance_on_pre[dimension] + (n - 1) / pow(n, 2) * pow(new_value[dimension] - m_mean_on_pre[dimension], 2);
        std::cout << "current pvariance:---------   " << m_pvariance_on_cur[dimension] << std::endl;
        std::cout << "previous pvariance:---------   " << m_pvariance_on_pre[dimension]<< std::endl;
        m_file << "curr stream pvariance dimension " << static_cast<uint32_t>(dimension) << ": " << std::setprecision(3) << std::fixed << m_pvariance_on_cur[dimension] << std::defaultfloat << std::endl;
        m_file << "prev stream pvariance dimension " << static_cast<uint32_t>(dimension) << ": " << std::fixed << m_pvariance_on_pre[dimension] << std::defaultfloat<< std::endl;
    }
}

template <typename T>
void R::Statistics<T>::recalculate_stream_coeffv()
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        m_coeffv_stream[dimension] = std::sqrt(m_pvariance_on_cur[dimension]) / m_mean_on_cur[dimension];
        std::cout << "current coefficient_variation:---------   " << m_coeffv_stream[dimension] << std::endl;
        m_file << "current coefficient_variation dimension " << static_cast<uint32_t>(dimension) << ": " << std::setprecision(3) << std::fixed << m_coeffv_stream[dimension] << std::defaultfloat << std::endl;
    }
}


// --------------------------------------------------------------- // 

template<typename T>
void R::Statistics<T>::react_buffer()
{
    std::cout << "buffer reacted" << std::endl;
    std::vector<T> &buffer = m_RbufferManager.get_buffer();
    std::ofstream &m_file = m_RbufferManager.get_file();
    std::pair<typename std::vector<T>::iterator, std::vector<T>> &rejected = m_RbufferManager.get_rejected_buffer_data();
    std::vector<T> new_value;
    if (rejected.first == buffer.end())
    {
        std::cout << "new value" << std::endl;
        new_value.insert(new_value.begin(), buffer.end() - m_dimension, buffer.end());
        // take the mean of stream
        m_mean_off_cur = m_mean_on_cur;
        m_mean_off_pre = m_mean_off_cur;
        /* ------------ */

        // take the pvariance of stream
        m_pvariance_off_cur = m_pvariance_on_cur;
        m_pvariance_off_pre = m_pvariance_off_cur;
        /* ------------ */

        // take the coefficient of variation of stream
        m_coeffv_buffer = m_coeffv_stream;
        /* ------------ */

        m_file << "| buffer addition |" << std::endl;
        m_file << "values: ";
        print_vector(m_file, new_value);
        m_file << std::endl;
        m_RbufferManager.print_buffer(m_file);
    }
    else
    {
        std::cout << "swapped" << std::endl;
        new_value.insert(new_value.begin(), rejected.first, rejected.first + m_dimension);

        m_file << "| buffer swapping |" << std::endl;
        m_file << "new values: "; 
        print_vector(m_file, new_value);
        m_file << std::endl;
        m_file << "rejected values: ";
        print_vector(m_file, rejected.second);
        m_file << std::endl;
        m_RbufferManager.print_buffer(m_file);

        // Recalulate the buffer Statistics
        recalculate_buffer_mean(rejected.second, new_value, buffer.size() / m_dimension);
        recalculate_buffer_pvariance(rejected.second, new_value, buffer.size() / m_dimension);
        recalculate_buffer_coeffv();
    }

    std::cout << "buffer inserted: ";
    print_vector(std::cout, new_value);
    std::cout << std::endl;
}


template<typename T>
void R::Statistics<T>::recalculate_buffer_mean(std::vector<T> old_value, std::vector<T> new_value, double n)
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        std::cout << "Previous buffer mean: " << m_mean_off_pre[dimension] << std::endl;
        m_mean_off_pre[dimension] = m_mean_off_cur[dimension];
        m_mean_off_cur[dimension] = m_mean_off_pre[dimension] + (new_value[dimension] - old_value[dimension]) / n;
        std::cout << "old value: " << old_value[dimension] << std::endl;
        std::cout << "new value: " << new_value[dimension] << std::endl;
        std::cout << "Buffer mean on dimension: " << static_cast<uint32_t>(dimension) << "   " << m_mean_off_cur[dimension] << std::endl;
        m_file << "curr buffer mean dimension " << static_cast<uint32_t>(dimension) << ": " << std::fixed << m_mean_off_cur[dimension] << std::defaultfloat << std::endl;
        m_file << "prev buffer mean dimension " << static_cast<uint32_t>(dimension) << ": " << std::fixed << m_mean_off_pre[dimension] << std::defaultfloat << std::endl;
    }
}

template<typename T>
void R::Statistics<T>::recalculate_buffer_pvariance(std::vector<T> old_value, std::vector<T> new_value, double n)
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        m_pvariance_off_pre[dimension] = m_pvariance_off_cur[dimension];
        m_pvariance_off_cur[dimension] = m_pvariance_off_pre[dimension] + (pow(new_value[dimension], 2) - pow(old_value[dimension], 2)) / n + pow(m_mean_off_pre[dimension], 2) - pow(m_mean_off_cur[dimension], 2);
        m_file << "curr buffer pvariance dimension " << static_cast<uint32_t>(dimension) << ": " << std::setprecision(3) << std::fixed << m_pvariance_off_cur[dimension] << std::defaultfloat << std::endl;
        m_file << "prev buffer pvariance dimension " << static_cast<uint32_t>(dimension) << ": " << std::fixed << m_pvariance_off_pre[dimension] << std::defaultfloat<< std::endl;
    }
}


template<typename T>
void R::Statistics<T>::recalculate_buffer_coeffv()
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        m_coeffv_buffer[dimension] = std::sqrt(m_pvariance_off_cur[dimension]) / m_mean_off_cur[dimension];
        std::cout << "current buffer coefficient_variation:---------   " << m_coeffv_buffer[dimension] << std::endl;
        m_file << "current buffer coefficient_variation dimension " << static_cast<uint32_t>(dimension) << ": " << std::setprecision(3) << std::fixed << m_coeffv_buffer[dimension] << std::defaultfloat << std::endl;
    }
}


template<typename T>
void R::Statistics<T>::print_vector(std::ostream &os, std::vector<T> &values)
{
    for (uint8_t dimension = 0; dimension < m_dimension; dimension++)
    {
        os << values[dimension] << " ";
    }
}


template<typename T>
bool R::Statistics<T>::check_condition(double threshold)
{
    std::ofstream &m_file = m_RbufferManager.get_file();
    // check for dimension 0
    if (std::abs((m_coeffv_stream[0] - m_coeffv_buffer[0]) / m_coeffv_stream[0]) > threshold)
    {
        std::cout << "CV condition: " << std::abs((m_coeffv_stream[0] - m_coeffv_buffer[0]) / m_coeffv_stream[0]) << std::endl;
        m_file << "CV condition: " << std::abs((m_coeffv_stream[0] - m_coeffv_buffer[0]) / m_coeffv_stream[0]) << std::endl;
        return true;
    }
    return false;

}


template<typename T>
void R::Statistics<T>::clear_state()
{
    std::fill(m_mean_off_cur.begin(), m_mean_off_cur.end(), 0);
    std::fill(m_mean_off_pre.begin(), m_mean_off_pre.end(), 0);
    std::fill(m_mean_on_cur.begin(), m_mean_on_cur.end(), 0);
    std::fill(m_mean_on_pre.begin(), m_mean_on_pre.end(), 0);
    std::fill(m_pvariance_off_cur.begin(), m_pvariance_off_cur.end(), 0);
    std::fill(m_pvariance_off_pre.begin(), m_pvariance_off_pre.end(), 0);
    std::fill(m_pvariance_on_cur.begin(), m_pvariance_on_cur.end(), 0);
    std::fill(m_pvariance_on_pre.begin(), m_pvariance_on_pre.end(), 0);
    std::fill(m_coeffv_buffer.begin(), m_coeffv_buffer.end(), 0);
    std::fill(m_coeffv_stream.begin(), m_coeffv_stream.end(), 0);

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
        // std::srand(std::time(nullptr));
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
