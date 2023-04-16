#define HEADER_PATH_A "test/aspect/defines/lib/a.h"

// Include path defined internally
#include HEADER_PATH_A

// Include path defined in from the outside in the Bazel target
#include HEADER_PATH_B

int useLibs() {
    return doA() + doB();
}
