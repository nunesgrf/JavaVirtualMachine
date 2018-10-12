#ifndef ___FIELDINFO_H___
#define ___FIELDINFO_H___

#include <cstdint>
#include <vector>
#include "AttributeInfo.hpp"

class FieldInfo {
    public:
        uint16_t access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;
        uint16_t attributes_count;
        /* TO DO: An Attribute info instance */
        AttributeInfo *attributes;

        void static read(ClassLoader, FILE*);
};

#endif