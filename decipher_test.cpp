#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

void print(vector<int>& six_indicies, string& string)
{
  int count = 1;

  for (std::vector<int>::iterator pos = six_indicies.begin();
       pos < six_indicies.end()
       ; pos++)
  {
    std::cout << "Deciphered message version #" << count << std::endl;

    for (std::string::iterator it = string.begin(); it < string.end(); it++)
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
} // print() function

void calcLikelihoods(vector<int>& fc_p, vector<float>& ec_p, string &string,
                     vector<float>& square_p, vector<float>& total_square_p)
{
  for (int shift = 0; shift < 26; shift++) // each shift
  {

    for (int l_index = 0; l_index < 26; l_index++) // each letter in alphabet
    {
      fc_p.at(l_index) = 0;

      for (std::string::iterator it = string.begin(); it < string.end(); it++)
      {

        if ('A' + l_index + shift > 'Z') // goes past Z
        {

          if (std::toupper(*it) == 'A' + l_index + shift - 26)
          {
            fc_p.at(l_index)++; // counting letters
          } // not a letter
        } // doesn't need to be cycled
        else // does not go past Z
        {

          if (std::toupper(*it) == 'A' + l_index + shift)
          {
            fc_p.at(l_index)++; // counting letters
          } // not a letter
        } // else
      } // for length of string

      square_p.at(l_index) = ec_p.at(l_index) * (float)fc_p.at(l_index);
      total_square_p.at(26-shift) += square_p.at(l_index);
    } // for letter in alphabet
  } // for shift
} // calcLikelihoods() function

int main(int argc, const char * argv[])
{
  std::ifstream inf(argv[1]);
  std::stringstream buffer;
  buffer << inf.rdbuf();
  string str(buffer.str());
  inf.close();
  std::vector<int> fc(26, 0); // frequencies from argv[1]
  std::vector<float> ec(26, 0); // frequencies from Sean's file
  std::vector<float> square(26, 0); // multiplying frequencies
  std::vector<float> total_square(27, 0); // likelihood
  std::vector<float> copy(26, 0);
  std::vector<int> six_index(6, 0);

  std::ifstream freq("frequencies");

  for (int i = 0; i < 26; i++)
    freq >> ec.at(i);
  freq.close();
  calcLikelihoods(fc, ec, str, square, total_square);
  total_square.at(0) = total_square.at(26);
  total_square.erase(total_square.begin() + 26);
  copy = total_square;
  sort(copy.begin(), copy.end());
  
  
  for (int i = 0; i < 6; i++)
  {
    six_index.at(i) = (int)(find(total_square.begin(), total_square.end(),
                            copy.at(25-i)) - total_square.begin());
  } // for six
  

  print(six_index, str);

  return 0;
} // main
