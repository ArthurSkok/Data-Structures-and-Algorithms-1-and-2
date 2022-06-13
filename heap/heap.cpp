//Arthur Skok heap.cpp file for DSA 2 PROJ 2
#include "heap.h"
#include <iostream>
#include <string>
#include <vector>

#include <bits/stdc++.h>
using namespace std;

heap::heap(int capacity):mapping(capacity*2){
    data.resize(capacity + 1);
    filled = 0;
}

int heap::insert(const std::string &id, int key, void *pv){
    if(filled == capacity - 1){
        return 1;
    }
    else{
        if(mapping.contains(id)){
            //in this case a NODE with the given ID already exists
            //check the list of NODES 
            return 2;
        }
        else{
            int hole = ++filled;
            node new_node;
            new_node.id = id;
            new_node.key = key;
            new_node.pData = pv;
            data[hole] = new_node;
            //Kenny helped me with this insert call here, I was confused how to actually place the node
            mapping.insert(id, &data[filled]);
            //The percolate up call is needed to ensure that the heap is ordered correctly, and
            //up, not down, is used because it is placed at the final position available.
            percolateUp(filled);
            return 0;
        }
    }   
}

int heap::setKey(const std::string &id, int key){
    if(!(mapping.contains(id))){
        return 1;
    }
    else{
        //needed an intermediary place to put the pointer, otherwise it would
        //have been overwritten and lost
        int placeholder;
        //note "b" input for getPointer is optional
        node *pn = static_cast<node *> (mapping.getPointer(id));
        placeholder = pn -> key;
        pn -> key = key;
        int posCur = getPos(pn);
        //here to correct heap order property once the key is changed, but in this
        //case can either necessitate percolations up or down as setkey can cause
        //a node to possess a key that can either be higher or lower than its
        //position in the binary heap would allow
        if(key < placeholder){
            percolateUp(posCur);
        }
        else if(key > placeholder){
            percolateDown(posCur);
        }
        return 0;
    }
}
int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
    if(filled == 0){
        return 1;
        //heap would be empty
    }
    else{
        if(!(pId == nullptr)){
            //the data in the [1] position
            *pId = data[1].id;
        }
        if(!(pKey == nullptr)){
            *pKey = data[1].key;
        }
        if(!(ppData == nullptr)){
            //supplied by assignment
            *(static_cast<void **> (ppData)) = data[1].pData;
        }
        mapping.remove(data[1].id);
        data[1] = data[filled--];
        percolateDown(1);
        return 0;       
    }
}

int heap::remove(const std::string &id, int *pKey, void *ppData){
    if(!(mapping.contains(id))){
        return 1;
    }
    else{
        node *pn=static_cast<node *>(mapping.getPointer(id));
        // If pKey is supplied, write to that address the key of the node being deleted.
        if(!(pKey == nullptr)){
            *pKey = pn -> key;
        }
        //If ppData is supplied, write to that *address* the associated void pointer.
        if(!(ppData == nullptr)){
            *(static_cast<void **> (ppData)) = pn -> pData;
        }
        //Regardless do:
        //This was an interesting idea I talked to Kenny Huang about, instead of 
        //doing all the reordering again in the remove function, we could essentially
        //just ensure that the node we are trying to remove has its key set to the lowest
        //possible value to ensure deletion when deleteMin (which was already defined) 
        //of the specified node takes place
        setKey(id, INT_MIN);
        deleteMin();
    }
}

void heap::percolateDown(int posCur){
    //From figure 6.12 of textbook:
    //instead of using the "move" command, setPointer was used 
    int child;
    node tmp = data[posCur];
    //noting that filled is the number of entries in the heap
    for(; (posCur * 2) <= filled; posCur = child){
        child = posCur * 2;
        if(!(child == filled) && (data[child + 1].key < data[child].key)){
            child++;
        }
        if(data[child].key < tmp.key){
            data[posCur] = data[child];
            mapping.setPointer(data[posCur].id, &data[posCur]);
        }
        else
        {
            break;
        }
    }
    data[posCur] = tmp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}
void heap::percolateUp(int posCur){
    //the debugging here was annoying to keep track of, switched
    //from the for loop 
    node tmp = data[posCur];
    int parent = posCur / 2;
    while(posCur > 1 && (tmp.key < data[posCur /2].key)){
        data[posCur] = data[posCur /2];
        mapping.setPointer(data[posCur].id, &data[posCur]);    
    posCur = parent;
    //parent node is one power of two higher (lower in value) than either
    //of its two children
    parent = posCur / 2;
    }  
    data[posCur] = tmp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}
