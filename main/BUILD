cc_library(
    name = "ast",
    hdrs = ["ast.hpp", "ast_adapted.hpp"],
)

cc_binary(
    name = "clvl",
    srcs = ["main.cpp"],
    deps = [
        ":ast",
    ],
)
