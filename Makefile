SOURCES_DIR 		:= src
OUTPUT_DIR 			:= bin
BUILD_TYPE 			?= debug
FILE_NAME			:= CenturyPackage
SRCDIRS 			:= $(shell find src -type d)

CELL_MK_DIR 		?= 	$(SCE_PS3_ROOT)/samples/mk
include 			$(CELL_MK_DIR)/sdk.makedef.mk

PPU_SRCS 			:= $(shell find $(SOURCES_DIR) -name "*.c")
PPU_PRX_LDFLAGS 	+= $(PRX_LDFLAGS_EXTRA)
PPU_PRX_TARGET 		=  $(OUTPUT_DIR)/$(BUILD_TYPE)/$(FILE_NAME).prx
PPU_PRX_LDLIBS 		+= -lc -lfs_stub -lsysutil_np_stub -lsysutil_stub -lsysutil_userinfo_stub -lsysmodule_stub

ifeq ($(BUILD_TYPE),release)
PPU_CFLAGS 			= -O2
PRX_LDFLAGS_EXTRA	= -s --stripe-unused-data
else
PPU_CFLAGS 			= -O0
PRX_LDFLAGS_EXTRA	= 
endif

PPU_CFLAGS 			+= -std=c99 -ffunction-sections -fdata-sections -fno-builtin-printf -nodefaultlibs -Wno-shadow -Wno-unused-parameter
PPU_CFLAGS 			+= $(addprefix -I,$(SRCDIRS))

CLEANFILES 			= $(PRX_DIR)/$(PPU_SPRX_TARGET)

include 			$(CELL_MK_DIR)/sdk.target.mk

CLANG_TIDY     ?= clang-tidy
TIDY_CHECKS    ?= clang-analyzer-*,modernize-*,readability-*
TIDY_FLAGS     ?= --checks=$(TIDY_CHECKS) --warnings-as-errors=* --quiet
PS3_SDK_PATH := $(shell cygpath -u "$(SCE_PS3_ROOT)")
PS3_SDK_INC  := $(PS3_SDK_PATH)/target/ppu/include
INCLUDES	   := -isystem $(PS3_SDK_INC)
.PHONY: tidy

rebuild:
	$(MAKE) --no-print-directory clean
	$(MAKE) --no-print-directory all

clean:
	@echo "# removing $(OUTPUT_DIR)/$(BUILD_TYPE) if it is empty.";
	@if [ -d $(OUTPUT_DIR)/$(BUILD_TYPE) ]; then \
		rm -r $(OUTPUT_DIR)/$(BUILD_TYPE); \
	fi

	@rmdir $(OUTPUT_DIR) ||:

release:
	$(MAKE) rebuild BUILD_TYPE=release

tidy: $(PPU_SRCS:%.c=%.tidy)

%.tidy: %.c
	@echo "==> Running clang-tidy on $<"
	@echo "    PS3_SDK_INC = $(PS3_SDK_INC)"
	@echo "    Full command:"
	@echo "      $(CLANG_TIDY) $< --checks=$(TIDY_CHECKS) --warnings-as-errors=* --quiet --extra-arg=-I$(PS3_SDK_INC) -- $(PPU_CFLAGS)"
	@$(CLANG_TIDY) $< \
		--checks=$(TIDY_CHECKS) \
		--warnings-as-errors=* \
		--quiet \
		-- $(PPU_CFLAGS) $(INCLUDES)
