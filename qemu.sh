#!/bin/sh
set -e
. ./iso.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done

touch lastpid
PID="$(cat './lastpid' | cut -d' ' -f1)"
if [ "$PID" != "" ] && ps -p $PID > /dev/null;  then
  kill "$PID"
fi

qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -m 1G -no-shutdown -no-reboot -cdrom KuromiOS.iso -serial file:log.log -daemonize
echo $(ps ax | grep "qemu-system-$(./target-triplet-to-arch.sh $HOST)" | grep 'KuromiOS') > "lastpid"