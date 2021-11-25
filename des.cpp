#include "des.hh"

void genRoundKeys(string key, vector<string> &roundKeysBinary, vector<string> &roundKeysHex){
  key = hex2bin(key);
  key = permute(key, keyp, 56);

  string leftHalf = key.substr(0, 28);
  string rightHalf = key.substr(28, 28);

  for (int i = 0; i < 16; i++) {
        // Shifting
        leftHalf = circularShiftLeft(leftHalf, shiftTable[i], 28);
        rightHalf = circularShiftLeft(rightHalf, shiftTable[i], 28);
 
        // Combining
        string combine = leftHalf + rightHalf;
 
        // Key Compression
        string RoundKey = permute(combine, keyCompressionTable, 48);

        roundKeysBinary.push_back(RoundKey);
        roundKeysHex.push_back(bin2hex(RoundKey));
    }
}

string hex2bin(string s){
    string bin = "";
    for (int i = 0; i < s.size(); i++) {
        bin += hexBinaryMap[s[i]];
    }
    return bin;
}

string bin2hex(string s){
  string hex = "";
  for (int i = 0; i < s.length(); i += 4) {
      string ch = "";
      ch += s[i];
      ch += s[i + 1];
      ch += s[i + 2];
      ch += s[i + 3];
      hex += binaryHexMap[ch];
    }
  return hex;
}

string permute(string input, int * permutationTable, int n){
    string permutedString = "";
    for (int i = 0; i < n; i++) {
        permutedString += input[permutationTable[i] - 1];
    }
    return permutedString;
}

string circularShiftLeft(string input, int shiftsNum, int regWidth){
    string strBuffer = "";
    for (int i = 0; i < shiftsNum; i++) {
        for (int j = 1; j < regWidth; j++) {
            strBuffer += input[j];
        }
        strBuffer += input[0];
        input = strBuffer;
        strBuffer = "";
    }
    return input;
}

string XOR(string a, string b){
    string answer = "";
    for (int i = 0; i < a.size(); i++) {
      if (a[i] == b[i]) answer += "0";
      else answer += "1";
    }
    return answer;
}

void DES_round(string &leftHalf, string &rightHalf, string roundKeyBinary, string roundKeyHex, int roundIndex){
  // Expansion D-box
  string right_expanded = permute(rightHalf, expansionPermutationTable, 48);
 
  // XOR RoundKey[i] and right_expanded
  string x = XOR(roundKeyBinary, right_expanded);
 
  // S-boxes
  string op = "";
  for (int i = 0; i < 8; i++) {
    int row = 2 * int(x[i * 6] - '0') + int(x[i * 6 + 5] - '0');
    int col = 8 * int(x[i * 6 + 1] - '0') + 4 * int(x[i * 6 + 2] - '0') + 2 * int(x[i * 6 + 3] - '0') + int(x[i * 6 + 4] - '0');
    int val = sBoxTable[i][row][col];
    op += char(val / 8 + '0');
    val = val % 8;
    op += char(val / 4 + '0');
    val = val % 4;
    op += char(val / 2 + '0');
    val = val % 2;
    op += char(val + '0');
  }

  // Straight D-box
  op = permute(op, straightPermutationTable, 32);
 
  // XOR left and op
  x = XOR(op, leftHalf);
 
  leftHalf = x;
 
  // Swapper
  swap(leftHalf, rightHalf);
        
  // cout << "Round " << roundIndex + 1 << " " << bin2hex(leftHalf) << " "<< bin2hex(rightHalf) << " " << roundKeyHex << endl;
}

string encrypt(string plainText, vector<string> roundKeysBinary, vector<string> roundKeysHex){
    // Hexadecimal to binary
    plainText = hex2bin(plainText);

    // Initial Permutation
    plainText = permute(plainText, initialPermutation, 64);
    // cout << "After initial permutation: " << bin2hex(plainText) << endl;
 
    // Splitting
    string left = plainText.substr(0, 32);
    string right = plainText.substr(32, 32);
    // cout << "After splitting: L0=" << bin2hex(left) << " R0=" << bin2hex(right) << endl;
    // cout << endl;

    // 16 DES rounds
    for (int i = 0; i < 16; i++) {
        DES_round(left, right, roundKeysBinary[i], roundKeysHex[i], i);
    }

    // last swapping
    swap(left, right);
 
    // Combination
    string combine = left + right;
 
    // Final Permutation
    string cipher = bin2hex(permute(combine, finalPermutation, 64));
    return cipher;
}
