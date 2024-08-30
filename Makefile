
TARGET = tangerine
CC = x86_64-w64-mingw32-g++ -c -O3 -fpermissive  
LD = x86_64-w64-mingw32-g++ 


all: $(TARGET).exe

$(TARGET).exe: main.o tangerine.o tgVideoOut.o emul.o memio.o srec.o sdramDmaRegs.o rootRegs.o
	$(LD) main.o tangerine.o tgVideoOut.o emul.o memio.o srec.o sdramDmaRegs.o rootRegs.o -lmingw32  -lSDL2main -lSDL2 -o bin/$(TARGET)

main.o: main.cpp
	$(CC) -o main.o main.cpp 

tangerine.o: tangerine.cpp
	$(CC) -o tangerine.o tangerine.cpp 

tgVideoOut.o: tgVideoOut.cpp
	$(CC) -o tgVideoOut.o tgVideoOut.cpp 

sdramDmaRegs.o: sdramDmaRegs.cpp
	$(CC) -o sdramDmaRegs.o sdramDmaRegs.cpp 

rootRegs.o: rootRegs.cpp
	$(CC) -o rootRegs.o rootRegs.cpp 

emul.o: emul.cpp
	$(CC) -o emul.o emul.cpp 

memio.o: memio.cpp
	$(CC) -o memio.o memio.cpp 

srec.o: srec.cpp
	$(CC) -o srec.o srec.cpp 


clean:
	rm -f *.o *.rec *.disasm *.elf *.mif *.mi *.bin *.map *.exe
	rm -f bin/*.exe


run: $(TARGET).exe
	cd bin
	start $(TARGET).exe
	
	