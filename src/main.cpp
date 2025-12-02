#include <iostream>
#include "Bits.cpp"
#include "test.cpp"
#include <chrono>
#include <functional>
#include <cstdlib>

int main() {
	setlocale(LC_ALL, "Russian");

	//1. Проверим сначала подсчет битов	
	std::cout << "\nТестирование функций подсчета битов \n";	

	constexpr uint64_t num = 10000000; //количество чисел для проверки
	//1.а. Просто сверим результаты 3 функций (предположим, что если все функции выдают одинаковый результат, все Ok)
	std::cout << "\nТестируем корректность работы функций на " << num << " числах\n";
	TestCounts::random_test(num);
	std::cout << "\nТестируем корректность работы функций на отдельных пограничных случаях\n";
	TestCounts::edge_test();	

	//1.б. Замерим скорость каждой функции
	std::cout << "\nТестируем время работы функций на " << num << " числах\n";
	std::cout << "\n a. Наивная реализация \n";
	TestCounts::time_test(num, Bits::count_bits_naiv);
	std::cout << "\n b. Подсчет через отнимание единички \n";
	TestCounts::time_test(num, Bits::count_bits_sub_one);
	std::cout << "\n c. Через предподсчет \n";
	TestCounts::time_test(num, Bits::count_bits_cashed);
	std::cout << "\n c. Через предподсчет с оптимизацией\n";
	TestCounts::time_test(num, Bits::count_bits_opt_cashed);
	
	std::cout << "------------------------------------------\n";
	//2. Шахматные задачки
	std::cout << "\nТесты решений шахматных задачек \n";
	std::string testFolder = "1.Bitboard - King";
	TestBitboard kingTest(Bits::king, testFolder);
	kingTest.run();

	testFolder = "2.Bitboard - Knight";
	TestBitboard knightTest(Bits::knight, testFolder);
	knightTest.run();

	testFolder = "3.Bitboard - Rook";
	TestBitboard rookTest(Bits::rook, testFolder);
	rookTest.run();

	testFolder = "4.Bitboard - Bishop";
	TestBitboard bishopTest(Bits::bishop, testFolder);
	bishopTest.run();

	testFolder = "5.Bitboard - Queen";
	TestBitboard queenTest(Bits::queen, testFolder);
	queenTest.run();

	

	
	
	
	return 0;
}
