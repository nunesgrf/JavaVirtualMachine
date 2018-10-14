#include "../hpp/FieldInfo.hpp"

void FieldInfo::setFieldInfo(FILE *fp,FieldInfo* fieldinfo) {
    ByteReader<uint16_t> TwoByte;
    fieldinfo->access_flags = TwoByte.byteCatch(fp);
    fieldinfo->name_index = TwoByte.byteCatch(fp);
    fieldinfo->descriptor_index = TwoByte.byteCatch(fp);
    fieldinfo->attributes_count = TwoByte.byteCatch(fp);
    
    for(int i = 0; i < attributes_count; i++)
    {
        /* Allocate a attribute */
        AttributeInfo * attribute = (AttributeInfo *)calloc(1, sizeof(*attribute));
        //attribute.read(fp)
        /* Puts into the vector of attributes  */
        //fieldinfo->attributes.push_back(attribute);
        
    }
    

}