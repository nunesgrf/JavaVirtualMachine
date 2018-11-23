#Para escrever comentários ##
############################# Makefile ##########################
all:	jvm
jvm:	AttributeInfo.o ByteReader.o ClassLoader.o ClassPrinter.o CpAttributeInterface.o CpInfo.o FieldInfo.o Frame.o Instruction.o InstructionImpl.o InterfaceInfo.o MethodInfo.o Stack.o bla.o
# O compilador faz a ligação entre os dois objetos
	g++ -o jvm ../../../obj/AttributeInfo.o ../../../obj/ByteReader.o ../../../obj/Interpreter.o ../../../obj/ClassLoader.o ../../../obj/ClassPrinter.o ../../../obj/CpAttributeInterface.o ../../../obj/CpInfo.o ../../../obj/FieldInfo.o ../../../obj/Frame.o ../../../obj/Instruction.o ../../../obj/InstructionImpl.o ../../../obj/InterfaceInfo.o ../../../obj/MethodInfo.o ../../../obj/Stack.o ../../../obj/bla.o
#-----> Distancia com o botão TAB ### e não com espaços
AttributeInfo.o:	AttributeInfo.cpp
	g++ -std=c++11 -c AttributeInfo.cpp ../../../obj/AttributeInfo.o

ByteReader.o:	ByteReader.cpp
	g++ -std=c++11 -c ByteReader.cpp ../../../obj/ByteReader.o

ClassLoader.o:	ClassLoader.cpp
	g++ -std=c++11 -c ClassLoader.cpp ../../../obj/ClassLoader.o

ClassPrinter.o:	ClassPrinter.cpp
	g++ -std=c++11 -c ClassPrinter.cpp ../../../obj/ClassPrinter.o

CpAttributeInterface.o:	CpAttributeInterface.cpp
	g++ -std=c++11 -c CpAttributeInterface.cpp ../../../obj/CpAttributeInterface.o

CpInfo.o:	CpInfo.cpp
	g++ -std=c++11 -c CpInfo.cpp ../../../obj/CpInfo.o

FieldInfo.o:	FieldInfo.cpp
	g++ -std=c++11 -c FieldInfo.cpp ../../../obj/FieldInfo.o

Frame.o:	Frame.cpp
	g++ -std=c++11 -c Frame.cpp ../../../obj/Frame.o

Instruction.o:	Instruction.cpp
	g++ -std=c++11 -c Instruction.cpp ../../../obj/Instruction.o

InstructionImpl.o:	InstructionImpl.cpp
	g++ -std=c++11 -c InstructionImpl.cpp ../../../obj/InstructionImpl.o

InterfaceInfo.o:	InterfaceInfo.cpp
	g++ -std=c++11 -c InterfaceInfo.cpp ../../../obj/InterfaceInfo.o

MethodInfo.o:	MethodInfo.cpp
	g++ -std=c++11 -c MethodInfo.cpp ../../../obj/MethodInfo.o
		
Stack.o:	Stack.cpp
	g++ -std=c++11 -c Stack.cpp ../../../obj/Stack.o

Interpreter.o:	Interpreter.cpp
	g++ -std=c++11 -c Interpreter.cpp ../../../obj/Interpreter.o

bla.o:	bla.cpp
	g++ -std=c++11 -c bla.cpp ../../../obj/bla.o

#clean:
#	rm -rf *.o
#mrproper: clean
#	rm -rf teste