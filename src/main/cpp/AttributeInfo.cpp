#ifndef CPP_ATR
#define CPP_ATR

#include "InstructionReader.cpp"
#include "../hpp/AttributeInfo.hpp"
#include "ByteReader.cpp"
#include "CpAttributeInterface.cpp"
#include <iomanip>
#include <iostream>
using namespace std;

ByteReader<uint8_t> OneByte;
ByteReader<uint16_t> TwoByte;
ByteReader<uint32_t> FourByte;

AttributeInfo::~AttributeInfo() {

}

void ConstantValue::read(FILE *fp) {
    constvalue_index = TwoByte.byteCatch(fp);
}
void ConstantValue::print(std::vector<CpInfo*> trueCpInfo) {
    CpAttributeInterface utf8Getter;

    cout << "Constante Value Index= constantpool[" << this->constvalue_index << "]" << utf8Getter.getUTF8(trueCpInfo, this->constvalue_index - 1) << endl;
}

void NumberTableAttribute::read(FILE *fp) {
    length = TwoByte.byteCatch(fp);
    line_number_table = (NumberTableAttribute_lineNumber *)malloc(length*sizeof(NumberTableAttribute_lineNumber));
    for(int i=0; i< length; i++){
        line_number_table[i].start_pc = TwoByte.byteCatch(fp);
        line_number_table[i].lineNumber = TwoByte.byteCatch(fp);
    }
}
void NumberTableAttribute::print() {
    cout << "\nLine Number Table" << endl;
    for(int i=0;i < this->length ; i++ ){
        cout <<"["<< i <<"] || "<< this->line_number_table[i].start_pc << " || " << this->line_number_table[i].lineNumber << endl;
    }    
    cout << "\n\n";
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


void CodeAttribute::print(std::vector<CpInfo*> trueCpInfo) {
    unsigned int i, j, k;
    CpAttributeInterface utf8Getter;
    InstructionReader instructionReader;

    cout << "Max Stacks " << this->max_stacks << endl;
    cout << "Max Locals " << this->max_locals << endl;

    cout << "\nCode:" << endl<<endl;
    for (i = 0; i < this->code_length; i++){
        int code_num = (int)code[i];
        instructionReader.print(code_num);
    }
    cout << "\n";



    cout << "Exception Table Length:" << endl <<endl;
    for (j = 0; j < this->exception_table_length; j++){
        cout << "Start PC =" << this->code_exception_table[j].start_pc<< endl;
        cout << "End PC =" << this->code_exception_table[j].end_pc<< endl;
        cout << "Handler PC=" << this->code_exception_table[j].handler_pc<< endl;
        if(code_exception_table[j].catch_type) {
            cout << "Catch type=constantpool[" << this->code_exception_table[j].catch_type<< "]" << utf8Getter.getUTF8(trueCpInfo, this->code_exception_table[j].catch_type-1) << endl;
        }
    }

    cout << "Attributes Count=" << this->attributes_count << endl;
    for (k = 0; k < this->attributes_count; k++){
        attributes[k].print(trueCpInfo);
    }
}
void InnerClass::read(FILE *fp) {
    class_length = TwoByte.byteCatch(fp);
}

void InnerClass::print(std::vector<CpInfo*> trueCpInfo) {
    cout << "Class length=" << this->class_length<<endl;
}

void Exception::read(FILE *fp) {
    int i;

    number_exceptions = TwoByte.byteCatch(fp);
    exception_index_table = (uint16_t *)malloc(number_exceptions*sizeof(uint16_t));
    for (i = 0; i < number_exceptions; i++){
        exception_index_table[i] = TwoByte.byteCatch(fp);
    }
}

void Exception::print(std::vector<CpInfo*> trueCpInfo) {
    int i;
    CpAttributeInterface utf8Getter;

    cout << "Number of exceptions=" << number_exceptions <<endl;
    for (i = 0; i < number_exceptions; i++){
        cout << setw(2) << setfill('0') << "Exception Index Table =constantpool[" << this->exception_index_table[i]<< "]" << utf8Getter.getUTF8(trueCpInfo, exception_index_table[i]-1) << endl;
    }
}

void SourceFile::read(FILE *fp) {
 
    sourceFileIndex = TwoByte.byteCatch(fp);
    
}

void SourceFile::print(std::vector<CpInfo*> trueCpInfo) {
    CpAttributeInterface utf8Getter;

    cout <<"Source file name index = constantpool[" << this->sourceFileIndex << "]" <<" <" << utf8Getter.getUTF8(trueCpInfo, this->sourceFileIndex - 1)<< ">" << endl;
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
    else if(attribute_name == "SourceFile"){
        sourceFile.read(fp);
    }
    else if(attribute_name == "LineNumberTable"){
        lineNumber.read(fp);
    }
    else {
        info = (uint8_t*)malloc(length*sizeof(uint8_t));
        /* Ignore attribute if it doesn't exist */
        for(int j = 0; j < length ; j++) {
            info[j] = OneByte.byteCatch(fp);
        }
        
    }
}

void AttributeInfo::print(std::vector<CpInfo *> trueCpInfo) {
    CpAttributeInterface utf8Getter;
    std::string attribute_name = utf8Getter.getUTF8(trueCpInfo, this->name_index-1);

    cout << setw(2) << setfill('0') << "Attribute Name Index= constantpool[" << this->name_index<< "] " <<"<"<< attribute_name << ">" <<endl;
    cout << setw(2) << setfill('0') << "Attribute Length = " << this->length<< endl;
    
    if(attribute_name == "Code"){
        code.print(trueCpInfo);
    }

    else if(attribute_name == "ConstantValue"){
        constant_value.print(trueCpInfo);
    }

    else if(attribute_name == "Exceptions"){
        exception.print(trueCpInfo);
    }
    else if(attribute_name == "InnerClass"){
        inner_class.print(trueCpInfo);
    }
    else if(attribute_name == "SourceFile"){
        sourceFile.print(trueCpInfo);
    }
    else if(attribute_name == "LineNumberTable"){
        lineNumber.print();
    }

}

#endif
