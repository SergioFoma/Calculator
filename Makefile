CXX = g++

BUILD_DIR = Build

FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align \
	-Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations\
	-Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain\
	-Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy\
	-Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

FOLDER_NAME = ASM
SOURCES = main.cpp paint.cpp assembleCode.cpp onegin.cpp

EXE_NAME = $(addprefix $(FOLDER_NAME)/, $(BUILD_DIR) )/Result.exe  # Processor/Build/Result.exe

OBJECTS = $(SOURCES:.cpp=.obj)

OBJECTS_IN_SOURCE = $(addprefix $(FOLDER_NAME)/src/, $(OBJECTS))  # Processor/src/main.obj

.PHONY: all clean

all: $(EXE_NAME)

$(EXE_NAME): makeBuild $(OBJECTS_IN_SOURCE)
	$(CXX) $(FLAGS) $(addprefix $(FOLDER_NAME)/$(BUILD_DIR)/, $(OBJECTS_IN_SOURCE)) -o $(EXE_NAME)

$(OBJECTS_IN_SOURCE) : %.obj: %.cpp
	$(CXX) $(FLAGS) -I ./$(FOLDER_NAME)/include -c ./$^ -o ./$(FOLDER_NAME)/$(BUILD_DIR)/$@

makeBuild:
	mkdir -p ./$(FOLDER_NAME)/$(BUILD_DIR)/$(FOLDER_NAME)/src
clean:
	rm -rf ./Processor/$(BUILD_DIR)
	rm -rf ./ASM/$(BUILD_DIR)

#Processor:
#	FOLDER_NAME = Processor
#	SOURCES = main.cpp stack.cpp checkError.cpp paint.cpp SPULoop.cpp onegin.cpp softProcessor.cpp
#ASM:
#	FOLDER_NAME = ASM
#	SOURCES = main.cpp paint.cpp assembleCode.cpp onegin.cpp
