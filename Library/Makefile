# Install Boost in your machine
# Replace Boost include folder path
HEADER			= -I /usr/local/Cellar/boost/1.66.0/include $(shell pwd)/include

# Replace Boost lib folder path
LIBB			= -L/usr/local/Cellar/boost/1.66.0/lib
LIFF			= -lboost_filesystem -lboost_system

CC				:= g++
#WARNING			:= -Wall
CFLAGS			:= -O5 -g -std=c++14
LDFLAGS			:= -lm
TARGET			:= hyppox
#SFLAG			:= -m64

UNAME			:= $(shell uname)
ifeq ($(UNAME), Darwin)
  CFLAGS		+= $(HEADER)
  LDFLAGS		+= $(LIBB) $(LIFF)
else ifeq ($(UNAME), Linux)
  LDFLAGS		+= $(LIFF)
endif

#CFLAGS		+= $(SFLAG)

SOURCES				=	$(wildcard *.cpp)
OBJECTS				=	$(SOURCES:.cpp=.o)

all: $(TARGET)

hyppox: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

#%.o: %.h
#	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o *.dSYM
	
.PHONY: all clean

