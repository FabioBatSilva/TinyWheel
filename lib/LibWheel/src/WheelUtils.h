#pragma once

#include <string>
#include <vector>

class WheelUtils
{
    public:
        static uint8_t byteToUint8(uint8_t b);

        static uint16_t bytesToUint16(uint8_t* bytes, size_t length);

        static std::string bytesToHexString(uint8_t* bytes, size_t size);

        static std::string bytesToHexString(std::vector<uint8_t>* bytes);

        static std::vector<uint8_t> hexStringToBytes(std::string* hex);
};
