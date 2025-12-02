#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <fstream>
#include <string>
#include <filesystem>
#include <Bits.cpp>
#include <random>

//Класс для тестирования функций подсчета битов
class TestCounts {
public:
	
	void static random_test(uint64_t num) {
		bool ok = true;		
		std::random_device rd;
		std::mt19937_64 gen(rd());  // 64-битная версия
		std::uniform_int_distribution<uint64_t> distrib;

		for (uint64_t i = 0; i < num; ++i) {
			if (!(i % 10000)) {//чтобы не слишком часто
				int percent = (i * 100) / num;
				std::cout << "\r[" << std::string(percent / 2, '=')
					<< std::string(50 - percent / 2, ' ') << "] "
					<< percent << "%" << std::flush;
			}
			uint64_t number = distrib(gen);
			uint64_t naiv = Bits::count_bits_naiv(number);
			uint64_t sub_one = Bits::count_bits_sub_one(number);
			uint64_t cashed = Bits::count_bits_cashed(number);
			uint64_t cashed_opt = Bits::count_bits_opt_cashed(number);
			if (naiv != sub_one || sub_one != cashed || cashed != cashed_opt) {
				std::cout << "diffrernce result in: " <<
					i << ", number " << number << ", naiv: " << naiv << ", sub_one: " << sub_one << ", cashed: " << cashed << "\n";
				ok = false;
			}
		}
		std::cout << "\rCompleted: " << num << " numbers ------------------------                         \n";
		if (ok) std::cout << "Test ok \n";
		else std::cout << "Test failed\n";
		std::cout << "\n-----------------------------------------\n\n";
	};

	void static edge_test() {
		const uint64_t test_numbers[] = {
			0,                    // все нули
			UINT64_MAX,           // все единицы
			0xAAAAAAAAAAAAAAAA,   // чередование 1010...
			0x5555555555555555,   // чередование 0101...
			0xFF00FF00FF00FF00,   // байтовые паттерны
			0x00FF00FF00FF00FF,
			0x0000000000000001,   // одна единица
			0x8000000000000000,   // одна единица в старшем бите
		};
		bool ok = true;
		srand(time(nullptr));
		for (uint64_t i = 0; i != 8; ++i) {			
			
			uint64_t naiv = Bits::count_bits_naiv(test_numbers[i]);
			uint64_t sub_one = Bits::count_bits_sub_one(test_numbers[i]);
			uint64_t cashed = Bits::count_bits_cashed(test_numbers[i]);
			if (naiv != sub_one || sub_one != cashed) {
				std::cout << "diffrernce result in: " <<
					i << ", number " << test_numbers[i] << ", naiv: " << naiv << ", sub_one: " << sub_one << ", cashed: " << cashed << "\n";
				ok = false;
			}
		}		
		if (ok) std::cout << "Test ok \n";
		else std::cout << "Test failed\n";
		std::cout << "\n-----------------------------------------\n\n";
	}

	uint64_t static time_test(uint64_t num, std::function<uint64_t(uint64_t)> func) {
		std::random_device rd;
		std::mt19937_64 gen(123456789);  // 64-битная версия!
		std::uniform_int_distribution<uint64_t> distrib;
		
		bool ok = true;
		//srand(123456789); //чтобы тесты запускались на одинаковой последовательности
		auto start = std::chrono::high_resolution_clock::now();
		
		for (uint64_t i = 0; i < num; ++i) {
			uint64_t number = distrib(gen);
			try {
				func(number);
			}
			catch (const std::exception& e) {
				std::cout << "Итерация: " << i << ", число " << number << ", ошибка: " << e.what();
			}
		};
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		if (ok) std::cout << "Test ok, затрачено времени " << duration << "us\n";
		else std::cout << "Test failed " << duration << "us\n";
		std::cout << "\n-----------------------------------------\n\n";
		return duration;
	};	
};

//Класс для тестирования решения шахматных задачек
class TestBitboard {

public:
	TestBitboard(std::function<Bits::moves(uint64_t)> run, std::string folder) : 
		test_run(run), folder(folder) {} // в конструкторе передаем тестируемую функцию и папку с тестами

	//основная функция тестирования
	void run()
	{
		namespace fs = std::filesystem;

		int iter = 0;
		std::cout << "\n------------ Start test *" << folder << "* -----------------\n\n";
		while (true) {
			std::string fileIn = getTestPath(iter, "in");
			std::string fileOut = getTestPath(iter, "out");

			if (!fs::exists(fileIn) || !fs::exists(fileOut)) break;

			std::string inputs = readFile(fileIn);
			std::vector<std::string>  expected = readFileLines(fileOut);			

			_run(iter, inputs, expected);
			++iter;
		}
		std::cout << "\n------------ End test -----------------\n\n";
	}
private: //служебные функции
	//собственно запуск функции
	void _run(int iter, std::string input, std::vector<std::string> expected) {
		Bits::moves result;
		uint64_t start_pos = std::stoull(input);
		uint64_t count_moves = std::stoull(expected[0]);
		uint64_t move_mask = std::stoull(expected[1]);

        std::ostringstream out_stream;
        out_stream << "Тест " << std::setw(2) << std::setfill('0') << iter;        

        try {
            result = test_run(start_pos); //запускаем нашу тестируемую функцию			
            // Проверяем результат
			if (result.count == count_moves && result.mask == move_mask) {
				out_stream << " OK! start position: " << start_pos << "; число возможных ходов: " << count_moves
					<< "; битовая маска ходов: " << move_mask << "\n";
			}
	        else {            
				out_stream << " ОШИБКА: " << " результат: " << result.count <<
					" ходов, ожидалось " << count_moves << "; битовая маска: " <<
					result.mask << ", ожидалась " << move_mask << "\n";
			};
        }
        catch (const std::exception& e) {
            out_stream << " НЕ ВЫПОЛНЕН: " << start_pos << " ошибка: " << e.what();
        }

        std::cout << out_stream.str() << std::endl;
	}

	//получение пути к файлу с тестами
	std::string getTestPath(int testNum, const std::string& extension) {
		return std::string(TEST_DIR) + "/" + folder + "/test." +
			std::to_string(testNum) + "." + extension;
	}

	// Чтение одной строки (для файлов с результатом)
	std::string readFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) throw std::runtime_error("Cannot open: " + filename);
		std::string content;
		std::getline(file, content);
		file.close();
		return content;
	}

	// Чтение всех строк (для входных файлов)
	std::vector<std::string> readFileLines(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) throw std::runtime_error("Cannot open: " + filename);

		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			if (!line.empty()) {
				lines.push_back(line);
			}
		}
		file.close();
		return lines;
	}

private:
	//функция для тестирования
	std::function<Bits::moves(uint64_t)> test_run;
	std::string folder;
};