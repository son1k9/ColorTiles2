#include <string>
#include <functional>
#include "random.h"

namespace SeedGenerator {
	inline constexpr int seedLenght = 10;
	inline constexpr std::string_view seedSymbols("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	inline std::string getSeedString() {
		Random::seed();
		std::string result(seedLenght, 0);
		auto randchar = []() -> char {
			return seedSymbols[Random::geti(0, seedSymbols.length() - 1)];
			};
		std::generate_n(result.begin(), seedLenght, randchar);
		return result;
	}

	inline int strToSeed(const std::string& str) {
		return std::hash<std::string>{}(str);
	}

	inline int getSeed() {
		return strToSeed(getSeedString());
	}
}