//Arthur Skok
//Programming Assignment 1 DSA 2

//Definitions for functions named in provided hash.h file. 

//This file took so much longer to debug than the spellchecker, especially since gdb isn't that useful for debugging exec files produced by
//the make command (can't put breakpoints in anywhere intelligible to find out where all the seg fault errors were).

//Nonetheless, I still used make with the -g flag to run it in debug mode, as it still helped somewhat telling which function the seg fault
//manifested in, although it was tedious as this method was debugging backwards, fixing things that might not have needed to be fixed 
//as I was not able to put breakpoints at the start of the operation of this file (I assumed that the bugs were due to this file over the
//spellcheck file, as this file does a lot more with memory allocation and referencing).

#include "hash.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<unsigned int> listPrimes = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};
//List from https://www.planetmath.org/goodhashtableprimes#:~:text=each%20number%20in%20the%20list%20is%20prime.%202.,because%20it%20minimizes%20clustering%20in%20the%20hashed%20table.
hashTable::hashTable(int size){
    filled = 0;
    capacity = getPrime(size);
	//This loop was used to avoid a segmentation fault error that Taylor Foster helped me out with
    for(unsigned int i = 0; i < capacity; i++){
        data.push_back(hashItem());
    }
    //cout << "hashTable works";
}

int hashTable::insert(const string &key, void *pv){
    if(filled*2 >= capacity){
       rehash();
	   if(filled*2 >= capacity){
			//cout << "hash insert works but no size" << endl;
		  	return 2;
	   }
	}
    int currentPos = hash(key);
    if(contains(key) == true){
        return 1;
    }
    while(data[currentPos].isOccupied){
        currentPos ++;
        if(currentPos == capacity){
            currentPos = 0;
        }
    }
    data[currentPos].key = key;
	data[currentPos].isOccupied = true;
	data[currentPos].pv = pv;
    filled++;
    return 0;
    //cout << "hash insert works";
	//To get the position of the data, I had already defined the findPos function,
    //so I just use it in the insert function since we're already given a key through the insert function
	//to be used in the findPos function
}

bool hashTable::contains(const string &key){
	int currentPos = findPos(key);
	if (currentPos == -1){ 
		return false; 
	}
	else{
		return (!data[currentPos].isDeleted);
	}
}

// A hash routine for string objects from the textbook, added some optimization for potential wrap around 
int hashTable::hash(const string &key){
    int hashVal = 0;
    for(int i = 0; i < key.length(); i++)
        hashVal = 37 * hashVal + key[i];
    hashVal %= capacity;
	if(hashVal < 0){
		hashVal += capacity;
	}
	return hashVal;
}

int hashTable::findPos(const std::string &key){
    int currentPos = hash(key);
    //Searching for an unused slot
    while(data[currentPos].isOccupied){
        if(!data[currentPos].isDeleted && data[currentPos].key == key){
            //cout << "hash findpos works";
            return currentPos;
        }
        else{
            currentPos++;
            if(currentPos == capacity){
                currentPos = 0;
            }
        }
    }
    return -1;
}

unsigned int hashTable::getPrime(int size){
    int i = 0;
    if(listPrimes[25] < size){
            return -1;
            //cout << "Size is too large" << endl;
    }
    while(listPrimes[i] < size){
        i++;
    }
    return(listPrimes[i]);
}
//Got help from a friend for the rehash function because the form I was originally doing it in had segmentation faults that made me
//essentially debug in circles, as the debugger only told where the error manifested, but not where it occured as it might have otherwise
//been told as when using breakpoints.
bool hashTable::rehash(){
	vector<hashItem> oldData = data;
	int newCapacity = getPrime(capacity*2);
	capacity = newCapacity;
	filled = 0;
	data.resize(newCapacity);
	for(int i=0; i<data.size(); i++){
        	data[i].isOccupied=data[i].isOccupied=false;
	}
	for(int i=0; i<oldData.size();i++){
		if(oldData[i].isOccupied && !oldData[i].isDeleted){
			insert(oldData[i].key, oldData[i].pv);
		}
	}
	return true;
}




