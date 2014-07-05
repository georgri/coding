#include <iostream>
#include <vector>

using namespace std;

int main()
{
  int size;
  cin >> size;
 
 
  vector<unsigned int> a(size / 16); // <- input tab to encrypt
  vector<unsigned int> b(size / 16); // <- output tab
 
  for (int i = 0; i < size / 16; i++) {   // Read size / 16 integers to a
    cin >> hex >> a[i];
  }

  for (int i = 0; i < size / 16; i++) {   // Write size / 16 zeros to b
    b[i] = 0;
  }	
  
	// human-readable version of the conversion algorithm
	// Input: From integers to bools
	vector<bool> v1(size), v2(size);
	for (int i = 0; i < size; ++i) {
		v1[i] = 			(a[i / 32] >> (i % 32)) & 1;
		v2[i] = (a[i / 32 + size / 32] >> (i % 32)) & 1;
	}
	
	vector<bool> res(size * 2);
	for (int i = 0; i < size; ++i) {
		// if (v1[i]) {
			for (int j = 0; j < size; j++) {
				if (v2[j] && v1[i]) {
					res[i + j] = !res[i + j];
				}
			}
		// }
	}
	
	// Output: from bools to integers
	for (int i = 0; i < size * 2; ++i) {
		b[i / 32] |= (int)res[i] << (i % 32);
	}
 
  /* for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      b[(i + j) / 32] ^= ( (a[i / 32] >> (i % 32)) &
		       (a[j / 32 + size / 32] >> (j % 32)) & 1 ) << ((i + j) % 32);   // Magic centaurian operation
  */
  for(int i = 0; i < size / 16; i++)
    cout << hex << b[i] << " ";       // print result

 /* 
    Good luck humans     
 */
  return 0;
}