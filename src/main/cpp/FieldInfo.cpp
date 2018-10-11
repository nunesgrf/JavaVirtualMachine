#ifndef ___FIELDINFO_H___
#define ___FIELDINFO_H___

#include <cstdint>
#include <vector>


class FieldsInfo {
    public:
        uint16_t access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;
        uint16_t attributes_count;
        /* TO DO: An Attribute info instance */

};

#endif