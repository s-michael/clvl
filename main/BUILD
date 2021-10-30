cc_library(
    name = "ast",
    hdrs = ["ast.hpp", "ast_adapted.hpp"],
    deps = [
        "@boost//:fusion",
    ],
)

cc_binary(
    name = "clvl",
    srcs = ["main.cpp"],
    deps = [
        ":ast",
        "@boost//:filesystem",
        "@boost//:spirit",
        "@boost//:iostreams",
        "@boost//:variant",
        "@boost//:format",
        "@boost//:program_options",
        "@boost//:fusion",
    ],
)
