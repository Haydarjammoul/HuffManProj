//Haydar JAMMOUL & Antonin Grigoire
// Huffman.cpp : This file contains the 'main' function that calls the method to encode the text in custData
//and the method that decodes the text after recieving a bin file,that has the Frequencymap in Mapdb; and the encrypted file that has our compressed text
//the decoded text is written in Decoded

#include <iostream>
#include <string>
#include "Text.h"
using namespace std;
int main()
{
    std::cout << "Hello World!\n";
    Text tut;
    tut.codageHuffman();
    tut.Decode();

}
