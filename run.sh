#!/bin/bash
set -xeuo pipefail

SOURCE=${BASH_SOURCE[0]}
while [ -L "$SOURCE" ]
do
  DIR=$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)
  SOURCE=$(readlink "$SOURCE")
  [[ $SOURCE != /* ]] && SOURCE=$DIR/$SOURCE
done
DIR=$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)

"$DIR"/build.sh --debug
qemu-system-x86_64 \
  -enable-kvm \
  -nographic \
  -no-reboot \
  -cpu host \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive file=fat:rw:.,format=raw \
  -machine q35 \
  -monitor none \
  -serial stdio
