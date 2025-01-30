OPTIONS := -O2 -Wall -Wno-sign-compare -Wno-unused-parameter -g
LIBS 		:= -lsfml-graphics -lsfml-window -lsfml-system
EXE_NAME = ESP32_Boy

BIN_PATH     = bin/
BUILD_PATH   = build/
DEP_PATH     = dep/
INCLUDE_PATH = include/
SRC_PATH     = src/

################################################################################

INCLUDE_DIRS = -I$(INCLUDE_PATH)


# get project files
ALL_CPP := $(subst $(SRC_PATH)main.cpp,,$(shell find $(SRC_PATH) $(INCLUDE_PATH) -type f -name "*.cpp"))
ALL_H   := $(shell find $(SRC_PATH) $(INCLUDE_PATH) -type f -name "*.h")
ALL_O   := $(subst $(SRC_PATH),$(BUILD_PATH),$(subst .cpp,.o,$(ALL_CPP)))
ALL_D 	:= $(subst $(SRC_PATH),$(DEP_PATH),$(subst .cpp,.d,$(ALL_CPP)))
DIR_STRUCTURE := $(subst $(SRC_PATH),,$(shell find $(SRC_PATH) -type d))


all: replicate_dirs $(BIN_PATH)$(EXE_NAME)

# this instruction is necessary because g++ won't make the .d and .o files
# inside dep and build unless the folders they should be in are already there
replicate_dirs:
	@mkdir -p $(addprefix $(BUILD_PATH), $(DIR_STRUCTURE))
	@mkdir -p $(addprefix $(DEP_PATH), $(DIR_STRUCTURE))

#linking
$(BIN_PATH)$(EXE_NAME): $(ALL_O)
	@echo ' -> linking'
	@g++ $(OPTIONS) -o $@ $(ALL_O) $(LIBS)
	@echo Finished!

# generic build rule
$(BUILD_PATH)%.o: $(SRC_PATH)%.cpp
	@echo ' -> building:' $<
	@g++ $(OPTIONS) -c $< -o $@ $(INCLUDE_DIRS) -MMD -MF  $(subst $(BUILD_PATH),$(DEP_PATH),$(@:.o=.d))


.PHONY: clean count init

init:
	mkdir bin build dep

clean:
	@echo Removed files: $(ALL_D) $(ALL_O) $(BIN_PATH)$(EXE_NAME)
	@rm -r $(DEP_PATH)* $(BUILD_PATH)* $(BIN_PATH)*

count:
	@echo ' lines words characters'
	@wc $(ALL_CPP) $(ALL_H) | tail -1

-include $(ALL_D)