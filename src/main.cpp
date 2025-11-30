#include <iostream>
#include "Bits.cpp"

int main() {

	
	
	uint64_t num = 1024ULL;
	Bits::print_bitboard(num);

	std::cout << Bits::count_bits_naiv(num) << std::endl;
	std::cout << Bits::count_bits_sub_one(num) << std::endl;
	std::cout << Bits::count_bits_cashed(num) << std::endl;

	
	
	return 0;
}
