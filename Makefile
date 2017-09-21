CC=gcc
CXX=g++

TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build
EXTERNALS_DIR ?= ./externals
INCLUDE_DIR ?= ./include
SRC_DIR ?= ./src

SRCS = -L$(SRC_DIR) -L$(EXTERNALS_DIR)/alsa-lib/src/ -L$(EXTERNALS_DIR)/rtmidi
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I$(INCLUDE_DIR) -I$(EXTERNALS_DIR)/alsa-lib/include -I$(EXTERNALS_DIR)/rtmidi)

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
