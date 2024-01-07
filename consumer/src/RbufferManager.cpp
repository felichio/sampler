#include "RbufferManager.hpp"
#include <iostream>
#include <algorithm>



R::RbufferManagerBase::RbufferManagerBase(bool endianess, uint8_t dimension, size_t type_size, size_t RBUFFER_SIZE)
    : m_endianess{endianess}
    , m_dimension{dimension}
    , m_rbuffer_size{RBUFFER_SIZE}
    , m_type_size{type_size}
    , m_bytes_seen{}
{
    // reserve for at least reservoir buffer size
    m_agnostic_stream.reserve(m_type_size * dimension * m_rbuffer_size);
    m_agnostic_buffer.reserve(m_type_size * dimension * m_rbuffer_size);
    std::cout << "RbufferBase created" << std::endl; 
    std::cout << "sizes |  " <<  m_type_size * dimension * m_rbuffer_size << std::endl; 
}

R::RbufferManagerBase::~RbufferManagerBase(){}


void R::RbufferManagerBase::push_back_to_stream(const uint8_t value)
{
    m_agnostic_stream.push_back(value);
    m_bytes_seen++;
    std::cout << "byte no. " << m_bytes_seen << std::hex << " 0x" << static_cast<uint32_t>(value) << std::dec << std::endl;
    
    if (m_bytes_seen % m_type_size == 0)
    {
        uint8_t *begin;
        uint8_t *end;
        if (m_endianess)
        {
            begin = &*m_agnostic_stream.rbegin();
            end = begin - m_type_size;
        }
        else
        {
            end = &*m_agnostic_stream.end();
            begin = end - m_type_size;
        }
        std::cout << static_cast<void*>(begin) << std::endl;
        push_back_concrete_value(begin, end);
    }

}



template<typename T>
R::RbufferManager<T>::RbufferManager(bool end, uint8_t dimension, size_t RBUFFER_SIZE)
    : RbufferManagerBase(end, dimension, sizeof (T), RBUFFER_SIZE)
    , m_Statistics{*this, dimension}
    , m_local_stream_index{0}
    , m_stream_index{0}
{
    // reserve for at least reservoir buffer size
    m_stream.reserve(RBUFFER_SIZE * dimension);
    m_buffer.reserve(RBUFFER_SIZE * dimension);
    std::cout << "Rbuffer created" << std::endl; 
}

template<typename T>
void R::RbufferManager<T>::push_back_concrete_value(uint8_t *begin, uint8_t *end)
{
    T local{};
    for (uint8_t index = 0, *it = begin; it != end; ++index)
    {
        local |= static_cast<T>(*it) << index * 8;
        m_endianess ? --it : ++it;
    }
    std::cout << "pushing to local stream buffer, value: " << local << std::endl;
    m_local_stream_buffer.push_back(local);
     
    // m_stream_index holds the size of seen stream values (dimension is NOT considered) 

    // check if vector (whole) value is ready
    if (++m_local_stream_index % m_dimension == 0)
    {
        push_back_to_stream();
        m_local_stream_buffer.clear();
        m_local_stream_index = 0;
    }
}

template <typename T>
uint64_t R::RbufferManager<T>::get_stream_size()
{
    return m_stream_index;
}

template <typename T>
std::vector<T>& R::RbufferManager<T>::get_stream()
{
    return m_stream;
}

template <typename T>
std::vector<T>& R::RbufferManager<T>::get_buffer()
{
    return m_buffer;
}

template <typename T>
std::pair<typename std::vector<T>::iterator, std::vector<T>>& R::RbufferManager<T>::get_rejected_buffer_data()
{
    return m_rejected_buffer_data;
}

template<typename T>
void R::RbufferManager<T>::push_back_to_stream()
{
    std::cout << "pushing dimensional value to stream" << std::endl;
    m_stream.insert(m_stream.end(), m_local_stream_buffer.begin(), m_local_stream_buffer.end());
    m_stream_index++;
    // feed to Statistics
    m_Statistics.react_stream();
    // check Algorithm R, X etc...
    // feed to reservoir
    
    int64_t offset = Statistics<T>::AlgorithmR(m_stream_index, m_rbuffer_size);
    if (offset > -1)
    {
        std::cout << "pushing back to reservoir with offset " << offset * m_dimension << std::endl;
        push_back_to_reservoir(m_buffer.begin() + offset * m_dimension);
    }
}

template<typename T>
void R::RbufferManager<T>::push_back_to_reservoir(const typename std::vector<T>::iterator &location)
{
    if (location != m_buffer.end()) // swap
    {
        std::cout << "Location not end()" << std::endl;
        m_rejected_buffer_data.first = location;
        m_rejected_buffer_data.second.clear();
        m_rejected_buffer_data.second.insert(m_rejected_buffer_data.second.begin(), location, location + m_dimension);
        std::cout << "rejected data: " << m_rejected_buffer_data.second[0] << std::endl;
        std::copy(m_stream.end() - m_dimension, m_stream.end(), location);
    }
    else // new addition
    {
        std::cout << "new addition " << std::endl;
        m_rejected_buffer_data.first = location + m_dimension;
        m_buffer.insert(location, m_stream.end() - m_dimension, m_stream.end());

    }

    std::cout << " //----------------------\\" << std::endl;
    for (int i = 0; i < m_buffer.size(); i++)
    {
        std::cout << "m_buffer[" << i << "]: " << m_buffer[i] << std::endl;
    }
    std::cout << " //----------------------\\" << std::endl;
    m_Statistics.react_buffer();
}


// explicit template instantiation for two cases
template class R::RbufferManager<int64_t>;
template class R::RbufferManager<int32_t>;
