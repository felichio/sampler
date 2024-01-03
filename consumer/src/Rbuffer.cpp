#include "Rbuffer.hpp"
#include <iostream>



R::RbufferBase::RbufferBase(bool endianess, uint8_t dimension, size_t type_size, size_t RBUFFER_SIZE)
    : m_endianess{endianess}
    , m_dimension{dimension}
    , m_rbuffer_size{RBUFFER_SIZE}
    , m_type_size{type_size}
{
    // reserve for at least reservoir buffer size
    m_agnostic_stream.reserve(m_type_size * dimension * m_rbuffer_size);
    m_agnostic_buffer.reserve(m_type_size * dimension * m_rbuffer_size);
    std::cout << "RbufferBase created" << std::endl; 
    std::cout << "sizes |  " <<  m_type_size * dimension * m_rbuffer_size << std::endl; 
}

uint64_t R::RbufferBase::stream_index = 0;

void R::RbufferBase::push_back_to_stream(const uint8_t value)
{
    std::cout << "got : " << static_cast<int>(value) << std::endl;
    m_agnostic_stream.push_back(value);
    stream_index++;
    
    if (stream_index % m_type_size == 0)
    {
        uint8_t *begin;
        uint8_t *end;
        if (m_endianess)
        {
            begin = &(*m_agnostic_stream.rbegin());
            end = begin - m_type_size;
        }
        else
        {
            begin = &(*m_agnostic_stream.begin());
            end = begin + m_type_size;
        }
        push_back_concrete_value(begin, end);
    }

}



template<typename T>
R::Rbuffer<T>::Rbuffer(bool end, uint8_t dimension, size_t RBUFFER_SIZE)
    : RbufferBase(end, dimension, sizeof (T), RBUFFER_SIZE)
{
    // reserve for at least reservoir buffer size
    m_stream.reserve(RBUFFER_SIZE * dimension);
    m_buffer.reserve(RBUFFER_SIZE * dimension);
    std::cout << "Rbuffer created" << std::endl; 
}

template<typename T>
void R::Rbuffer<T>::push_back_concrete_value(uint8_t *begin, uint8_t *end)
{
    T local{};
    for (uint8_t index = 0, *it = begin; it != end; ++index)
    {
        local |= static_cast<T>(*it) << index * 8;
        m_endianess ? --it : ++it;
    }
    std::cout << "pushing to rstream value: " << local << std::endl;
    m_stream.push_back(local);
}


// explicit template instantiation for two cases
template class R::Rbuffer<int64_t>;
template class R::Rbuffer<int32_t>;
