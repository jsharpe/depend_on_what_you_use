cc_library(
    name = "in_file_defines",
    hdrs = ["in_file_defines.h"],
    deps = ["//test/aspect/defines/lib:a"],
)

cc_library(
    name = "defines_from_bazel_target",
    hdrs = ["defines_from_bazel_target.h"],
    copts = ["-DSOME_COPT 42"],
    defines = ["SOME_DEFINE"],
    local_defines = ["LOCAL_DEFINE"],
    deps = ["//test/aspect/defines/lib:a"],
)

cc_library(
    name = "transitive_defines_from_bazel_target",
    hdrs = ["transitive_defines_from_bazel_target.h"],
    deps = [
        "//test/aspect/defines/lib:a",
        "//test/aspect/defines/support:transitive_define",
    ],
)

cc_library(
    name = "include_path_from_define",
    srcs = ["include_path_from_define.cpp"],
    copts = ['-DHEADER_PATH_B=\\"test/aspect/defines/lib/b.h\\"'],
    deps = [
        "//test/aspect/defines/lib:a",
        "//test/aspect/defines/lib:b",
    ],
)
