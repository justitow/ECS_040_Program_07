//
//  main.cpp
//  ECS_040_Program_07
//
//  Created by Justin Weich on 2/25/16.
//  Copyright © 2016 Justin Weich. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <iterator>

using namespace std;

class Likelihoods
{
public:
  int shift;
  int likelihood;
  bool operator<(Likelihoods& rhs);
}; // Likelihoods class

bool Likelihoods::operator<(Likelihoods& rhs)
{
  if (likelihood < rhs.likelihood)
    return true;
  return false;
}

int main(int argc, const char * argv[]) {

  int c_count =0;

  vector<int> fc(26, 0);  // frequencies from encrypted message?
  vector<float> ec(26, 0); // frequencies from Sean's file?
  vector<float> chi(26, 0);
  vector<float> total_chi(27, 0);
  vector<float> square(26, 0);
  vector<float> total_square(27, 0);
  vector<Likelihoods> likelihoods(26);
	
  ifstream inf(argv[1]);
  stringstream buffer;
  buffer << inf.rdbuf();
  string str(buffer.str());
  inf.close();
  // store entire file into a single string
  //cout << str << endl;

  for (string::iterator it = str.begin(); it != str.end(); it++)
  {
    if (toupper(*it) >= 'A' && toupper(*it) <= 'Z')
      c_count++;
  }
	
  
  ifstream freq("frequencies");
  for (int i = 0; i < 26; i++)
  {
    freq >> ec.at(i);
    ec.at(i) = (float)ec.at(i)/1000.0*(float)c_count; // what is this?
  }
  freq.close();
  
  for (int i = 0; i < 26; i++)
    cout << ec.at(i) << ' ';
  
  
	/*
  ifstream freqFile("frequencies");
  istream_iterator<int> freqStart(freqFile), freqEnd;
  vector<int> freqVec(freqStart, freqEnd);
  // read frequencies from file into vector
	*/

  for (int shift = 0; shift < 26; shift++) // shift amount
  {
    for (int l_index = 0; l_index < 26; l_index++) // each letter
    // does this for loop count the frequencies of each letter in the file? i think count() is more efficient
    {
      fc.at(l_index) = 0;
      for (string::iterator it = str.begin(); it < str.end(); it++)
      {
        if ('A' + l_index + shift > 'Z')
        {
          if (toupper(*it) == 'A' + l_index + shift - 26)
          {
            fc.at(l_index)++;
          }
        }
        else
        {
          if (toupper(*it) == 'A' + l_index + shift)
          {
            fc.at(l_index)++;
          }
        }
      }
			
      chi.at(l_index) = powf(fc.at(l_index) - ec.at(l_index), 2.0)/(float)ec.at(l_index);
      total_chi.at(26-shift) += chi.at(l_index);

      square.at(l_index) = ec.at(l_index) * (float)fc.at(l_index);
      total_square.at(26-shift) += square.at(l_index);
    }
  }
	
	total_chi.at(0) = total_chi.at(26);
	total_square.at(0) = total_chi.at(26);
	
  for (int i = 0; i < 26; i++)
  {
    cout << "Message " << i + 1 << " Chi: " << total_chi.at(i) << " Square: " << total_square.at(i) <<  endl;
    for (string::iterator it = str.begin(); it < str.end(); it++)
    {
      if (toupper(*it) >= 'A' && toupper(*it) <= 'Z')
      {
        if (toupper(*it) + i > 'Z')
        {
          cout << (char)(*it + i - 26);
        }
        else
        {
          cout << (char)(*it + i);
        }
      }
      else
        cout << (char)(*it);
    }
  }
	
  return 0;
	
}
