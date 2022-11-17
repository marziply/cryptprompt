#!/bin/bash

warn_deprecated() {
  echo "The syntax 'root=${1}' is deprecated"
  echo "Use 'cryptdevice=${1}:root root=/dev/mapper/root' instead."
}

crypt_args() {
  for opt in ${1//,/ }; do
    case $opt in
      allow-discards|discard)
        args="${args} --allow-discards"
      ;;
      no-read-workqueue|perf-no_read_workqueue)
        args="${args} --perf-no_read_workqueue"
      ;;
      no-write-workqueue|perf-no_write_workqueue)
        args="${args} --perf-no_write_workqueue"
      ;;
      *)
        echo "Encryption option '${opt}' not known, ignoring." >&2
      ;;
    esac
  done

  echo -ne $args
}

run_prompt() {
  message="Password for $1 volume"
  command="/sbin/cryptsetup open $2 $1 $(crypt_args $3) $4"

  cryptprompt --message $message --command $command
}

run_hook() {
  modprobe -a -q dm-crypt >/dev/null 2>&1

  dev_default="${root}:root"

  if [ "$quiet" = "y" ]; then
    quiet_arg=" >/dev/null"
  fi

  if [ -z "$cryptdevice" ]; then
    dev_opt_missing=1
  fi

  IFS=: read dev_uuid dev_name dev_opts <<< ${cryptdevice:-$dev_default}

  if dev_path=$(resolve_device $dev_uuid $rootdelay); then
    if cryptsetup isLuks ${dev_path} >/dev/null 2>&1; then
      if [ $dev_opt_missing ]; then
        warn_deprecated $root
      fi

      echo
      echo "A password is required to access the ${dev_name} volume:"

      run_prompt $dev_name $dev_path $dev_opts $quiet_arg

      sleep 2

      if [ -e "/dev/mapper/${dev_name}" ]; then
        if [ $dev_opt_missing ]; then
          export root="/dev/mapper/root"
        fi
      else
        err "Password succeeded, but ${dev_name} creation failed, aborting..."

        exit 1
      fi
    fi
  fi
}
