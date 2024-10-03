#      for passing arguments add this to the html file
#      var Module = {
#        arguments: ['tunnel.rec'],

TARGET = tangerine
CC = emcc -c -O3 -fpermissive  -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 
LD = emcc -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sINITIAL_MEMORY=73400320
                                                         


all: $(TARGET).html

$(TARGET).html: main.o tangerine.o tgVideoOut.o emul.o memio.o srec.o sdramDmaRegs.o rootRegs.o spiSdCardRegs.o usbHostRegs.o sdCard.o blitterRegs.o blitter.o usbHost.o disasm.o audioRegs.o audio.o
	$(LD) main.o tangerine.o tgVideoOut.o emul.o memio.o srec.o sdramDmaRegs.o rootRegs.o spiSdCardRegs.o usbHostRegs.o sdCard.o blitterRegs.o blitter.o usbHost.o disasm.o audioRegs.o audio.o --preload-file bin@/ -o html/$(TARGET).html

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

spiSdCardRegs.o: spiSdCardRegs.cpp
	$(CC) -o spiSdCardRegs.o spiSdCardRegs.cpp

sdCard.o: sdCard.cpp
	$(CC) -o sdCard.o sdCard.cpp

usbHostRegs.o: usbHostRegs.cpp
	$(CC) -o usbHostRegs.o usbHostRegs.cpp 

usbHost.o: usbHost.cpp
	$(CC) -o usbHost.o usbHost.cpp 

blitterRegs.o: blitterRegs.cpp
	$(CC) -o blitterRegs.o blitterRegs.cpp

blitter.o: blitter.cpp
	$(CC) -o blitter.o blitter.cpp

audioRegs.o: audioRegs.cpp
	$(CC) -o audioRegs.o audioRegs.cpp

audio.o: audio.cpp
	$(CC) -o audio.o audio.cpp

emul.o: emul.cpp
	$(CC) -o emul.o emul.cpp 

memio.o: memio.cpp
	$(CC) -o memio.o memio.cpp 

srec.o: srec.cpp
	$(CC) -o srec.o srec.cpp 

disasm.o: disasm.cpp
	$(CC) -o disasm.o disasm.cpp 

clean:
	rm -f *.o *.rec *.disasm *.elf *.mif *.mi *.bin *.map *.exe
	rm -f bin/*.exe
	rm -f html/*


	
	