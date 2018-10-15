#include "../hpp/FieldInfo.hpp"

void FieldInfo::read(FILE *fp, std::vector<CpInfo *> trueCpInfo)
{
    ByteReader<uint16_t> TwoByte;
    access_flags = TwoByte.byteCatch(fp);
    name_index = TwoByte.byteCatch(fp);
    descriptor_index = TwoByte.byteCatch(fp);
    attributes_count = TwoByte.byteCatch(fp);
    
    for(int i = 0; i < attributes_count; i++)
    {
        /* Allocate a attribute */
        AttributeInfo * attribute = (AttributeInfo *)calloc(1, sizeof(*attribute));
        attribute[i].read(fp,trueCpInfo);
        /* Puts into the vector of attributes  */
        attributes.push_back(attribute);
    }
    

}