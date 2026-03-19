# tme_msr

Provides a UEFI PE used to measure firmware enablement of Intel TME.

## Build

Run `./build.sh` to build the UEFI PE for production.

Pass `--debug` to build a debug variant with a simulated MSR value.

## Run

Run `./run.sh` to run the UEFI PE in QEMU.
