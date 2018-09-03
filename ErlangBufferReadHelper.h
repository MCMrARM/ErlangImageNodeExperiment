#pragma once

#include <ei.h>
#include <string>

class ErlangBufferReadHelper {

private:
    const char* data;
    int index = 0;

public:
    ErlangBufferReadHelper(const char* data) : data(data) {}

    int getType(int* size = nullptr) {
        int ret, _size;
        if (ei_get_type(data, &index, &ret, size == nullptr ? &_size : size) < 0)
            throw std::runtime_error("Failed to get the type");
        return ret;
    }

#define _DecodeFunc(classFuncName, erlangFuncName, type) \
    type classFuncName() { \
        type ret; \
        if (erlangFuncName(data, &index, &ret) < 0) \
            throw std::runtime_error("Failed to read the specified type"); \
        return ret; \
    }
#define _DecodeStringFunc(classFuncName, erlangFuncName) \
    std::string classFuncName() { \
        int type, size; \
        if (ei_get_type(data, &index, &type, &size) < 0) \
            throw std::runtime_error("Failed to get the string length"); \
        std::string ret; \
        ret.resize(size); \
        if (erlangFuncName(data, &index, &ret[0]) < 0) \
            throw std::runtime_error("Failed to read the specified string"); \
        return ret; \
    }

    _DecodeFunc(decodeVersion, ei_decode_version, int)
    _DecodeFunc(decodeULong, ei_decode_ulong, unsigned long)
    _DecodeFunc(decodeLong, ei_decode_long, long)
    _DecodeFunc(decodeDouble, ei_decode_double, double)
    _DecodeFunc(decodeBoolean, ei_decode_boolean, int)
    _DecodeFunc(decodeChar, ei_decode_char, char)
    _DecodeStringFunc(decodeString, ei_decode_string)
    _DecodeStringFunc(decodeAtom, ei_decode_atom)

#undef _DecodeFunc

};