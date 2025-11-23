#include <cstddef>
#include <iostream>
#include <string.h>
#include <cstring>
#include <bitset>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
  std::vector< std::vector <float> > NTCvals;
  NTCvals.clear();
  std::vector<float> temp;
  
  float NTCarray[3]={0.,1.,2.};
  for(int i=0;i<3;i++) {
    for(int k=0;k<3;k++) {
      temp.push_back(1.);
      NTCvals.push_back(temp);
    }
  }

  for(int i=0;i<3;i++) {
    for(int k=0;k<3;k++) {
      cout<<NTCvals[i][k]<<endl;}}
  return 0;
}
