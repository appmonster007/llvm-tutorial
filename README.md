# llvm-tutorial #
Helpful guide towards learning to LLVM passes (legacy and new passes) for beginners
## At a glance ##
A step-by-step tutorial for building an out-of-source LLVM pass


## Setup ##

LLVM is an umbrella project for building compilers
and code transformation tools. It consists of several sub-projects like Clang,
LLD and, confusingly enough, the LLVM sub-project. We consider in this tutorial:
- Building the LLVM *sub-project* from source
- Building a trivial out-of-source LLVM pass.

We will be building LLVM v`12.0.1` which is the latest as of this writing.
We assume that you have a working compiler toolchain (GCC or LLVM) and that CMake is installed (minimum version 3.4).

### Current file structure ###
```
llvm-tutorial
├── LICENSE
├── README.md
└── src
    ├── CMakeLists.txt
    ├── CustomLegacyPass
    │   ├── CMakeLists.txt
    │   ├── MyLegacyPass.cpp
    │   └── MyLegacyPass.h
    ├── CustomNewPass
    │   ├── CMakeLists.txt
    │   ├── MyNewPass.cpp
    │   └── MyNewPass.h
    └── tmp
        ├── cfile.bc
        ├── cfile.c
        ├── cfile.ll
        ├── cppfile.bc
        ├── cppfile.cpp
        └── cppfile.ll

4 directories, 15 files
```

## Compiling LLVM ##
Compiling LLVM from source is mandatory if you are developing an in-source pass (within LLVM source tree).
It can also be convenient in the case of developing out-of-source passes as it gives you full control over the compilation options.
For example, a debug build of LLVM is much more pleasant to work with compared to an optimized one. To compile LLVM, please follow the following steps:

1. Download LLVM [source](http://llvm.org/releases/)
and unpack it in a directory of your choice which will refer to as `[LLVM_SRC]` or download from github repo [The LLVM Compiler Infrastructure
](https://github.com/llvm/llvm-project)

2. Create a separate build directory
    ```bash
    mkdir llvm-build
    cd llvm-build
    ```
3. Instruct CMake to detect and configure your build environment:

    ```bash
    cmake -DCMAKE_BUILD_TYPE=Debug -DLLVM_TARGETS_TO_BUILD=X86 [LLVM_SRC]
    ```
    Note that we instructed cmake to only build `X86` backend.
    You can choose a different backend if needed. If you do not specify `LLVM_TARGETS_TO_BUILD`,
    then all supported backends will be built by default which requires more time.

4. Now start the actual compilation within your build directory

    ```bash
    cmake --build .
    ```
    The `--build` option is a portable why to tell cmake to invoke the underlying
    build tool (make, ninja, xcodebuild, msbuild, etc.)

5. Building takes some time to finish. After that you can install LLVM in its default directory which is `/usr/local`
    ```bash
    cmake --build . --target install
    ```
    Alternatively, it's possible to set a different install directory `[LLVM_HOME]`.
    Since we will need `[LLVM_HOME]` in the next stage, we assume that you have defined
    it as an environment variable `$LLVM_HOME`. Now you can issue the following command
    ```bash
    cmake -DCMAKE_INSTALL_PREFIX=$LLVM_HOME -P cmake_install.cmake
    ```
    Note that `$LLVM_HOME` must __not__ contain `~` (tilde) to refer to your home directory
    as it won't be expanded. Use `$HOME` or an absolute path instead.


## Brew installation ##
Download header and pre-compiled binary files using Homebrew or Linuxbrew

1. Download and install llvm
    ```bash
    brew install llvm
    brew install llvm@12 # alternative
    brew upgrade llvm # for upgrade
    ```

2. Exporting lib and include flags
    ```bash
    export LDFLAGS="-L/usr/local/opt/llvm/lib"
    export CPPFLAGS="-I/usr/local/opt/llvm/include"
    ```

    [Optional] For those using VScode, include following path in c++ include path settings
    ```
    /usr/local/opt/llvm/include/**
    ```


## Building a trivial LLVM pass ##

To build the skeleton legacy LLVM pass found in `CustomLegacyPass` folder or,
to build the skeleton new LLVM pass found in `CustomNewPass` folder :

[Important] Enable or add respective pass folder in CMakeLists.txt

```bash
cd llvm-tutorial
mkdir build
cd build
cmake ../src/
make
```

Clean build
```bash
git clean -d -f -x # or
rm -rf build && mkdir build
```

`cmake` needs to find its LLVM configurations in `[LLVM_DIR]`. We automatically
setup `[LLVM_DIR]` based on `$LLVM_HOME` for you. Now the easiest way to run the skeleton pass is to use Clang:

[Legacy]
```bash
# C frontend
clang \
    -Xclang \
    -load -Xclang build/CustomLegacyPass/libCustomLegacyPass.so src/tmp/{file}.c 

# C++ frontend
clang++ \
    -Xclang \
    -load -Xclang build/CustomLegacyPass/libCustomLegacyPass.* src/tmp/{file}.cpp 
```
[New]
```bash
# C frontend
clang \
    -fexperimental-new-pass-manager \
    -fpass-plugin=build/CustomNewPass/libCustomNewPass.so src/tmp/{file}.c

# C++ frontend
clang++ \
    -fexperimental-new-pass-manager \
    -fpass-plugin=build/CustomNewPass/libCustomNewPass.so src/tmp/{file}.cpp
```

**NOTE**:
- Execution of any command is from root dir, unless specifically mentioned
- Clang is the compiler front-end of the LLVM project. It can be installed separately in binary form.

## Using opt [llvm optimiser] ##

### Building low level file (.ll) and bytecode file (.bc) ###

[.ll|.bc]
```bash
clang -emit-llvm -S src/tmp/{file}.c* # src/tmp/{file}.ll

clang -O1 -emit-llvm src/tmp/{file}.c* -c # src/tmp/{file}.bc
# -O* represent optimization level

cat src/tmp/{file}.ll
```

Executing [.ll|.bc] files
```bash
lli src/tmp/{file}[.ll|.bc]
```

We can also use the cc1 for generating IR:

```bash
clang \
    -cc1 \
    -emit-llvm src/tmp/{file}[.c|.cpp] \
    -o src/tmp/{file}.ll
```

### opt commands to load and enable custom pass for [.ll|.bc] file ###
[Legacy]
```bash
opt \
    -load build/CustomLegacyPass/libCustomLegacyPass.so \
    -"MyLegacyPass" -disable-output src/tmp/{file}.[bc|ll] 
    
# Alternative 
opt \
    -load build/CustomLegacyPass/libCustomLegacyPass.so \ 
    -MyLegacyPass < src/tmp/{file}.[bc|ll] > /dev/null 
```
[New]
```bash
opt \
    -load-pass-plugin=build/CustomNewPass/libCustomNewPass.so \
    -passes="MyNewPass" -disable-output src/tmp/{file}.[bc|ll]
```

To generate modified [.bc] file after optimization performed by plugged in pass, 
we add -S flag to get output and store it in new [.bc] file

```bash
opt \
    -S \
    -load-pass-plugin=build/CustomNewPass/libCustomNewPass.so \
    -passes="MyNewPass" src/tmp/{file_V1}.bc > src/tmp/{file_V2}.bc
```

To generate CFG files use -dot-cfg flag
```bash
opt -S -dot-cfg src/tmp/{file_V1}.bc > {file_V2}.bc # or
opt -S -dot-cfg src/tmp/{file_V1}.bc -o {file_V2}.bc
```

### Further resources ###
This tutorial is based on the following resources

- Adrian Sampson's blog entry "LLVM for Grad Students" ([link](http://adriansampson.net/blog/llvm.html))
- tomgu1991.github.io blog : Introduction to LLVM Pass ([link](http://tomgu1991.github.io/blog/Technical/LLVM/LLVM_Pass.html))
- LLVM docs (llvm.org doc)
    - My First Language Frontend with LLVM Tutorial  ([link](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html))
    - Writing an LLVM Pass  ([link](https://llvm.org/docs/WritingAnLLVMPass.html))
    - Writing an LLVM New Pass  ([link](https://llvm.org/docs/WritingAnLLVMNewPMPass.html))
    - Using New Pass Manager  ([link](https://llvm.org/docs/NewPassManager.html))
    - LLVM’s New Pass Manager  ([link](https://blog.llvm.org/posts/2021-03-26-the-new-pass-manager/))
    - LLVM documentation: Building LLVM with CMake  ([link](http://llvm.org/docs/CMake.html#cmake-out-of-source-pass))
    - Getting Started with the LLVM System  ([link](https://llvm.org/docs/GettingStarted.html))
    - Projects built with LLVM  ([link](https://llvm.org/ProjectsWithLLVM/))
- llvm: clang installed from linuxbrew is not able to link binary executable #22034 (github issue) ([link](https://github.com/Homebrew/linuxbrew-core/issues/22034))
- Installed LLVM doesn't get added to $PATH #29733 (github issue) ([link](https://github.com/Homebrew/legacy-homebrew/issues/29733))
- How to automatically register and load modern Pass in Clang? (so) ([link](https://stackoverflow.com/questions/54447985/how-to-automatically-register-and-load-modern-pass-in-clang))
- How to write your own compiler  ([link](https://staff.polito.it/silvano.rivoira/HowToWriteYourOwnCompiler.htm))


### Reference ###
- Adrian Sampson's "LLVM for Grad Students"  
[sampsyo/llvm-pass-skeleton](https://github.com/sampsyo/llvm-pass-skeleton) (github repo)
- [banach-space/llvm-tutor](https://github.com/banach-space/llvm-tutor) (github repo)
- LLVM [Documentation](llvm.org)
-  Writing an LLVM Pass: 101;  
    LLVM 2019 tutorial by Andrzej Warzyński