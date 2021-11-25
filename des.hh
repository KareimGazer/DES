#pragma once

#include "globals.hh"

/*********   Helper Functions **********/

/*
  inputs:
    input: input string
    permutationTable: array specify the place of next index in the input string
                      which is opposite to the way mentioned in lectures
                      a mapping from the table to the input string. 
    n: the size of the permutation table
  output: permuted string
*/
string permute(string input, int * permutationTable, int n);

/*
  performs circular left shift on the given string
  inputs:
    input: input string
    shiftsNum: number of left shifts
    regWidth: the register (i.e word) width it should be 28 for 56-bit key
  output: shifted left string
*/
string circularShiftLeft(string input, int shiftsNum, int regWidth);

/*
  performs XOR operation on the two input strings
  inputs:
    a: first input string
    b: second input string
  output: result of the XOR operation
*/
string XOR(string a, string b);

/*
  performs hex to binary conversion
  inputs:
    s: input string
  output: result of the conversion operation
*/
string hex2bin(string s);

/*
  performs hex to binary conversion
  inputs:
    s: input string
  output: result of the conversion operation
*/
string bin2hex(string s);

/*
  performs one DES round
  inputs:
    leftHalf: leftHalf of permuted plaintext
    rightHalf: rightHalf of permuted plaintext
  output:
    leftHalf: leftHalf of encrypted plaintext
    rightHalf: rightHalf of encrypted plaintext
*/
void DES_round(string &leftHalf, string &rightHalf, string roundKeyBinary, string roundKeyHex, int roundIndex);

void genRoundKeys(string key, vector<string> &roundKeysBinary, vector<string> &roundKeysHex);

string encrypt(string plainText, vector<string> roundKeysBinary, vector<string> roundKeysHex);
