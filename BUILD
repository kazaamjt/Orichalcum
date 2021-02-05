load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

alias(
	name = "main",
	actual = ":Orihalcum"
)

cc_binary(
	name = "Orihalcum",
	srcs = ["src/main.cpp"],
	deps = [
		":lib",
	],
)

cc_library(
	name = "lib",
	hdrs = glob(["src/lib/*.hpp"]),
	srcs = glob(["src/lib/*.cpp"]),
)
