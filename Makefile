SFML_PATH = /home/ali/SFML
CXX = g++

CXXFLAGS = -I$(SFML_PATH)/include -I$(SFML_PATH)/build/include -std=c++17 -O3
LDFLAGS = -L$(SFML_PATH)/build/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wl,-rpath,$(SFML_PATH)/build/lib

OBJECTS = Main.o Player.o Level.o Physics.o Render.o Ui.o libudev.o
TARGET = Main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

libudev.o:
	g++ -c -I$(SFML_PATH)/include $(SFML_PATH)/include/libudev.c -o libudev.o

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
