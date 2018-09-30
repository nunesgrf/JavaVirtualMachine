#ifndef ___CPINFO_H___
#define ___CPINFO_H___

#include <cstdint>
#include <vector>



struct CpInfo {

    uint8_t tag;
    union {
        struct {
            uint16_t name_index;
        } Class;
        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } Fieldref;
        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } Methodref;
        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } InterfaceMethodref;
        struct {
            uint16_t string_index;
        } String;
        struct {
            uint32_t bytes;
        } Integer;
        struct {
            uint32_t bytes;
        } Float;
        struct {
            uint32_t high_bytes;
            uint32_t low_bytes;
        } Long;
        struct {
            uint32_t high_bytes;
            uint32_t low_bytes;
        } Double;
        struct {
            uint16_t name_index; 
	        uint16_t descriptor_index;
        } NameAndType;
        struct {
            uint16_t length; 
	        std::vector<uint8_t> bytes;
        } UTF8;
    };   

};



#endif // ___CPINFO_H____