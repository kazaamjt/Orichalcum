#include "catch.hpp"

#include "src/lib/data_structures/BigInt.hpp"

TEST_CASE("BigInt Tests") {
	std::string value = "123456789";

	SECTION("BigInt encode and decode") {
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

		LibOrichalcum::BigInt f(123456789);
		REQUIRE(LibOrichalcum::to_string(f) == value);

		LibOrichalcum::BigInt g(-123456789);
		REQUIRE(LibOrichalcum::to_string(g) == "-" + value);

		LibOrichalcum::BigInt h(big_int);
		REQUIRE(LibOrichalcum::to_string(h) == value);

		LibOrichalcum::BigInt i = big_int;
		REQUIRE(LibOrichalcum::to_string(i) == value);

		int64_t int64_value = 123456789123456789;
		LibOrichalcum::BigInt j(int64_value);
		REQUIRE(LibOrichalcum::to_string(j) == "123456789123456789");
	}

	SECTION("BigInt addition, subtraction and unary negative") {
		LibOrichalcum::BigInt a(value);
		REQUIRE(LibOrichalcum::to_string(a + a) == "246913578");
		REQUIRE(LibOrichalcum::to_string(-a) == "-" + value);
		REQUIRE(LibOrichalcum::to_string(a - a) == "0");

		LibOrichalcum::BigInt b("120000000000");
		REQUIRE(LibOrichalcum::to_string(b + b) == "240000000000");
		REQUIRE(LibOrichalcum::to_string(b - a) == "119876543211");
		REQUIRE(LibOrichalcum::to_string(a - b) == "-119876543211");

		LibOrichalcum::BigInt c("999999999");
		REQUIRE(LibOrichalcum::to_string(c + c) == "1999999998");

		LibOrichalcum::BigInt no_reuse(0);
		no_reuse += a;
		REQUIRE(LibOrichalcum::to_string(no_reuse) == value);
		no_reuse += a;
		REQUIRE(LibOrichalcum::to_string(no_reuse) == "246913578");
		no_reuse -= a;
		REQUIRE(LibOrichalcum::to_string(no_reuse) == value);
		no_reuse -= a;
		REQUIRE(LibOrichalcum::to_string(no_reuse) == "0");

		LibOrichalcum::BigInt d("-123456789");
		REQUIRE(LibOrichalcum::to_string(d + d) == "-246913578");
	}

	SECTION("BigInt multiplication") {
		LibOrichalcum::BigInt test_rebase(value);
		test_rebase.rebase(1);
		REQUIRE(LibOrichalcum::to_string(test_rebase) == (value + "000000000"));

		LibOrichalcum::BigInt a(value);
		REQUIRE(LibOrichalcum::to_string(a * a) == "15241578750190521");

		LibOrichalcum::BigInt b(1024);
		REQUIRE(LibOrichalcum::to_string(b * b * b * b * b * b) == "1152921504606846976");

		LibOrichalcum::BigInt c(1);
		REQUIRE(LibOrichalcum::to_string(a * c) == value);

		LibOrichalcum::BigInt d(100);
		REQUIRE(LibOrichalcum::to_string(a * d) == (value + "00"));

		LibOrichalcum::BigInt no_reuse(1);
		REQUIRE(LibOrichalcum::to_string(no_reuse *= a) == LibOrichalcum::to_string(a));
		REQUIRE(LibOrichalcum::to_string(no_reuse *= a) == LibOrichalcum::to_string(a * a));
		REQUIRE(LibOrichalcum::to_string(no_reuse *= a) == LibOrichalcum::to_string(a * a * a));
	}
}
