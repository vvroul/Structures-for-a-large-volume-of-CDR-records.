OBJS 	= mainSample.o NodeFunctions.o ListNodeFunctions.o HashListFunctions.o HashFunctions.o BinaryMaxHeapFunctions.o Utilities.o BucketNodeFunctions.o DeListFunctions.o RegionFunctions.o SubListFunctions.o
SOURCE	= mainSample.cpp NodeFunctions.cpp ListNodeFunctions.cpp HashListFunctions.cpp HashFunctions.cpp BinaryMaxHeapFunctions.cpp Utilities.cpp BucketNodeFunctions.cpp DeListFunctions.cpp RegionFunctions.cpp SubListFunctions.cpp
HEADER  =  Headers.hpp Record.hpp RestRecord.hpp Node.hpp ListNode.hpp HashList.hpp HashTable.hpp BinaryMaxHeap.hpp Utilities.hpp BucketNode.hpp DeList.hpp Region.hpp SubList.hpp
CC	= g++
FLAGS   = -g -lrt -c

all : werhauz

werhauz : mainSample.cpp NodeFunctions.cpp ListNodeFunctions.cpp HashListFunctions.cpp HashFunctions.cpp BinaryMaxHeapFunctions.cpp Utilities.cpp BucketNodeFunctions.cpp DeListFunctions.cpp RegionFunctions.cpp SubListFunctions.cpp
	$(CC) -o $@ $^

mainSample.o: mainSample.cpp
	$(CC) $(FLAGS) mainSample.cpp

NodeFunctions.o: NodeFunctions.cpp
	$(CC) $(FLAGS) NodeFunctions.cpp

ListNodeFunctions.o: ListNodeFunctions.cpp
	$(CC) $(FLAGS) ListNodeFunctions.cpp

HashListFunctions.o: HashListFunctions.cpp
	$(CC) $(FLAGS) HashListFunctions.cpp

HashFunctions.o: HashFunctions.cpp
	$(CC) $(FLAGS) HashFunctions.cpp

BinaryMaxHeapFunctions.o: BinaryMaxHeapFunctions.cpp
	$(CC) $(FLAGS) BinaryMaxHeapFunctions.cpp

BucketNodeFunctions.o: BucketNodeFunctions.cpp
	$(CC) $(FLAGS) BucketNodeFunctions.cpp

Utilities.o: Utilities.cpp
	$(CC) $(FLAGS) Utilities.cpp

DeListFunctions.o: DeListFunctions.cpp
	$(CC) $(FLAGS) DeListFunctions.cpp

RegionFunctions.o: RegionFunctions.cpp
	$(CC) $(FLAGS) RegionFunctions.cpp			

SubListFunctions.o: SubListFunctions.cpp
	$(CC) $(FLAGS) SubListFunctions.cpp

# clean
clean:
	rm -f $(OBJS) werhauz

#accounting
count:
	wc $(SOURCE) $(HEADER)
