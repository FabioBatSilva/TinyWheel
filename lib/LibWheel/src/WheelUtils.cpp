#include <WheelUtils.h>
#include <Arduino.h>

uint8_t WheelUtils::byteToUint8(uint8_t b) {
    return b & 255;
}

uint16_t WheelUtils::bytesToUint16(uint8_t* bytes, size_t length) {
    if(length < 2) {
        return 0;
    }

    return (byteToUint8(bytes[0]) << 8) + byteToUint8(bytes[1]);
}

std::string WheelUtils::bytesToHexString(uint8_t* bytes, size_t size) {
    char hexstr[size * 2 + 1];

    for (int i = 0; i < size; i++) {
        // Format as base16 string.
        sprintf(hexstr + i * 2, "%02X", bytes[i]);
    }

    return {hexstr};
}

std::string WheelUtils::bytesToHexString(std::vector<uint8_t>* bytes) {
    return WheelUtils::bytesToHexString(bytes->data(), bytes->size());
}

std::vector<uint8_t> WheelUtils::hexStringToBytes(std::string* hex) {
    std::vector<uint8_t> bytes;

    for (unsigned int i = 0; i < hex->length(); i += 2) {
        std::string byteString = hex->substr(i, 2);
        uint8_t byte = strtol(byteString.c_str(), NULL, 16);

        bytes.push_back(byte);
    }

    return bytes;
}
