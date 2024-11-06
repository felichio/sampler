#include <iostream>
#include "OutputStream.hpp"


R::OutputStreamBase::OutputStreamBase()
{

}
R::OutputStreamBase::~OutputStreamBase()
{

}

template<typename T>
R::OutputStream<T>::OutputStream(bool endianess, int dimension):
    OutputStreamBase(),
    m_buffer{},
    m_endianess{endianess},
    m_dimension{dimension}
{

}


template<typename T>
void R::OutputStream<T>::fill_buffer(uint8_t *begin, uint8_t *end)
{
    T local{};
    for (uint8_t index = 0, *it = begin; it != end; ++index)
    {
        local |= static_cast<T>(*it) << index * 8;
        m_endianess ? --it : ++it;
    }
    m_buffer.push_back(local);
    std::cout << local << std::endl;
}

template<typename T>
std::vector<T>& R::OutputStream<T>::get_buffer()
{
    return m_buffer;
}

template<typename T>
void R::OutputStream<T>::flush_to_stream(std::ostream &stream)
{
    uint32_t dsize = m_buffer.size() / m_dimension;
    for (uint32_t i = 0; i < dsize; i++)
    {
        stream << "buffer[" << i << "]: ";
        for (uint8_t j = 0; j < m_dimension; j++)
        {
            stream << m_buffer[i * m_dimension + j] << " ";
        }
        stream << std::endl;
    }
}





template class R::OutputStream<int64_t>;
template class R::OutputStream<int32_t>;