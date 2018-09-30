#ifndef ___CPINFO_H___
#define ___CPINFO_H___

#include <cstdint>
#include <vector>

class CpInfo{};

struct CONSTANT_Class_info              : public CpInfo {
    uint8_t  tag;
    uint16_t name_index;
};
struct CONSTANT_Fieldref_info           : public CpInfo {
    uint8_t  tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
};
struct CONSTANT_Methodref_info          : public CpInfo {
    uint8_t  tag;
    uint16_t class_index; 
	uint16_t name_and_type_index;
};
struct CONSTANT_InterfaceMethodref_info : public CpInfo {
    uint8_t  tag;
    uint16_t class_index; 
	uint16_t name_and_type_index;
};
struct CONSTANT_String_info             : public CpInfo {
    uint8_t  tag;
    uint16_t string_index;
};
struct CONSTANT_Integer_info            : public CpInfo {
    uint8_t  tag;
    uint32_t bytes;
};
struct CONSTANT_Float_info              : public CpInfo {
    uint8_t  tag;
    uint32_t bytes;
};
struct CONSTANT_Long_info               : public CpInfo {
    uint8_t  tag;
    uint32_t high_bytes;
    uint32_t low_bytes;
};
struct CONSTANT_Double_info             : public CpInfo {
    uint8_t  tag;
    uint32_t high_bytes;
    uint32_t low_bytes;
};
struct CONSTANT_NameAndType_info        : public CpInfo {
    uint8_t  tag;
    uint16_t name_index; 
	uint16_t descriptor_index;
};
struct CONSTANT_Utf8_info               : public CpInfo {
    uint8_t  tag;
    uint16_t length; 
	std::vector<uint8_t> bytes;
};

#endif // ___CPINFO_H____