# Documentation of noteworthy project design decisions <!-- omit in toc -->

- [General](#general)
  - [Why use Python](#why-use-python)
  - [Why use a multi step automatic fixes workflow](#why-use-a-multi-step-automatic-fixes-workflow)
- [Platforms](#platforms)
  - [Why is Bazel \< 4.0.0 not supported](#why-is-bazel--400-not-supported)
  - [Why is Python \< 3.6 not supported](#why-is-python--36-not-supported)
- [Rejected Concepts](#rejected-concepts)
  - [Includes parsing via .d files](#includes-parsing-via-d-files)

# General

## Why use Python

There are many programming languages available which could be used to implement DWYU. Why use Python because:

- It is well established tool many developers are familiar with.
- Most platforms support Python well and developer setups often have an interpreter pre-installed.
- There is a wide range of well established third-party libraries.
- There is no need to deploy pre compiled binaries for a wide range of platforms.
- It is well suited for scripting tasks and testing.
- The task done by DWYU does not require many resources. Thus, efficiency and performance are secondary.
- The project maintainer is experienced with Python.

## Why use a multi step automatic fixes workflow

Having to execute a separate tool to apply fixes seems bothersome. Ideally, DWYU would perform fixes
while analyzing the problems.<br/>
However, given DWYU is implemented as a Bazel aspect, there are limitations to what we can do in a single step:

- The DWYU aspect is analyzing the dependencies of the targets. Changing the dependencies while analyzing them would
  invalidate the dependency graph and require rebuilding the graph after each fix before continuing to
  analyze more targets. There is no standard feature of Bazel aspects allowing this.
- A Bazel aspect is executed in the sandbox. To be able to modify the BUILD files in the workspace, one would have to
  escape the sandbox. This is generally considered a bad practice when working with Bazel.

We circumvent the above problems by using a two step approach. First we discover all problems and store the result in
a machine readable format. Then, we use a separate tool to process the results and apply fixes to the BUILD files in
the workspace. There are no problems regarding the sandboxing, since we utilize `bazel run` to execute the fixing tool.
A tool being executed like this can access any part of the host system.

# Platforms

## Why is Bazel \< 5.0.0 not supported

The DWYU aspect is incompatible to Bazel \< 4.0.0.
Bazel 4.x is not tested, but should still work.

We officially support and test only Bazel 5.0.0 and later.
One major feature of DWYU is ensuring the proper usage of `implementation_deps`, which is a feature which is only
available starting from Bazel 5.0.0.
Furthermore, many Bazel rule sets already rely on Bazel 5.x as well.

## Why is Python \< 3.8 not supported

We aim to only support Python versions which are not EOL.
Using a modern Python version enables us to write lean code utilizing modern Python features.

# Rejected Concepts

## Includes parsing via .d files

Most modern compilers can generate `.d` files which document the headers required to compile a source file.
Essentially, this makes parsing of source code and aggregating the include statements with custom tooling superfluous.

One downside is, that this only works for source files, but not for header only code.
This could be mitigated by generating source files for the headers and then running the compiler on them.

A major drawback of this approach is however, that the `.d` files list all transitively included headers which are
required for compiling a source file.

For example, given the 3 target `A`, `B` and `C` with the files:

`a.h`

```c++
void doA() { ... };
```

`b.h`

```c++
#include "a.h"

void doB() { doA(); };
```

`c.cpp`

```c++
#include "b.h"

void doC() { doB(); };
```

The `.d` file for `c.cpp` will list the headers `a.h` and `b.h`.
This makes sense, after all the compiler requires all used headers to compile `c.cpp`.
However, it makes the `.d` file impractical for DWYU.
We need to know if header `a.h` was included directly in `c.cpp` or is used transitively by `b.h`.
Without this distinction we cannot compare the include statements to the list of direct dependencies.

At the time of writing this no way could be found to configure GCC to generate `.d` files matching our requirements.
