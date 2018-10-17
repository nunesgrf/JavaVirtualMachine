#ifndef CPP_ATR
#define CPP_ATR

#include "../hpp/AttributeInfo.hpp"
#include "ByteReader.cpp"
#include "CpAttributeInterface.cpp"

ByteReader<uint8_t> OneByte;
ByteReader<uint16_t> TwoByte;
ByteReader<uint32_t> FourByte;

AttributeInfo::~AttributeInfo() {

}

void ConstantValue::read(FILE *fp) {
    constvalue_index = TwoByte.byteCatch(fp);
}

void CodeAttribute::read(FILE *fp, std::vector<CpInfo*> trueCpInfo) {
    unsigned int i, j, k;

    max_stacks = TwoByte.byteCatch(fp);
    max_locals = TwoByte.byteCatch(fp);
    code_length = FourByte.byteCatch(fp);

    code = (uint8_t *)malloc(code_length*sizeof(uint8_t));
    for (i = 0; i < code_length; i++){
        code[i] = OneByte.byteCatch(fp);
    }

    exception_table_length = TwoByte.byteCatch(fp);

    code_exception_table = (CodeException *)malloc(exception_table_length * sizeof(CodeException));
    for (j = 0; j < exception_table_length; j++){
        code_exception_table[j].start_pc = OneByte.byteCatch(fp);
        code_exception_table[j].end_pc = OneByte.byteCatch(fp);
        code_exception_table[j].handler_pc = OneByte.byteCatch(fp);
        code_exception_table[j].catch_type = OneByte.byteCatch(fp);
    }

    attributes_count = TwoByte.byteCatch(fp);
    attributes = (AttributeInfo *)malloc(attributes_count * sizeof(AttributeInfo));
    for (k = 0; k < attributes_count; k++){
        attributes[k].read(fp, trueCpInfo);
    }
}

void InnerClass::read(FILE *fp) {
    class_length = TwoByte.byteCatch(fp);
}

void Exception::read(FILE *fp) {
    int i;

    number_exceptions = TwoByte.byteCatch(fp);
    exception_index_table = (uint16_t *)malloc(number_exceptions*sizeof(uint16_t));
    for (i = 0; i < number_exceptions; i++){
        exception_index_table[i] = TwoByte.byteCatch(fp);
    }
}

void AttributeInfo::read(FILE * fp, std::vector<CpInfo *> trueCpInfo){

    CpAttributeInterface utf8Getter;

    name_index = TwoByte.byteCatch(fp);
    length = FourByte.byteCatch(fp);

    std::string attribute_name = utf8Getter.getUTF8(trueCpInfo, name_index-1);
    
    if(attribute_name == "Code"){
        code.read(fp,trueCpInfo);
    }

    else if(attribute_name == "ConstantValue"){
        constant_value.read(fp);
    }

    else if(attribute_name == "Exceptions"){
        exception.read(fp);
    }

    else if(attribute_name == "InnerClass"){
        inner_class.read(fp);
    }

    else {
        info = (uint8_t*)malloc(length*sizeof(uint8_t));
        /* Ignore attribute if it doesn't exist */
        for(int j = 0; j < length ; j++) {
            info[j] = OneByte.byteCatch(fp);
        }
    }
}

#endif