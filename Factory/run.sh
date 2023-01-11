#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT="$(realpath $DIR/..)"

source "$PARENT/3rd/bash_framework/lib/oo-bootstrap.sh"

import util/type
import util/log

Log "Running with QEMU"

qemu-system-x86_64 \
    -kernel $PARENT/Build/kernel/x86_64/arch/x86_64/boot/bzImage \
	-m 1024 \
	-initrd $PARENT/Build/rootfs.cpio.gz \
	-append "earlyprintk=serial.ttys0 console=ttyS0 noapic nolapic acpi=off" \
	-nographic