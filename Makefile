# Install Boost in your machine
CC := g++
WARNING := -Wall
CFLAGS := -w -g -std=c++11
TARGET := main
# Replace Boost include folder path
HEADER = -I /usr/local/Cellar/boost/1.66.0/include
# Replace Boost lib folder path
LIBB = -L/usr/local/Cellar/boost/1.66.0/lib
LIFF = -lboost_filesystem -lboost_system

OBJECTS = config.o unionFindWithPathCompression.o customCollection.o RGBColor.o clusterInformation.o phenotype.o dataPoint.o cluster.o quickSort.o dbScan.o quadNode.o quadTree.o fileHandler.o individual.o graph.o simplicialComplex.o mapper.o 

all: $(TARGET)

main: $(TARGET).cpp $(OBJECTS)
	$(CC) $(HEADER) $(CFLAGS) $(LIBB) $(LIFF) -o $(TARGET) $(TARGET).cpp $(OBJECTS)

config.o: config.h config.cpp
	$(CC) $(CFLAGS) -c config.cpp

unionFindWithPathCompression.o: unionFindWithPathCompression.h unionFindWithPathCompression.cpp
	$(CC) $(CFLAGS) -c unionFindWithPathCompression.cpp

customCollection.o: customCollection.h customCollection.cpp
	$(CC) $(CFLAGS) -c customCollection.cpp

RGBColor.o: RGBColor.h RGBColor.cpp
	$(CC) $(CFLAGS) -c RGBColor.cpp

clusterInformation.o: clusterInformation.h clusterInformation.cpp
	$(CC) $(CFLAGS) -c clusterInformation.cpp

phenotype.o: phenotype.h phenotype.cpp
	$(CC) $(CFLAGS) -c phenotype.cpp

dataPoint.o: dataPoint.h dataPoint.cpp
	$(CC) $(CFLAGS) -c dataPoint.cpp

cluster.o: cluster.h cluster.cpp
	$(CC) $(CFLAGS) -c cluster.cpp

quickSort.o: quickSort.h quickSort.cpp
	$(CC) $(CFLAGS) -c quickSort.cpp

dbScan.o: dbScan.h dbScan.cpp
	$(CC) $(CFLAGS) -c dbScan.cpp

quadNode.o: quadNode.h quadNode.cpp
	$(CC) $(CFLAGS) -c quadNode.cpp

quadTree.o: quadTree.h quadTree.cpp
	$(CC) $(CFLAGS) -c quadTree.cpp

fileHandler.o: fileHandler.h fileHandler.cpp
	$(CC) $(CFLAGS) -c fileHandler.cpp

individual.o: individual.h individual.cpp
	$(CC) $(CFLAGS) -c individual.cpp

graph.o: graph.h graph.cpp
	$(CC) $(CFLAGS) -c graph.cpp

simplicialComplex.o: simplicialComplex.h simplicialComplex.cpp
	$(CC) $(CFLAGS) -c simplicialComplex.cpp

mapper.o: mapper.h mapper.cpp
	$(CC) $(CFLAGS) -c mapper.cpp

clean:
	rm -rf main *.o *.dSYM
	
.PHONY: all clean

