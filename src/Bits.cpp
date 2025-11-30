#include <iostream>
#include <array>

class Bits {
public:
	Bits() = default;
	Bits(const Bits& other) = default;
	Bits(Bits&& other) = default;
	
	~Bits() = default;
	Bits&  operator= (const Bits&) = default;
	Bits& operator= (Bits&&) = default;			

public: //публичные константы 
	constexpr static uint64_t ONE = 1ULL;
	constexpr static uint64_t ZERO = 0ULL;
	constexpr static uint64_t BYTE_MASK = 0xFFULL;
	constexpr static uint64_t BIT(uint64_t n) { return 1ULL << n; };	

public: //структура для решения шахматных задачек
	struct moves {
		uint64_t mask; //битовая маска ходов
		uint64_t count; //число ходов
	};	

public:
	//временная - удалить!
	void static print_bitboard(uint64_t number) {
		for (uint64_t i = 8; i > 0; --i) {
			for (uint64_t j = 0; j < 8; ++j) {
				uint64_t idx = (i - 1) * 8 + j;
				uint64_t mask = ONE << idx;
				bool is_true = number & mask;
				std::cout << " " << is_true << " ";	
			};
			std::cout << "\n";
		};
	}
	//функции подсчета битов в числе
	//очевидная реализация подсчета числа битов в числе
	uint64_t static count_bits_naiv(uint64_t number) {
		uint64_t count = 0;
		while (number != 0) {
			count += (number & ONE);
			number >>= ONE;
		}
		return count;
	}
	//считаем отнимая единички
	uint64_t static count_bits_sub_one(uint64_t number) {
		uint64_t count = 0;
		while (number != 0) {
			
			number &= (number - ONE);
			++count;
		}
		return count;
	};

	//используем предрассчитанный массив
	uint64_t static count_bits_cashed(uint64_t number) {
		uint64_t count = 0;
		const uint64_t m = 0xFF;
		//разбиваем число на байты и считаем в каждом байте
		for (uint8_t i = 0; i != 8; ++i) {
			uint64_t m_byte = (number >> (i << 3)) & BYTE_MASK;
			count += ones[m_byte];
		};
		return count;
	};	

	//шахматные задачки
	
	moves knigth(uint64_t initial_pos) {
		moves result; //переменная для результата
		
		//начальная позиция
		uint64_t bits_pos = BIT(initial_pos);		

		//исключаемые зоны
		uint64_t to_right_1 = 0xFeFeFeFeFeFeFeFe; //при движении вправо на 1 клетку за доску
		uint64_t to_right_2 = 0xFcFcFcFcFcFcFcFc; //при движении вправо на 2 клетки за доску
		uint64_t  to_left_1 = 0x7f7f7f7f7f7f7f7f; //при движении влево на 1 клетку за доску
		uint64_t to_left_2 = 0x3f3f3f3f3f3f3f3f; //при движении влево на 2 клетки за доску

		result.mask = to_left_2 & (bits_pos << 6 | bits_pos >> 10)
			| to_left_1 & (bits_pos << 15 | bits_pos >> 17)
			| to_right_1 & (bits_pos << 17 | bits_pos >> 15)
			| to_right_2 & (bits_pos << 10 | bits_pos >> 6);


	};

	/*
	void king(std::string s_pos) {
		int point;
		s = s.ToUpper();
		try {
			point = int.Parse(s);
		}
		catch {
			try {
				point = (s[0] - 'A') + (s[1] - '1') * 8;
			}
			catch (Exception ex) {
				Console.SetCursorPosition(0, 10);
				Console.WriteLine(ex.Message);
				return;
			}
		}
		bb = (ulong)1 << point;
	}*/
//служебные функции
private:





private:
	//предрасчитанный массив для подсчета битов (расчет в момент компиляции)
	inline constexpr static std::array<int, 256> ones = []() {
		std::array<int, 256> result{};
		for (int i = 0; i < 256; ++i) {
			// Тот же цикл, как и в "наивной" реализации, но записан в одну строку
			result[i] = (i & 1) + ((i >> 1) & 1) + ((i >> 2) & 1) + ((i >> 3) & 1) +
				((i >> 4) & 1) + ((i >> 5) & 1) + ((i >> 6) & 1) + ((i >> 7) & 1);
		}
		return result;
		}();
};