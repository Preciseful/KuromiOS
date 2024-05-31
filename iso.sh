#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/KuromiOS.kernel isodir/boot/KuromiOS.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "KuromiOS" {
	multiboot /boot/KuromiOS.kernel
}
EOF
grub-mkrescue -o KuromiOS.iso isodir
