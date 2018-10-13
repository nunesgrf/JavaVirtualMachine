#ifndef ___FIELDINFO_H___
#define ___FIELDINFO_H___

#include "ClassLoader.hpp"
#include "AttributeInfo.hpp"
#include "../cpp/AttributeInfo.cpp"

class FieldInfo {
    private:
        uint16_t access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;
        uint16_t attributes_count;
        /* TO DO: An Attribute info instance */
        AttributeInfo *attributes;

    public:
        uint16_t getAccessFlags() {
            return this->access_flags;
        }
        uint16_t getNameIndex() {
            return this->name_index;
        }
        uint16_t getDescriptorIndex() {
            return this->descriptor_index;
        }
        uint16_t getAttributesCountes() {
            return this->attributes_count;
        }
        AttributeInfo* getInterfaceInfo() {
            return this->attributes;
        }
        void setFieldInfo(FILE*);
};

#endif