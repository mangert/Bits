#include <iostream>
#include "Bits.cpp"

int main() {

	//1000000001000000001000000001000000001000000001000000001000000001
	//0001000000001000000001000000001000000001000000000000000000000000
	//9241421688590303745
	//1155177711073755136
		
	//Bits::print_bitboard(1155177711073755136);
	//uint64_t a = 9241421688590303745;
	//std::cout << a << std::endl;
	//std::cout << (a / 8) << std::endl;

	/*
	uint64_t num = 1024ULL;
	Bits::print_bitboard(num);

	std::cout << Bits::count_bits_naiv(num) << std::endl;
	std::cout << Bits::count_bits_sub_one(num) << std::endl;
	std::cout << Bits::count_bits_cashed(num) << std::endl;
	std::cout << "_______________" << std::endl;

	Bits::moves r = Bits::rook(34);
	Bits::print_bitboard(r.mask);
	std::cout << "_______________" << std::endl;
	*/
	Bits::moves b = Bits::king(32);
	Bits::print_bitboard(b.mask);
	
	
	return 0;
}
