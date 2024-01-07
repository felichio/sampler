#include "RbufferManager.hpp"
#include <iostream>



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
    , m_Statistics{*this}
    , m_stream_index{}
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
    std::cout << "pushing to rstream value: " << local << std::endl;
    m_stream.push_back(local);
     
    // flush reservoir
    

    // check if vector (whole) value is ready
    if (++m_stream_index % m_dimension == 0)
    {
        // feed to Statistics
        m_Statistics.react_stream();
        // check Algorithm R, X etc...
        // feed to reservoir
    }


}

template <typename T>
size_t R::RbufferManager<T>::get_stream_size()
{
    return m_stream.size();
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

template<typename T>
void R::RbufferManager<T>::push_back_to_reservoir(T value)
{
    m_buffer.push_back(value);
}


// explicit template instantiation for two cases
template class R::RbufferManager<int64_t>;
template class R::RbufferManager<int32_t>;
