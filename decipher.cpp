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


int main(int argc, const char * argv[]) {

	int c_count =0;

	std::vector<int> fc(26, 0);
	std::vector<float> ec(26, 0);
	std::vector<float> scaled_ec(26, 0);
	std::vector<float> chi(26, 0);
	std::vector<float> total_chi(27, 0);
	std::vector<float> square(26, 0);
	std::vector<float> total_square(27, 0);
	std::vector<float> copy(26,0);
	std::vector<int> six_index(6, 0);
	
	std::ifstream inf(argv[1]);
	std::stringstream buffer;
	buffer << inf.rdbuf();
	std::string str(buffer.str());
	inf.close();
	
	for (std::string::iterator it = str.begin(); it < str.end(); it++)
	{
		if (std::toupper(*it) >= 'A' && std::toupper(*it) <= 'Z')
			c_count++;
	}
	
	std::ifstream freq("/Users/justin/Developer/ECS_040_Program_07/ECS_040_Program_07/frequencies");
	for (int i = 0; i < 26; i++)
	{
		freq >> ec.at(i);
		scaled_ec.at(i) = (float)ec.at(i)/1000.0*(float)c_count;
	}
	freq.close();
	
	

	for (int shift = 0; shift < 26; shift++)
	{
		for (int l_index = 0; l_index < 26; l_index++) //each letter
		{
			fc.at(l_index) = 0;
			for (std::string::iterator it = str.begin(); it < str.end(); it++)
			{
				if ('A' + l_index + shift > 'Z')
				{
					if (std::toupper(*it) == 'A' + l_index + shift - 26)
					{
						fc.at(l_index)++;
					}
				}
				else
				{
					if (std::toupper(*it) == 'A' + l_index + shift)
					{
						fc.at(l_index)++;
					}
				}
			}
			
			
			chi.at(l_index) = powf(fc.at(l_index) - scaled_ec.at(l_index), 2.0)/(float)scaled_ec.at(l_index);
			total_chi.at(26-shift) += chi.at(l_index);

			square.at(l_index) = ec.at(l_index) * (float)fc.at(l_index);
			total_square.at(26-shift) += square.at(l_index);
		}
	}
	total_chi.at(0) = total_chi.at(26);
	total_square.at(0) = total_chi.at(26);
	total_chi.erase(total_chi.begin() + 25);
	total_square.erase(total_square.begin() + 25);
	
	copy = total_square;
	std::sort(copy.begin(), copy.end());
	for (int i = 0; i < 6; i++)
	{
		six_index.at(i) = (int)(find(total_square.begin(), total_square.end(), copy.at(25 - i)) - total_square.begin());
		//std::cout << pos << " ";
	}
	
	int count = 1;
	for (std::vector<int>::iterator pos = six_index.begin(); pos < six_index.end(); pos++)
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
				}
				else
				{
					std::cout << (char)(*it + *pos );
				}
			}
			else
				std::cout << (char)(*it);
		}
		std::cout << std::endl;
		count++;
	}
	
	return 0;
	
}
