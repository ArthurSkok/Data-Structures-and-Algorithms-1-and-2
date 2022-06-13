//Arthur Skok
//Programming Assignment 1 
//DSA 2 
//Spellchecker File 
//This file is designed to work with a hash.cpp file, a hash.h file that gives the functions defined in the hash.cpp file, and a makefile to build the resulting executable.
//This specific file provides a way to compare values inputted from an input file to values placed in a dictionary, both via user input. 
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <ctime>
#include "hash.h"
#include <chrono>
using namespace std;

void loaddictionary(hashTable * dictionaryhashTable, string dictionaryFile);
int main() {
    string infile, outfile, dictionaryFile;
    hashTable * dictionaryhashTable = new hashTable(50000);
	chrono::time_point<chrono::system_clock> start,end;
    ifstream inputFile;
    ofstream outputFile;
    char result;
    unsigned long lineNumber = 0;
    int i;
    string word = "";
    string line = "";
	bool number;
	bool generic;
	//End of variable initialization
    cout << "Enter name of dictionary: ";
    cin >> dictionaryFile;
	//used chrono to make more precise measurements, as it takes milliseconds for the sample function to spellcheck and it 
	//would round down to 0 in the terminal cout statements
    start = std::chrono::system_clock::now(); 
    loaddictionary(dictionaryhashTable, dictionaryFile);
    end = std::chrono::system_clock::now(); 
	std::chrono::duration<double> elapsed_seconds = end - start;
    cout << "Total time (in seconds) to load dictionary:" << (elapsed_seconds.count()) << endl;
    cout << "Enter name of input file: ";
    cin >> infile;
    cout << "Enter name of output file: ";
    inputFile.open(infile);
    if(inputFile.fail()){
        cerr << "Failed to open the input file:" << infile << strerror(errno) << endl;
    }
    cin >> outfile;
    outputFile.open(outfile);
    if(outputFile.fail()){
        cerr << "Failed to open the output file:" << outfile << strerror(errno) << endl;
    }
	start = std::chrono::system_clock::now(); 
    while(getline(inputFile, line)){
        //go line by line to compare between dictionary and input file
        lineNumber++;
        for(i = 0; i <= line.length(); i++){
            result = tolower(line[i]);
            generic = isalnum(result);
            number = isdigit(result);
            if(result == '-' || result == '\'' || generic){
				//designed to ignore the numbers in the input file
                if(!number){
                    word = word + result;
				}
            }
			//If there are no proper characters used to compose words in this dictionary, check if the word variable is too long (and there is a word stored)
			else if(word.length() > 0){
				if(word.length() > 20){
                    outputFile << "Long word at line " << lineNumber << ", starts: " << word.substr(0,20) << endl;
                }
				else if((!dictionaryhashTable -> contains(word))){
                    outputFile << "Unknown word at line " << lineNumber << ": " << word << endl;
                }
				//if the above if statement at the top of this tree is not true, the remaining characters should be 
				//word separators so the word variable should be empty
				word ="";
			}
            else{
                word = ""; 
				number = false;          
            }
        }
	}
	end = std::chrono::system_clock::now(); 
	elapsed_seconds = end - start;
	cout << "Total time (in seconds) to spellcheck file:" << (elapsed_seconds.count()) << endl;

}

void loaddictionary(hashTable * myhash, string mydict){
	ifstream dict;
    int i;	
	dict.open(mydict);
    if(dict.fail()){
        cerr << "Failed to open the dictionary file: " << mydict << strerror(errno) << endl;
    }
    string line;
    while(getline(dict, line)){
        for(i = 0; i <= line.length(); i++){
            line[i] = tolower(line[i]);
        }
        myhash -> insert(line,NULL);
    }
    dict.close();
}



   
    
