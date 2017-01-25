PRODUCT     := experiments
CORE_LIB	:= habcore
LIBRARY     := lib$(CORE_LIB).a
C_STD		:= c11


CC          := avr-gcc
AR			:= avr-ar
OBJCPY      := avr-objcopy
UPL         := avrdude

BOARD       := atmega328p
PROTOCOL    := arduino
PROC        := m328p
FREQ        := 16000000UL
AVR_PORT    ?= /dev/tty.usbmodem1411
BAUD        := 115200

SOURCE_DIR	:= ./src
HEADERS_DIR := ./src
BUILD_DIR	:= ./build
OBJECTS_DIR := $(BUILD_DIR)/intermediate
PRODUCT_DIR := $(BUILD_DIR)/product
LIBRARY_DIR := $(PRODUCT_DIR)/lib

CXXFLAGS    := -Os -std=$(C_STD) -DF_CPU=$(FREQ) -mmcu=$(BOARD) \
               -fpack-struct -fshort-enums -ffreestanding \
               -c -fdata-sections -ffunction-sections -Wl,--relax,--gc-sections -I$(HEADERS_DIR)
LDFLAGS     := -Os -mmcu=$(BOARD) -L$(LIBRARY_DIR) -ffreestanding -Wl,--gc-sections
UPLFLAGS    := -F -P $(AVR_PORT) -V -c $(PROTOCOL) -p $(PROC) -b $(BAUD) -U flash:w:

LIBRARIES   := -l$(CORE_LIB)
SOURCES		:= $(wildcard $(SOURCE_DIR)/$(PRODUCT)/*.c)
SOURCES_LIB	:= $(wildcard $(SOURCE_DIR)/$(CORE_LIB)/*.c)
OBJECTS 	:= $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECTS_DIR)/%.o)
OBJECTS_LIB	:= $(SOURCES_LIB:$(SOURCE_DIR)/%.c=$(OBJECTS_DIR)/%.o)

all: $(PRODUCT).elf

upload: $(PRODUCT).hex
	@$(UPL) $(UPLFLAGS)$(PRODUCT_DIR)/$(PRODUCT).hex


$(LIBRARY): $(OBJECTS_LIB)
	@echo "linking core library"
	@mkdir -p $(LIBRARY_DIR)
	@rm -rf $(LIBRARY_DIR)/$@
	@$(AR) -rcs $(LIBRARY_DIR)/$@ $(OBJECTS_LIB)
    

$(PRODUCT).hex : $(PRODUCT).elf
	@echo "creating intel HEX upload file"
	@avr-objdump -d $(PRODUCT_DIR)/$(PRODUCT).elf > $(PRODUCT_DIR)/$(PRODUCT).lst
	@$(OBJCPY) -O ihex -R .eeprom $(PRODUCT_DIR)/$(PRODUCT).elf $(PRODUCT_DIR)/$(PRODUCT).hex
    

$(PRODUCT).elf: $(LIBRARY) $(OBJECTS)
	@echo "linking elf for $@..."
	@$(CC) -o $(PRODUCT_DIR)/$(PRODUCT).elf $(LDFLAGS) $(OBJECTS) $(LIBRARIES)

$(OBJECTS_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "compiling $(notdir $<)..."
	@$(CC) $(CXXFLAGS) $< -o $@

clean:
	@echo "cleaning files..."
	@rm -rf $(BUILD_DIR)