#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
// #include <stdint.h>

#ifdef __GNUC__
# define __rdtsc __builtin_ia32_rdtsc
#else
# include<intrin.h>
#endif

#define getBit(index, data) (data >> index & 1)

#define setBit(index, data) (data |= u64(1ULL << index))

#define clearBit(index, data) (data &= ~(1ULL << index))

#define DES2Cindex(index, n) (n - index)

#define C2DESindex(index, n) (n - index)


/*
* returns the number of bits in a data type
*/
#define len(dataType) sizeof(dataType) * 8

using namespace std;

int initialPermutation[64] = {
                               58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
                               62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
                               57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
                               61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
                             };

int finalPermutation[64] = {
                            40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
                            36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
                            34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
                           };



int keyp[56] = { 57, 49, 41, 33, 25, 17, 9,
                 1, 58, 50, 42, 34, 26, 18,
                 10, 2, 59, 51, 43, 35, 27,
                 19, 11, 3, 60, 52, 44, 36,

                 63, 55, 47, 39, 31, 23, 15,
                 7, 62, 54, 46, 38, 30, 22,
                 14, 6, 61, 53, 45, 37, 29,
                 21, 13, 5, 28, 20, 12, 4
               };

int keyCompressionTable[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
                                23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
                                41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                                44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
                              };

int shiftTable[16] = { 1, 1, 2, 2,
                       2, 2, 2, 2,
                       1, 2, 2, 2,
                       2, 2, 2, 1
                     };

/* Round Globals */

// Expansion D-box Table
int expansionPermutationTable[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11,
                  12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
                  22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
                };

// S-box table
int sBoxTable[512] = { 
                   
                     14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
                   ,
                   
                     15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                     13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
                   ,
                   
                     10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
                    ,
                    
                      7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                      13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                      10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                      3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
                    ,
                    
                      2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                      14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                      4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                      11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
                    ,
                    
                      12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                      10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                      9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                      4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
                    ,
                    
                      4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                      13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                      1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                      6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
                    ,
                    
                      13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                      1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                      7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                      2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
                    
};

// Straight Permutation Table
int straightPermutationTable[32] = { 16, 7, 20, 21, 29, 12, 28,  17, 1, 15, 23, 26, 5, 18, 31, 10,
                                     2, 8, 24, 14,32, 27, 3, 9,19, 13, 30, 6,22, 11, 4, 25 };


typedef unsigned long long u64;

u64 permute(u64 plainText, int * permutationTable, int inputLen, int outputLen);

u64 circularShiftLeft(u64 input, int shiftsNum);

u64 applySboxs(u64 input);

vector<u64> getRoundKeys(u64 key);

u64 encrypt(u64 plainText, vector<u64> roundKeys);

int main(int argc, char *argv[]) {
  // u64  plainText = 0x123456ABCD132536;
  // u64  key = 0xAABB09182736CCDD;
  string PT, K, condition;
  condition = argv[1];
  PT = argv[2];
  K = argv[3];

  u64  plainText = stoull (PT, nullptr, 16);
  u64  key = stoull (K, nullptr, 16);

  vector<u64> roundKeys = getRoundKeys(key);

  if(condition == "encrypt"){
    long long t1 = __rdtsc();
    u64 cipher = encrypt(plainText, roundKeys);
    long long t2 = __rdtsc();
    printf("Cipher: %016llX\n", cipher);
    printf("Cycles: %lld\n", t2-t1);
  }
  else if(condition == "decrypt"){
    reverse(roundKeys.begin(), roundKeys.end());
    long long t1 = __rdtsc();
    u64 cipher = encrypt(plainText, roundKeys);
    long long t2 = __rdtsc();
    printf("Plain: %016llX\n", cipher);
    printf("Cycles: %lld\n", t2-t1);
  }
  else{
    cout << "error: please type encrypt or decrypt as first argument" << endl;
  }

  return 0;
}

u64 encrypt(u64 plainText, vector<u64> roundKeys){
  u64 rawData = permute(plainText, initialPermutation, 64, 64);
  // u64 rawData = initialPer;
  for (int i = 0; i < 16; i++) {
    u64 rightHalf = rawData & 0xFFFFFFFF;
    u64 leftHalf = (rawData & 0xFFFFFFFF00000000) >> 32;

    u64 rightExpanded = permute(rightHalf, expansionPermutationTable, 32, 48);

    u64 roundKey = roundKeys[i];

    u64 Fx = rightExpanded ^ roundKey;
    // printf("Fx: %016llX\n", Fx);
    //s-box
    u64 Sout = applySboxs(Fx);
    // printf("Sout: %016llX\n", Sout);

    // Straight D-box
    u64 straightPerm = permute(Sout, straightPermutationTable, 32, 32);
    // printf("straightPerm: %016llX\n", straightPerm);
    Fx = straightPerm ^ leftHalf;
    // printf("Fx2: %016llX\n", Fx);
    leftHalf = Fx;
    //swapping
    rawData = (rightHalf<<32) | leftHalf;

  }

  // 32-bit swap
  u64 rightHalf = rawData & 0xFFFFFFFF;
  u64 leftHalf = (rawData & 0xFFFFFFFF00000000) >> 32;
  rawData = (rightHalf<<32) | leftHalf;

  u64 finalPerm = permute(rawData, finalPermutation, 64, 64);
  return finalPerm;
}

vector<u64> getRoundKeys(u64 key){
  vector<u64> roundKeys;
  key = permute(key, keyp, 64, 56);
  u64 rightSubkey = key & 0xFFFFFFF;
  u64 leftSubkey = (key & 0x00FFFFFFF0000000) >> 28;
  for (int i = 0; i < 16; i++) {
    leftSubkey = circularShiftLeft(leftSubkey, shiftTable[i]);
    rightSubkey = circularShiftLeft(rightSubkey, shiftTable[i]);
    u64 combinedKey = (leftSubkey<<28) | rightSubkey;
    u64 roundKey = permute(combinedKey, keyCompressionTable, 56, 48);
    roundKeys.push_back(roundKey);
  }
  return roundKeys;
}

u64 applySboxs(u64 input){
  u64 result = 0x00;
  int index; int* box;
  for(int i=0; i<8; i++){
    box = sBoxTable + i * 64; //fetch the current S-box
    index = input >> (7-i) * 6 & 0x3F; //get the index
    index = index>> 1 & 15 | (index & 1) << 4 | index & 0x20; //reorder bits
    result |= box[index] << (7-i) * 4;
  }
  return result;
}

u64 circularShiftLeft(u64 input, int shiftsNum){
  u64 result = 0x00;
  //printf("input: %016llX\n", input);
  for (int i = 0; i < shiftsNum; i++) {
    u64 bit = u64(getBit(27, input));
    result = input << 1;
    clearBit(28, result);
    result |= bit;
    input = result;
  }
  return input;
}

u64 permute(u64 plainText, int * permutationTable, int inputLen, int outputLen){
  u64 out=0;
  for(int i=0;i<outputLen;++i)
  out|=(plainText>>(inputLen-permutationTable[outputLen-1-i])&1)<<i;
  return out;
}
