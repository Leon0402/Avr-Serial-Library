TARGET := Serial

#avr
MCU        := atmega328p
PROGRAMMER := arduino
PORT       := -P /dev/ttyUSB0
BAUD       := -b  57600
F_CPU      := 16000000

#Compiler
CXX       := avr-g++
LDFLAGS   := -Os  -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CXXFLAGS  := -Wall -std=c++11 -c -Os -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -D F_CPU=$(F_CPU)

# Commands
REMOVE := rm -r -f

#Directories
SRCDIR := src
OBJDIR := obj
BINDIR := bin

# Files
cppfiles := $(wildcard $(SRCDIR)/*.cpp)
objects  := $(subst $(SRCDIR)/, $(OBJDIR)/, $(cppfiles:.cpp=.o))

.Phony: avr install size clean

avr: $(BINDIR)/$(TARGET).hex

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BINDIR)/$(TARGET).elf: $(objects)
	mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -o  $@ $^

$(BINDIR)/$(TARGET).hex: $(BINDIR)/$(TARGET).elf
	avr-objcopy -O ihex -j .data -j .text $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex

upload:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U flash:w:$(BINDIR)/$(TARGET).hex:i

size:
	avr-size --mcu=$(MCU) -C --format=avr $(BINDIR)/$(TARGET).elf

clean:
	$(REMOVE) $(OBJDIR)
	$(REMOVE) $(BINDIR)
