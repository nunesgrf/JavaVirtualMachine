IDIR=../hpp
CXX=g++
CXXFLAGS=-I$(IDIR) -g -std=c++11 -Wall

_DEPS= AttributeInfo.hpp ByteReader.hpp ClassLoader.hpp ClassPrinter.hpp CpAttributeInterface.hpp CpInfo.hpp FieldInfo.hpp Frame.hpp Instruction.hpp InstructionImpl.hpp InterfaceInfo.hpp MethodInfo.hpp Stack.hpp CpAttributeInterface.hpp CpInfo.hpp FieldInfo.hpp Frame.hpp Instruction.hpp InstructionImpl.hpp InterfaceInfo.hpp Interpreter.hpp MethodInfo.hpp Stack.hpp jvm.hpp 
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

OBJ = AttributeInfo.o ByteReader.o ClassLoader.o ClassPrinter.o CpAttributeInterface.o CpInfo.o FieldInfo.o Frame.o Instruction.o InstructionImpl.o InterfaceInfo.o MethodInfo.o Stack.o CpAttributeInterface.o CpInfo.o FieldInfo.o Frame.o Instruction.o InstructionImpl.o InterfaceInfo.o Interpreter.o MethodInfo.o Stack.o jvm.o 

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

jvm: $(OBJ)
	 $(CXX) -o $@ $^ $(CXXFLAGS)