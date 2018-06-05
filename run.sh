#!/bin/bash

if [ -z "$CC" ]; then
  CC=$(which clang)
  if [ -z "$CC" ]; then
    echo "Clang compiler not found."
    exit
  fi
fi

if [ -z "$CXX" ]; then
  CXX=$(which clang++)
  if [ -z "$CXX" ]; then
    echo "Clang++ compiler not found."
    exit
  fi
fi

if [ -n "$SYSROOT" ]; then
  SYSROOT_FLAG="--sysroot=$SYSROOT"
fi

if [ -n "$LDSO_PATH" ]; then
  LINKER_FLAG="-Wl,--dynamic-linker=$LDSO_PATH"
fi

LIBRARY_PATH="."

if [ -n "$LIBC_PATH" ]; then
  LIBRARY_PATH="$LIBRARY_PATH:$LIBC_PATH"
fi

if [ -n "$LIBGCC_PATH" ]; then
  LIBRARY_PATH="$LIBRARY_PATH:$LIBGCC_PATH"
fi

export LD_LIBRARY_PATH="$LIBRARY_PATH"

cmd() {
  "$@"
} &> /dev/null

build() {
  EXT="${1##*.}"
  if [ "$EXT" = "c" ]; then
    C="$CC"
  elif [ "$EXT" = "cpp" ]; then
    C="$CXX"
  fi
  cmd "$C" "$@" -fsanitize=return-stack "$SYSROOT_FLAG"
}

check() {
  echo -n "  Testing $1..."
  build "$@" "$LINKER_FLAG" -fuse-ld=gold -o bin
  if [ "$?" -eq "0" ]; then
    cmd ./bin
    if [ "$?" -eq "0" ]; then
      echo "OK"
    else
      echo "FAILED"
    fi
  else
    echo "BUILDERROR"
  fi
  rm -f bin
}

echo "Basic functionality tests"
check args.c -O0
check basic.c -O0 # Requires -O0
check exception.cpp -O1
check setjmp.c -O1
check tailcall.c -O1 # Requires -O1
check thread.c -O1 -lpthread
check varargs.c -O0
echo ""
echo "Static library test"
build libsum.c -c -O1 -o libsum.a
check link.c -static -O1 -L. -lsum
rm -f libsum.a
echo ""
echo "Dynamic library test"
build libsum.c -O1 -fPIC -shared -fuse-ld=gold -o libsum.so
check link.c -O1 -L. -lsum
rm -f libsum.so

