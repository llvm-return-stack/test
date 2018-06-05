# Return Stack Test Suite

This is a small collection of C/C++ files to test return stack support.

Before executing the tests, make sure to export the following variables:

```
export SYSROOT=<path to where the custom LLVM compiler, GNU C library, and GCC are installed>
export CC=$SYSROOT/<path to clang compiler>
export CXX=$SYSROOT/<path to clang++ compiler>
export LDSO_PATH=$SYSROOT/<path to ld.so>
export LIBC_PATH=$SYSROOT/<path to where libc.so.6 resides>
export LIBGCC_PATH=$SYSROOT/<path to where libgcc_s.so.1 resides>
```

Then, simply run:

```
cd test && ./run.sh
```

**Note** that only `CC` and `CXX` must be set in case the tests are executed
within a return stack-aware chroot or Yocto Linux environment (see
[here](https://github.com/llvm-return-stack/return-stack) for more details).
