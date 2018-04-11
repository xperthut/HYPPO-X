# Install Boost in your machine
# Replace Boost include folder path
HEADER			= -I /usr/local/Cellar/boost/1.66.0/include

# Replace Boost lib folder path
LIBB			= -L/usr/local/Cellar/boost/1.66.0/lib
LIFF			= -lboost_filesystem -lboost_system

CC			:= g++
WARNING			:= -Wall
CFLAGS			:= -O5 -w -g -std=c++11
LDFLAGS			:= -lm
TARGET			:= hyppox

UNAME			:= $(shell uname)
ifeq ($(UNAME), Darwin)
  CFLAGS		+= $(HEADER)
  LDFLAGS		+= $(LIBB) $(LIFF)
endif


OBJECTS = config.o unionFindWithPathCompression.o customCollection.o RGBColor.o clusterInformation.o phenotype.o dataPoint.o cluster.o quickSort.o dbScan.o quadNode.o quadTree.o fileHandler.o individual.o graph.o simplicialComplex.o mapper.o 

all: $(TARGET)

hyppox: $(OBJECTS) main.o
	$(CC) -o $@ $^ $(LDFLAGS)

# $(CC) $(HEADER) $(CFLAGS) $(LIBB) $(LIFF) -o $(TARGET) $(TARGET).cpp $(OBJECTS)


%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o *.dSYM
	
.PHONY: all clean

