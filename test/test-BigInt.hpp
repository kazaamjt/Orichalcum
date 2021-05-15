#include "catch.hpp"

#include "src/lib/data_structures/BigInt.hpp"

TEST_CASE("BigInt Tests") {
	std::string value = "123456789";

	SECTION("Bigint encode and decode") {
		LibOrichalcum::BigInt big_int(value);
		REQUIRE(LibOrichalcum::to_string(big_int) == value);
		REQUIRE(LibOrichalcum::to_string(-big_int) == "-" + value);

		LibOrichalcum::BigInt zero("0");
		REQUIRE(LibOrichalcum::to_string(zero) == "0");
		REQUIRE(LibOrichalcum::to_string(-zero) == "0");

		LibOrichalcum::BigInt a("-12");
		REQUIRE(LibOrichalcum::to_string(a) == "-12");
		REQUIRE(LibOrichalcum::to_string(-a) == "12");

		LibOrichalcum::BigInt b("12000000000000000000000000000000");
		REQUIRE(LibOrichalcum::to_string(b) == "12000000000000000000000000000000");

		LibOrichalcum::BigInt c("-12000000000000000000000000000000");
		REQUIRE(LibOrichalcum::to_string(c) == "-12000000000000000000000000000000");

		LibOrichalcum::BigInt d("-122465224732547235473256475372457325472354234753");
		REQUIRE(LibOrichalcum::to_string(d) == "-122465224732547235473256475372457325472354234753");

		LibOrichalcum::BigInt e("122465224732547235473256475372457325472354234753");
		REQUIRE(LibOrichalcum::to_string(e) == "122465224732547235473256475372457325472354234753");
	}
}
