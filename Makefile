SFML_PATH = C:/msys64/mingw64
CXX = g++

CXXFLAGS = -I$(SFML_PATH)/include -std=c++17 -O3
LDFLAGS = -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32 -lfreetype

OBJECTS = Main.o Player.o Level.o Physics.o Render.o Ui.o
TARGET = Main.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q *.o $(TARGET)

.PHONY: all clean