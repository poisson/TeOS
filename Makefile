AUXFILES := Makefile linker.ld # Non-source, non-object
PROJDIRS := src/c src/asm src/cpp
CSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.c")
CPPSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.cpp")
ASMSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.s")
32CSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.c32")
32CPPSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.cpp32")
32ASMSRCFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.s32")
CHDRFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.h")
CPPHDRFILES := $(shell find $(PROJDIRS) -mindepth 1 -maxdepth 3 -name "*.hpp")
COBJFILES := $(patsubst %.c,%.o,$(CSRCFILES))
CPPOBJFILES := $(patsubst %.cpp,%.o,$(CPPSRCFILES))
ASMOBJFILES := $(patsubst %.s,%.o,$(ASMSRCFILES))
CDEPFILES := $(patsubst %.c,%.d,$(CSRCFILES))
CPPDEPFILES := $(patsubst %.cpp,%.d,$(CPPSRCFILES))
SRCFILES := $(CSRCFILES) $(CPPSRCFILES) $(ASMSRCFILES)
32OBJFILES := $(patsubst %.c32,%.o,$(32CSRCFILES)) $(patsubst %.cpp32,%.o,$(32CPPSRCFILES)) $(patsubst %.s32,%.o,$(32ASMSRCFILES))
HDRFILES := $(CHDRFILES) $(CPPHDRFILES)
OBJFILES := $(COBJFILES) $(CPPOBJFILES) $(ASMOBJFILES)
DEPFILES := $(CDEPFILES) $(CPPDEPFILES)
ALLFILES := $(SRCFILES) $(HDRFILES) $(AUXFILES)

.PHONY: clean

CFLAGS := -O3 -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -std=c99 -mcmodel=large -ffreestanding
CPPFLAGS := -O3 -Wall -Wextra -Werror -ffreestanding -m64 -nostdlib -nostartfiles -nodefaultlibs -fno-exceptions -mcmodel=large -fno-rtti
INCLUDES := -I./src/c/include -I./src/cpp/include -I/usr/include

all: boot.img

boot.img: kernel.bin
	sudo mount -o loop boot.img floppy
	sudo cp kernel.bin floppy
	sudo umount floppy

kernel.bin: $(OBJFILES) $(32OBJFILES) src/linker.ld
	x86_64-elf-ld -T src/linker.ld -nostdlib -nodefaultlibs -o kernel.bin $(OBJFILES) $(32OBJFILES)

-include $(DEPFILES)

%.o: %.c Makefile
	x86_64-elf-gcc $(CFLAGS) -MMD -MP -MT "$*.d" $(INCLUDES) -c $< -o $@

%.o: %.s Makefile
	nasm -f elf64 -o $@ $<

%.o: %.cpp Makefile
	x86_64-elf-g++ $(CPPFLAGS) -MMD -MP -MT "$*.d" $(INCLUDES) -c $< -o $@

clean:
	-@$(RM) $(wildcard $(OBJFILES) $(32OBJFILES) $(DEPFILES) kernel.bin loader.bin)

todolist:
	-@for file in $(ALLFILES); do fgrep -H -e TODO -e FIXME -e ENHANCE $$file; done; true
