#include "../hpp/AttributeInfo.hpp"


ByteReader<uint8_t> OneByte;
ByteReader<uint16_t> TwoByte;
ByteReader<uint32_t> FourByte;

ConstantValue ConstantValue::read(ClassLoader * classloader,FILE *fp, AttributeInfo attribute_info) {
    attribute_info.name_index = TwoByte.byteCatch(fp);
    attribute_info.length = FourByte.byteCatch(fp);
    attribute_info.constant_value.constvalue_index = TwoByte.byteCatch(fp);

    return attribute_info.constant_value;
}

CodeAttribute CodeAttribute::read(ClassLoader * classloader,FILE *fp, AttributeInfo attribute_info) {
    unsigned int i, j, k;

    attribute_info.name_index = TwoByte.byteCatch(fp);
    attribute_info.length = FourByte.byteCatch(fp);
    attribute_info.code.max_stacks = TwoByte.byteCatch(fp);
    attribute_info.code.max_locals = TwoByte.byteCatch(fp);
    attribute_info.code.code_length = FourByte.byteCatch(fp);

    attribute_info.code.code = (uint8_t *)malloc(attribute_info.code.code_length*sizeof(uint8_t));
    for (i = 0; i < attribute_info.code.code_length; i++){
        attribute_info.code.code[i] = OneByte.byteCatch(fp);
    }

    attribute_info.code.exception_table_length = TwoByte.byteCatch(fp);

    attribute_info.code.code_exception_table = (CodeException *)malloc(attribute_info.code.exception_table_length * sizeof(CodeException));
    for (j = 0; j < attribute_info.code.exception_table_length; j++){
        attribute_info.code.code_exception_table[j].start_pc = OneByte.byteCatch(fp);
        attribute_info.code.code_exception_table[j].end_pc = OneByte.byteCatch(fp);
        attribute_info.code.code_exception_table[j].handler_pc = OneByte.byteCatch(fp);
        attribute_info.code.code_exception_table[j].catch_type = OneByte.byteCatch(fp);
    }

    attribute_info.code.attributes_count = TwoByte.byteCatch(fp);
    attribute_info.code.attributes = (AttributeInfo *)malloc(attribute_info.code.attributes_count * sizeof(AttributeInfo));
    for (k = 0; k < attribute_info.code.exception_table_length; k++){
        /* Don't know what to do here */
    }
    return attribute_info.code;
}

InnerClass InnerClass::read(ClassLoader * classloader,FILE *fp, AttributeInfo attribute_info) {
    attribute_info.name_index = TwoByte.byteCatch(fp);
    attribute_info.length = FourByte.byteCatch(fp);
    attribute_info.inner_class.name_index = TwoByte.byteCatch(fp);
    attribute_info.inner_class.length = FourByte.byteCatch(fp);
    attribute_info.inner_class.class_length = TwoByte.byteCatch(fp);

    return attribute_info.inner_class;
}

Synthetic Synthetic::read(ClassLoader * classloader,FILE *fp, AttributeInfo attribute_info) {
    /* A class member that does not appear in the source code must be marked using a Synthetic attribute */
}

Exception Exception::read(ClassLoader * classloader,FILE *fp, AttributeInfo attribute_info) {
    int i;

    attribute_info.name_index = TwoByte.byteCatch(fp);
    attribute_info.length = FourByte.byteCatch(fp);
    attribute_info.exception.number_exceptions = TwoByte.byteCatch(fp);
    attribute_info.exception.exception_index_table = (uint16_t *)malloc(attribute_info.exception.number_exceptions*sizeof(uint16_t));
    for (i = 0; i < attribute_info.exception.number_exceptions; i++){
        attribute_info.exception.exception_index_table[i] = TwoByte.byteCatch(fp);
    }

    return attribute_info.exception;
}

AttributeInfo AttributeInfo::get_attribute_info(ClassLoader * classloader,FILE * fp, AttributeInfo attribute_info) {
    attribute_info.name_index = TwoByte.byteCatch(fp);
    attribute_info.length = FourByte.byteCatch(fp);

    /* Need to get attribute name inside constant pool */
    std::string attribute_name = "1";
    return attribute_info;
}