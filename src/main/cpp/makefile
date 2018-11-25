IDIR=../hpp
CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -Wall

_DEPS= AttributeInfo.hpp ByteReader.hpp ClassLoader.hpp ClassPrinter.hpp CpAttributeInterface.hpp CpInfo.hpp FieldInfo.hpp Frame.hpp Instruction.hpp InstructionImpl.hpp InterfaceInfo.hpp MethodInfo.hpp Stack.hpp blanter.hpp CpAttributeInterface.hpp CpInfo.hpp FieldInfo.hpp Frame.hpp Instruction.hpp InstructionImpl.hpp InterfaceInfo.hpp Interpreter.hpp MethodInfo.hpp Stack.hpp bla.hpp 
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

OBJ = AttributeInfo.o ByteReader.o ClassLoader.o ClassPrinter.o CpAttributeInterface.o CpInfo.o FieldInfo.o Frame.o Instruction.o InstructionImpl.o InterfaceInfo.o Interpreter.o MethodInfo.o Stack.o bla.o

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

jvm: $(OBJ)
	 $(CC) -o $@ $^ $(CFLAGS)