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
	std::vector<float> chi(26, 0);
	std::vector<float> total_chi(26, 0);
	std::vector<float> square(26, 0);
	std::vector<float> total_square(26, 0);
	
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
		//ec.at(i) = (float)ec.at(i)/1000.0*(float)c_count;
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
			
			chi.at(l_index) = powf(fc.at(l_index) - ec.at(l_index), 2.0)/(float)ec.at(l_index);
			total_chi.at(shift) += chi.at(l_index);

			square.at(l_index) = ec.at(l_index) * (float)fc.at(l_index);
			total_square.at(shift) += square.at(l_index);
		}
	}
	
	for (int i = 0; i < 26; i++)
	{
		std::cout << "Message " << i + 1 << " Chi: " << total_chi.at(i) << " Square: " << total_square.at(i) <<  std::endl;
		for (std::string::iterator it = str.begin(); it < str.end(); it++)
		{
			if (std::toupper(*it) >= 'A' && std::toupper(*it) <= 'Z')
			{
				if (std::toupper(*it) + i > 'Z')
				{
					std::cout << (char)(*it + i - 26);
				}
				else
				{
					std::cout << (char)(*it + i );
				}
			}
			else
				std::cout << (char)(*it);
		}
	}
	
	return 0;
	
}
