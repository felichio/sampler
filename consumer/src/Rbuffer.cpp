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

void R::RbufferBase::push_back_to_stream(const uint8_t value)
{
    std::cout << "got : " << static_cast<int>(value) << std::endl;
}



template<typename T>
R::Rbuffer<T>::Rbuffer(bool end, uint8_t dimension, size_t RBUFFER_SIZE)
    : RbufferBase(end, dimension, sizeof (T), RBUFFER_SIZE)
{
    // reserve for at least reservoir buffer size
    m_stream.reserve(RBUFFER_SIZE);
    m_buffer.reserve(RBUFFER_SIZE);
    std::cout << "Rbuffer created" << std::endl; 
}


// explicit template instantiation for two cases
template class R::Rbuffer<int64_t>;
template class R::Rbuffer<double>;
