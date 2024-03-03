CC = fasm
LIBS = asmlib/fmt.asm asmlib/str.asm asmlib/sys.asm asmlib/math.asm
LIBS_OBJ = asmlib/fmt.o asmlib/str.o asmlib/sys.o asmlib/math.o
.PHONY: default compile build once dump hex run
default: build run
compile: $(LIBS)
	fasm asmlib/fmt.asm && fasm asmlib/str.asm && fasm asmlib/sys.asm && fasm asmlib/math.asm
build: main.asm
	$(CC) main.asm
	ld main.o $(LIBS_OBJ) -o main
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
