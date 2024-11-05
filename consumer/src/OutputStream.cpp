#include <cstdint>
#include "OutputStream.hpp"

template<typename T>
R::OutputStream<T>::OutputStream(bool endianess, int dimension):
    m_buffer{},
    m_endianess{},
    m_dimension{}
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

}

template<typename T>
std::vector<T>& R::OutputStream<T>::get_buffer()
{
    return m_buffer;
}


template class R::OutputStream<int64_t>;
template class R::OutputStream<int32_t>;