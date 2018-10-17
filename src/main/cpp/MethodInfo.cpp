#ifndef CPP_METHOD
#define CPP_METHOD

#include "../hpp/MethodInfo.hpp"
#include "ByteReader.cpp"

MethodInfo::~MethodInfo()
{
    free(this->attributes);
}

void MethodInfo::read(FILE *fp, std::vector<CpInfo*> trueCpInfo)
{
    int i = 0;
    ByteReader<uint16_t> reader;

    this->access_flags = reader.byteCatch(fp);
    this->name_index = reader.byteCatch(fp);
    this->descriptor_index = reader.byteCatch(fp);
    this->attributes_count = reader.byteCatch(fp);

    
    this->attributes = (AttributeInfo *)calloc(attributes_count, sizeof(AttributeInfo));
    for(i = 0; i < this->attributes_count; i++){
        this->attributes[i].read(fp,trueCpInfo);
    }
}

#endif 