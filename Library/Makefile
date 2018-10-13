# Install Boost in your machine
# Replace Boost include folder path
HEADER			= -I /usr/local/Cellar/boost/1.66.0/include

# Replace Boost lib folder path
LIBB			= -L/usr/local/Cellar/boost/1.66.0/lib
LIFF			= -lboost_filesystem -lboost_system

CC			:= g++
#WARNING			:= -Wall
CFLAGS			:= -O5 -std=c++14
LDFLAGS			:= -lm
TARGET			:= hyppox

UNAME			:= $(shell uname)
ifeq ($(UNAME), Darwin)
  CFLAGS		+= $(HEADER)
  LDFLAGS		+= $(LIBB) $(LIFF)
else ifeq ($(UNAME), Linux)
  LDFLAGS		+= $(LIFF)
endif

SOURCES				=	$(wildcard *.h)
OBJECTS				=	$(SOURCES:%.h=%.o)

all: $(TARGET)

hyppox: $(OBJECTS) main.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o *.dSYM
	
.PHONY: all clean

