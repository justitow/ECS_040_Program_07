//
//  main.cpp
//  ECS_040_Program_07
//
//  Created by Justin Weich on 2/25/16.
//  Copyright © 2016 Justin Weich. All rights reserved.
//

#include <iostream>
#include <vector>


int main(int argc, const char * argv[]) {
	std::cout << "CYPHER:" << std::endl;
	char* str = new char[1000];
	std::vector<int> fc(26, 0);
	int mov = 0;
	
	std::cin >> str;

	for (int i = 0; i < 26; i++, str -= mov)
		for (mov = 0; *str != '\0'; str++, mov++)
			if (*str == 'a' + i)
				fc.at(i) += 1;
	
	return 0;
	
}
