#include "../hpp/FieldInfo.hpp"

void FieldInfo::setFieldInfo(FILE *fp) {
    ByteReader<uint16_t> TwoByte;
    access_flags = TwoByte.byteCatch(fp);
    name_index = TwoByte.byteCatch(fp);
    descriptor_index = TwoByte.byteCatch(fp);
    attributes_count = TwoByte.byteCatch(fp);
    
    for(int i = 0; i < attributes_count; i++)
    {
        /* Allocate a attribute */
        AttributeInfo * attribute = (AttributeInfo *)calloc(1, sizeof(*attribute));

        /* Puts into the vector of attributes  */
        attributes.push_back(attribute);
        attributes[i]->read(fp);
    }
    

}