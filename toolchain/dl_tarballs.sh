#!/bin/sh

# List of tarballs
#    binutils-2.20.1a.tar.bz2
#    cloog-ppl-0.15.10.tar.gz
#    dmalloc-5.5.2.tgz
#    duma_2_5_15.tar.gz
#    expat-2.0.1.tar.gz
#    gcc-4.4.3.tar.bz2
#    gdb-6.8a.tar.bz2
#    glibc-2.9.tar.bz2
#    glibc-ports-2.9.tar.bz2
#    gmp-4.3.2.tar.bz2
#    libelf-0.8.13.tar.gz
#    ltrace_0.5.3.orig.tar.gz
#    mpfr-3.0.0.tar.bz2
#    ncurses-5.7.tar.gz
#    ppl-0.10.2.tar.gz
#    strace-4.5.19.tar.bz2

mkdir -p .build/tarballs
cd .build/tarballs
alias wgetncc='wget --no-check-certificate --no-clobber'
wgetncc https://ftp.gnu.org/gnu/binutils/binutils-2.20.1a.tar.bz2
wgetncc https://ftp.osuosl.org/pub/clfs/conglomeration/cloog-ppl/cloog-ppl-0.15.10.tar.gz
wgetncc https://dmalloc.com/releases/dmalloc-5.5.2.tgz
wgetncc https://fossies.org/linux/privat/duma_2_5_15.tar.gz
wgetncc https://ftp.osuosl.org/pub/blfs/conglomeration/expat/expat-2.0.1.tar.gz
wgetncc https://ftp.gnu.org/gnu/gcc/gcc-4.4.3/gcc-4.4.3.tar.bz2
wgetncc https://ftp.gnu.org/gnu/gdb/gdb-6.8a.tar.bz2
wgetncc https://ftp.gnu.org/gnu/libc/glibc-2.9.tar.bz2
wgetncc https://ftp.gnu.org/gnu/libc/glibc-ports-2.9.tar.bz2
wgetncc https://ftp.gnu.org/gnu/gmp/gmp-4.3.2.tar.bz2
wgetncc https://ftp.osuosl.org/pub/blfs/conglomeration/libelf/libelf-0.8.13.tar.gz
wgetncc http://distro.ibiblio.org/openwall/Owl/pool/sources/ltrace/ltrace_0.5.3.orig.tar.gz
wgetncc https://ftp.gnu.org/gnu/mpfr/mpfr-3.0.0.tar.bz2
wgetncc https://ftp.gnu.org/gnu/ncurses/ncurses-5.7.tar.gz
wgetncc https://gcc.gnu.org/pub/gcc/infrastructure/ppl-0.10.2.tar.gz
wgetncc https://strace.io/files/4.5.19/strace-4.5.19.tar.bz2