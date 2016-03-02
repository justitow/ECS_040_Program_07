#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

int main(int argc, const char * argv[])
{
  int c_count = 0;
  std::ifstream inf(argv[1]);
  std::stringstream buffer;
  buffer << inf.rdbuf();
  string str(buffer.str());
  inf.close();
  std::vector<int> fc(26, 0); // frequencies from argv[1]
  std::vector<float> ec(26, 0); // frequencies from Sean's file
  //std::vector<float> scaled_ec(26, 0);
  //std::vector<float> chi(26, 0);
  //std::vector<float> total_chi(27, 0);
  std::vector<float> square(26, 0); // multiplying frequencies
  std::vector<float> total_square(27, 0); // likelihood
  std::vector<float> copy(26, 0);
  std::vector<int> six_index(6, 0);

  for (int i = 0; i < (int)str.length(); i++)
  {
    if (std::toupper(str.at(i)) >= 'A' && std::toupper(str.at(i)) <= 'Z')
      c_count++;
  } // for

  std::ifstream freq("frequencies");

  for (int i = 0; i < 26; i++)
  {
    freq >> ec.at(i);
    //scaled_ec.at(i) = (float)ec.at(i) / 1000.0 * (float)c_count;
  } // for

  freq.close();

  for (int shift = 0; shift < 26; shift++) // each shift
  {

    for (int l_index = 0; l_index < 26; l_index++) // each letter in alphabet
    {
      fc.at(l_index) = 0;

      for (std::string::iterator it = str.begin(); it < str.end(); it++) // each char in string
      {

        if ('A' + l_index + shift > 'Z') // goes past Z
        {

          if (std::toupper(*it) == 'A' + l_index + shift - 26)
          {
            fc.at(l_index)++; // counting letters
          } // not a letter
        } // doesn't need to be cycled
        else // does not go past Z
        {

          if (std::toupper(*it) == 'A' + l_index + shift)
          {
            fc.at(l_index)++; // counting letters
          } // not a letter
        } // else
      } // for length of string


      //chi.at(l_index) = powf(fc.at(l_index) - scaled_ec.at(l_index), 2.0) /
      //(float)scaled_ec.at(l_index);
      //total_chi.at(26-shift) += chi.at(l_index);

      square.at(l_index) = ec.at(l_index) * (float)fc.at(l_index);
      total_square.at(26-shift) += square.at(l_index);
    } // for letter in alphabet
  } // for shift

  //total_chi.at(0) = total_chi.at(26);
  //total_chi.erase(total_chi.begin() + 25);
  //total_square.at(0) = total_chi.at(26);
  total_square.erase(total_square.begin() + 25);

  copy = total_square;
  sort(copy.begin(), copy.end());

  for (int i = 0; i < 6; i++)
  {
    six_index.at(i) = (int)(find(total_square.begin(), total_square.end(),
                            copy.at(25-i)) - total_square.begin());
    //std::cout << pos << " ";
  } // for six

  int count = 1;

  for (std::vector<int>::iterator pos = six_index.begin(); pos < six_index.end()
       ; pos++)
  {
    //std::cout << "shift " << *pos << " Chi: " << total_chi.at(*pos) << " Square: " << total_square.at(*pos) <<  std::endl;
    std::cout << "Deciphered message version #" << count << std::endl;

    for (std::string::iterator it = str.begin(); it < str.end(); it++)
    {

      if (std::toupper(*it) >= 'A' && std::toupper(*it) <= 'Z')
      {

        if (std::toupper(*it) + *pos > 'Z')
        {
          std::cout << (char)(*it + *pos - 26);
        } // i don't

        else // this happened
        {
          std::cout << (char)(*it + *pos );
        } // even know
      } // what
      else // what am I doing with my life?
        std::cout << (char)(*it);
    } // 

    std::cout << std::endl;
    count++;
  } // for length of string

  return 0;
} // main