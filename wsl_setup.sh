sudo apt-get update && sudo apt-get upgrade

sudo apt-get install grub-pc-bin -y
sudo apt install gcc -y
sudo apt install make -y
sudo apt install binutils -y
sudo apt install build-essential -y
sudo apt install bison -y
sudo apt install flex -y
sudo apt install libgmp3-dev -y
sudo apt install libmpc-dev -y
sudo apt install libmpfr-dev -y
sudo apt install texinfo -y
sudo apt install libisl-dev -y
sudo apt install qemu-system -y

GCC_VERSION=$(gcc --version | cut -d ' ' -f 4 | head -n 1)
LD_VERSION=$(ld --version | cut -d ' ' -f 7 | head -n 1)
GCC_DIRECTORY=gcc-$GCC_VERSION
LD_DIRECTORY=binutils-$LD_VERSION

mkdir custom-compiler
cd custom-compiler

if ! [ -f $GCC_DIRECTORY.tar.gz ]; then
	wget "https://ftp.gnu.org/gnu/gcc/$GCC_DIRECTORY/$GCC_DIRECTORY.tar.gz"
fi

if ! [ -f $LD_DIRECTORY.tar.gz ]; then
	wget "https://ftp.gnu.org/gnu/binutils/$LD_DIRECTORY.tar.gz"
fi


tar -xvzf $GCC_DIRECTORY.tar.gz
tar -xvzf $LD_DIRECTORY.tar.gz

PREFIX="$HOME/opt/cross"
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"

mkdir src

cd src | {
  mkdir build-binutils
  cd build-binutils
  ../$LD_DIRECTORY/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
  make
  make install
}


# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc
cd build-gcc
../$GCC_DIRECTORY/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# Test
"$HOME"/opt/cross/bin/$TARGET-gcc --version
