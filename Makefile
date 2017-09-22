# Update Submodules

#GIT = git
#GIT_SUBMODULES := $(shell sed -nE 's/path = +(.+)/\1\/.git/ p' .gitmodules | paste -s -)

#.PHONY: all
#all: $(GIT_SUBMODULES)

#$(GIT_SUBMODULES): %/.git: .gitmodules
#	$(GIT) submodule init
#	$(GIT) submodule update $*

# Build Alsa

# Build RtMidi

# Build Project

TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build
EXTERNALS_DIR ?= ./externals
INC_DIRS ?= ./include
SRC_DIRS ?= ./src

SRCS = $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# C++ Source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
