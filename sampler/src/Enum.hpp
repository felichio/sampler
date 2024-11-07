#ifndef __ENUM_HPP
#define __ENUM_HPP

namespace R
{
    enum ENDIANESS: uint8_t
    {
        LE = 0x00,
        BE = 0x01
    };

    enum TYPE: uint8_t
    {
        INT64 = 0x00,
        INT32 = 0x01
    };
}

#endif