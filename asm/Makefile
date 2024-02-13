CC = fasm
LIBS = asmlib/fmt.o asmlib/str.o asmlib/sys.o asmlib/mth.o
.PHONY: default compile build once dump hex run
default: build run
compile: asmlib/fmt.o asmlib/str.o asmlib/sys.o asmlib/mth.o
	fasm asmlib/fmt.o && fasm asmlib/str.o && fasm asmlib/sys.o && fasm asmlib/mth.o
build: main.asm
	$(CC) main.asm
	ld main.o $(LIBS) -o main
once: main.asm
	$(CC) main.asm
	ld main.o -o main
dump: main
	objdump -S -M intel -d main > obj.dump
	cat obj.dump
hex: main
	hexeditor main
run: main		
	./main
