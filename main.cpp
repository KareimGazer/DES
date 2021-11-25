#include "globals.hh"
#include "des.hh"

int main(int argc, char *argv[]) {
  string plainText, key, condition;

  condition = argv[1];
  plainText = argv[2];
  key = argv[3];

  vector<string> roundKeysBinary;
  vector<string> roundKeysHex;

  genRoundKeys(key, roundKeysBinary, roundKeysHex);

  if(condition == "encrypt"){
    string cipher = encrypt(plainText, roundKeysBinary, roundKeysHex);
    cout << "Cipher: " << cipher << endl;
  }
  else if(condition == "decrypt"){
    reverse(roundKeysBinary.begin(), roundKeysBinary.end());
    reverse(roundKeysHex.begin(), roundKeysHex.end());
    string text = encrypt(plainText, roundKeysBinary, roundKeysHex);
    cout << "Plain: " << text << endl;
  }
  else{
    cout << "error: please type encrypt or decrypt as first argument" << endl;
  }

  return 0;
}
