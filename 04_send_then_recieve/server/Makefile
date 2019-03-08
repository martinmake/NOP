IDIRS:=../inc inc
SDIR:=src
ODIR:=obj
ELF :=bin/main.elf
ASM :=bin/main.asm
HEX :=bin/main.hex
BIN :=bin/main.bin

INC:=$(patsubst %,-I%,$(IDIRS))

C_SRCS:=$(shell find $(SDIR) -name *.c)
CXX_SRCS:=$(shell find $(SDIR) -name *.cpp)

C_OBJS:=$(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(C_SRCS))
CXX_OBJS:=$(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(CXX_SRCS))

CC:=gcc
CXX:=g++
LD:=g++

CCFLAGS:=$(INC)
CXXFLAGS:=$(INC)
LDFLAGS:=

all: $(ELF)
	@echo =====BUILDING=======
	objcopy -j .text -j .data -O ihex $(ELF) $(HEX)
	objcopy -j .text -j .data -O binary $(ELF) $(BIN)
	objdump -D -z $(ELF) > $(ASM)
	size $(ELF)
	@echo ========DONE========

$(ELF): $(C_OBJS) $(CXX_OBJS) $(HEADERS)
	@echo ======LINKING=======
	$(LD) $(LDFLAGS) $(C_OBJS) $(CXX_OBJS) -o $(ELF)
	@echo ========DONE========

$(ODIR)/%.o: $(SDIR)/%.c
	@echo =====COMPILING======
	$(CC) $(CCFLAGS) -c $< -o $@
	@echo ========DONE========

$(ODIR)/%.o: $(SDIR)/%.cpp
	@echo =====COMPILING======
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo ========DONE========

clean:
	@echo ======CLEANING======
	-rm -f $(C_OBJS) $(CXX_OBJS) $(ELF) $(HEX) $(BIN) $(ASM)
	@echo ========DONE========
