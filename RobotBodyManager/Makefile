#

CROSS_COMPILE 	= ae32000-elf-uclibc-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)g++
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib
ELF2FLT	= $(CROSS_COMPILE)elf2flt

CFLAGS	= -O2 -Wall -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED
COMPILER_PREFIX=/usr/local/ae32000-elf-uclibc-tools
LDFLAGS = -r -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld
LDFLAGS_DBG = -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld

#라이브러리 파일 추가
LDLIBS	= ../library/library.a

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB CFLAGS
PROJ = RobotBodyManager
OBJS = main.o CRobotBodyManager.o CTimeManager.o CRobotControlManager.o \
		CStateManager.o CMessageCommManager.o
		
all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC) $(LDFLAGS) -o $@.elf $^ $(LDLIBS)
	$(CC) $(LDFLAGS_DBG) -o $@.dbg $^ $(LDLIBS)
	$(ELF2FLT) $@.elf
	mv $@.elf.bflt $@

clean:
	rm -f *.o *.elf *.bflt *.dis $(TEST) *.dbg RobotBodyManager
