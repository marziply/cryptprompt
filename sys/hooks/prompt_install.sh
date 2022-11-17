#!/bin/bash

build() {
  add_module dm-crypt

  if [[ $CRYPTO_MODULES ]]; then
    for mod in $CRYPTO_MODULES; do
      add_module "$mod"
    done
  else
    add_all_modules "/crypto/"
  fi

  add_binary "cryptsetup"
  add_binary "dmsetup"

  add_file "/usr/lib/udev/rules.d/10-dm.rules"
  add_file "/usr/lib/udev/rules.d/13-dm-disk.rules"
  add_file "/usr/lib/udev/rules.d/95-dm-notify.rules"
  add_file \
    "/usr/lib/initcpio/udev/11-dm-initramfs.rules" \
    "/usr/lib/udev/rules.d/11-dm-initramfs.rules"

  add_binary "/usr/lib/libgcc_s.so.1"

  add_runscript
}
