#ifndef __OUTPUT_STREAM_HPP
#define __OUTPUT_STREAM_HPP

#include <vector>
#include <cstdint>
namespace R
{

    class OutputStreamBase
    {
        public:
        OutputStreamBase();
        virtual void fill_buffer(uint8_t *begin, uint8_t *end) = 0;
        virtual void flush_to_stream(std::ostream &stream) = 0;
        virtual ~OutputStreamBase();
    };

    template<typename T>
    class OutputStream: public OutputStreamBase
    {
        public:
        OutputStream(bool endianess, int dimension);

        std::vector<T>& get_buffer();
        virtual void fill_buffer(uint8_t *begin, uint8_t *end);
        void flush_to_stream(std::ostream &stream);
        private:
        std::vector<T> m_buffer;
        bool m_endianess;
        int m_dimension;
    };



}

#endif