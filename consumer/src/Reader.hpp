#include <vector>
#include <cstdint>
#include <string>
#include <fstream>


namespace R
{

    static const uint32_t buffer_size = 8192;

    enum TYPE: uint8_t
    {
        INT64 = 0x00,
        INT32 = 0x01
    };

    class Reader
    {
        public:
        Reader(int socketfd, bool endianess, uint8_t dimension, size_t type_size);
        void read_output_stream();
        void flush_buffer(uint8_t *beg, uint8_t *end);
        void get_com_attributes();

        private:
        int m_socketfd;
        uint8_t m_buffer[buffer_size];
        bool m_endianess; 
        uint8_t m_dimension;
        size_t m_type_size;
        uint64_t m_bytes_seen;
        bool m_buffer_pending;
        std::vector<uint8_t> m_agnostic_stream;
    };


}