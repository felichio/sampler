#ifndef _R_BUFFER_HPP

#define _R_BUFFER_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace R
{
    class RbufferBase
    {
        public:
            RbufferBase(bool endianess /* true for BE */ , uint8_t dimension, size_t type_size, size_t RBUFFER_SIZE);
            virtual ~RbufferBase();
            void push_back_to_stream(const uint8_t value);
        protected:
            bool m_endianess;
            uint8_t m_dimension;
            size_t m_rbuffer_size;
            size_t m_type_size;
            std::vector<uint8_t> m_agnostic_stream;
            std::vector<uint8_t> m_agnostic_buffer;
             
            virtual void push_back_concrete_value(uint8_t *begin, uint8_t *end) = 0;
            static uint64_t bytes_seen;
    };


    template<typename T>
    class Rbuffer : public RbufferBase
    {
        public:
            Rbuffer(bool end, uint8_t dimension, size_t RBUFFER_SIZE);
            
            virtual void push_back_concrete_value(uint8_t *begin, uint8_t *end);
            void push_back_to_reservoir(T value);
            static uint64_t stream_index;
        private:
            std::vector<T> m_stream;
            std::vector<T> m_buffer;
    };


} // R


#endif
