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
	//про коня
	moves static knight(uint64_t initial_pos) {
		if (initial_pos > 63)
			throw std::out_of_range("Position must be between 0 and 63");
		moves result;
		uint64_t knight_bit = BIT(initial_pos);

		// Маски для исключения выходов за границы доски
		uint64_t no_right_1 = 0xFeFeFeFeFeFeFeFe;  // Без правого края
		uint64_t no_right_2 = 0xFcFcFcFcFcFcFcFc;  // Без 2 правых краев
		uint64_t no_left_1 = 0x7f7f7f7f7f7f7f7f;  // Без левого края  
		uint64_t no_left_2 = 0x3f3f3f3f3f3f3f3f;  // Без 2 левых краев

		result.mask = no_left_2 & (knight_bit << 6 | knight_bit >> 10)
			| no_left_1 & (knight_bit << 15 | knight_bit >> 17)
			| no_right_1 & (knight_bit << 17 | knight_bit >> 15)
			| no_right_2 & (knight_bit << 10 | knight_bit >> 6);

		result.count = count_bits_cashed(result.mask);
		return result;
	};

	//про короля
	moves static king(uint64_t initial_pos) {
		if (initial_pos > 63)
			throw std::out_of_range("Position must be between 0 and 63");
		
		moves result;		
		uint64_t king_bit = BIT(initial_pos);

		// Маски для исключения выходов за границы доски
		uint64_t no_right_edge = 0xfefefefefefefefe;  // Без правого края		
		uint64_t no_left_edge = 0x7f7f7f7f7f7f7f7f;  // Без левого края  
		
		result.mask = (no_left_edge & (king_bit << 7 | king_bit >> 1 | king_bit >> 9)) |
			(king_bit << 8 | king_bit >> 8) |
			(no_right_edge & (king_bit << 9 | king_bit << 1 | king_bit >> 7));
		
		result.count = count_bits_cashed(result.mask);
		return result;
	};

	//про ладью
	moves static rook(uint64_t initial_pos) {
		if (initial_pos > 63)
			throw std::out_of_range("Position must be between 0 and 63");

		moves result;
		uint64_t rook_bit = BIT(initial_pos);

		int row = initial_pos / 8;    // Номер строки (0-7)
		int col = initial_pos % 8;    // Номер столбца (0-7)

		// Вертикаль: все биты столбца col
		uint64_t vertical = 0x0101010101010101ULL << col;
		// Горизонталь: все биты строки row  
		uint64_t horizontal = 0xFFULL << (row * 8);

		result.mask = (vertical | horizontal) ^ rook_bit;
		result.count = count_bits_cashed(result.mask);
		return result;
	};

	//про слона
	moves static bishop(uint64_t initial_pos) {
		if (initial_pos > 63) 
			throw std::out_of_range("Position must be between 0 and 63");
		
		moves result;
		uint64_t bishop_bit = BIT(initial_pos);

		int row = initial_pos / 8;    // Номер строки (0-7)
		int col = initial_pos % 8;    // Номер столбца (0-7)

		// Главная диагональ
		uint64_t main_diag = 0x8040201008040201;
		//сдвигаем главную диагональ на разницу, масштабированную на высоту доски
		main_diag = (row < col) ?
			main_diag >> ((col - row) * 8) :
			main_diag << ((row - col) * 8);	
		
		// Побочная диагональ
		uint64_t anti_diag = 0x102040810204080;
		//сдвигаем главную диагональ на разницу, масштабированную на высоту доски
		int rev_col = 7 - col; //разницу берем от конца
		anti_diag = (row < rev_col) ?
			anti_diag >> ((rev_col - row) * 8):
			anti_diag << ((row - rev_col) * 8);		
		
		result.mask = (main_diag | anti_diag) ^ bishop_bit;		
		result.count = count_bits_cashed(result.mask);
		return result;
	};

	//про ферзя
	moves static queen(uint64_t initial_pos) {		
		
		if (initial_pos > 63)
			throw std::out_of_range("Position must be between 0 and 63");
		
		uint64_t rook_mask = rook(initial_pos).mask;
		uint64_t bishop_mask = bishop(initial_pos).mask;
		
		moves result;
		result.mask = rook_mask | bishop_mask;
		result.count = count_bits_cashed(result.mask);
		return result;
	};


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