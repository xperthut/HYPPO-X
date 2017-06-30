# Makefile

CC = c++ -g -Wall -std=c++11

OBJECTS = ClusterInformation.o Phenotype.o DataPoint.o Cluster.o QuickSort.o DBScan.o RGBColor.o Box.o FileHandler.o CustomCollection.o Individual.o Graph.o UnionFindWithPathCompression.o ClusteringRegion.o SimplicialComplex.o PersistentHomology.o 

all: main

main: main.cpp $(OBJECTS)
	$(CC) -o main main.cpp $(OBJECTS)

Box.o: Box.h Box.cpp
	$(CC) -c Box.cpp

Cluster.o: Cluster.h Cluster.cpp
	$(CC) -c Cluster.cpp

ClusterInformation.o: ClusterInformation.h ClusterInformation.cpp
	$(CC) -c ClusterInformation.cpp

ClusteringRegion.o: ClusteringRegion.h ClusteringRegion.cpp
	$(CC) -c ClusteringRegion.cpp

CustomCollection.o: CustomCollection.h CustomCollection.cpp
	$(CC) -c CustomCollection.cpp

DataPoint.o: DataPoint.h DataPoint.cpp
	$(CC) -c DataPoint.cpp

DBScan.o: DBScan.h DBScan.cpp
	$(CC) -c DBScan.cpp

FileHandler.o: FileHandler.h FileHandler.cpp
	$(CC) -c FileHandler.cpp

Graph.o: Graph.h Graph.cpp
	$(CC) -c Graph.cpp

Individual.o: Individual.h Individual.cpp
	$(CC) -c Individual.cpp

PersistentHomology.o: PersistentHomology.h PersistentHomology.cpp
	$(CC) -c PersistentHomology.cpp

Phenotype.o: Phenotype.h Phenotype.cpp
	$(CC) -c Phenotype.cpp

QuickSort.o: QuickSort.h QuickSort.cpp
	$(CC) -c QuickSort.cpp

RGBColor.o: RGBColor.h RGBColor.cpp
	$(CC) -c RGBColor.cpp

SimplicialComplex.o: SimplicialComplex.h SimplicialComplex.cpp
	$(CC) -c SimplicialComplex.cpp

UnionFindWithPathCompression.o: UnionFindWithPathCompression.h UnionFindWithPathCompression.cpp
	$(CC) -c UnionFindWithPathCompression.cpp


clean:
	rm -f *.o *.gml main
	rm -rf main.dSYM

