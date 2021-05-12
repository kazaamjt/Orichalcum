#include "catch.hpp"

#include "src/lib/data_structures.hpp"

TEST_CASE("BigInt Tests") {
	std::string value = "123456789";

	SECTION("Bigint encode and decode") {
		LibOrichalcum::BigInt big_int(value);
		REQUIRE(LibOrichalcum::to_string(big_int) == value);
	}

	SECTION("Bigint Math") {
		LibOrichalcum::BigInt a(value);
		LibOrichalcum::BigInt b(value);
		std::string result = "246913578";

		REQUIRE(LibOrichalcum::to_string(a + b) == result);
		REQUIRE(LibOrichalcum::to_string(a - b) == "0");

		a += b;
		REQUIRE(LibOrichalcum::to_string(a) == result);

		a -= b;
		REQUIRE(LibOrichalcum::to_string(a) == value);
	}
}
