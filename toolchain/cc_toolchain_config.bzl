load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")

load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _imp(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/usr/local/bin/clang",
        ),
        tool_path(
           name = "ld",
           path = "/usr/bin/ld",
        ),
        tool_path(
           name = "cpp",
           path = "/usr/local/bin/clang++",
        ),
        tool_path(
           name = "nm",
           path = "/usr/bin/nm",
        ),
        tool_path(
           name = "ar",
           path = "/usr/bin/ar",
        ),
        tool_path(
           name = "gcov",
           path = "/usr/bin/gcov",
        ),
        tool_path(
           name = "strip",
           path = "/usr/bin/strip",
        ),
        tool_path(
           name = "objdump",
           path = "/usr/bin/objdump",
        ),
    ]

    features = [
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-fuse-ld=gold", 
                                "-lstdc++", 
                                "-lboost_filesystem", 
                                "-lboost_iostreams", 
                                "-lboost_program_options", 
                            ], 
                        ), 
                    ]),
                ),
            ]
        )
    ]


    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = [
            "/usr/local/lib64/clang/14.0.0/include",
            "/usr/local/lib/clang/14.0.0/include",
            "/usr/include",
            "/usr/local/include",
        ],
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8",
        target_libc = "unknown",
        compiler = "clang++",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _imp,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
