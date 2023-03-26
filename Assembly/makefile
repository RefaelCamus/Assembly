

OBJS = Assembler.o GeneralList.o PhaseZero.o GeneralFunctions.o Operation.o Address.o Registers.o PhaseOne.o PhaseSecond.o


ASSEMBLER_FILES = *.ob *.ext *.ent *.o
Assembler: $(OBJS)
	gcc -g -ansi -Wall -pedantic $(OBJS) -o Assembler


GeneralList.o: GeneralList.c GeneralList.h
	gcc -g -c -ansi -Wall -pedantic GeneralList.c -o GeneralList.o

PhaseZero.o: PhaseZero.c PhaseZero.h 
	gcc -g -c -ansi -Wall -pedantic PhaseZero.c -o PhaseZero.o
	

GeneralFunctions.o: GeneralFunctions.c GeneralFunctions.h 
	gcc -g -c -ansi -Wall -pedantic GeneralFunctions.c -o GeneralFunctions.o

	
Operation.o: Operation.c Operation.h
	gcc -g -c -ansi -Wall -pedantic Operation.c -o Operation.o
	
	
Registers.o: Registers.c Registers.h
	gcc -g -c -ansi -Wall -pedantic Registers.c -o Registers.o
	
Address.o: Address.c Address.h
	gcc -g -c -ansi -Wall -pedantic Address.c -o Address.o	
	
PhaseOne.o: PhaseOne.c PhaseOne.h
	gcc -g -c -ansi -Wall -pedantic PhaseOne.c -o PhaseOne.o

PhaseSecond.o: PhaseSecond.c PhaseSecond.h
	gcc -g -c -ansi -Wall -pedantic PhaseSecond.c -o PhaseSecond.o
	

Assembler.o: Assembler.c 
	gcc -g -c -ansi -Wall -pedantic Assembler.c -o Assembler.o

	
clean:
	rm -f $(OBJS) $(ASSEMBLER_FILES) Assembler 		

	
