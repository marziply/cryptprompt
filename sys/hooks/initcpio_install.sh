#!/bin/bash

build() {
  add_dir /dev/pts

	add_file /usr/lib/udev/rules.d/70-uaccess.rules
	add_file /usr/lib/udev/rules.d/71-seat.rules

	add_file /etc/passwd
	add_file /etc/nsswitch.conf
	add_binary "$(readlink -e /lib/libnss_files.so.2)"
	add_file /lib/libnss_files.so.2

	add_runscript
}
