#include <iostream>
#include<string>


std::string decode(int digits) {
	const std::string lut[] =
	{ {108,101,116,32,117},
	{110,101,118,101,114},
	{105,118,101,32,121},
	{32,100,111,119,110},
	{104,117,114,116,32},
	{111,117,32,117,112},
	{97,114,111,117,110},
	{32,103,111,105,110},
	{32,116,111,32,103},
	{32,97,110,100,32} };

	std::string val = "";
	while (digits != 0) {
		int lsb = digits % 10;
		digits /= 10;
		val += lut[lsb];
	}
	return val;
}

int main() {
	// Replace with the sequence obtained in 
	// your analysis
	int watchme_digits = 00000; 

	std::cout<<decode(watchme_digits)<<std::endl;
}







