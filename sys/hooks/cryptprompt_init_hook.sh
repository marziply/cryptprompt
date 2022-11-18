#!/bin/bash

run_earlyhook(){
  # first trigger graphics subsystem
  udevadm trigger \
    --action add \
    --attr-match=class 0x030000 \
    >/dev/null 2>&1

  # first trigger graphics and tty subsystem
  udevadm trigger \
    --action add \
    --subsystem-match graphics \
    --subsystem-match drm \
    --subsystem-match tty \
    >/dev/null 2>&1

  udevadm settle --timeout=30 2>&1

  /usr/bin/mknod /dev/fb c 29 &>/dev/null
  /usr/bin/mkdir -p /dev/pts
  /usr/bin/mount \
    -t devpts \
    -o noexec,nosuid,gid=5,mode=0620 \
    devpts \
    /dev/pts \
    || true
}

run_hook() {
  #
}

run_latehook(){
  #
}
