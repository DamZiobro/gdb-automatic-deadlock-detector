#! /bin/bash
#
# displayDeadlock.sh
# Copyright (C) 2017 Damian Ziobro <damian@xmementoit.com>

gdb -c core ./deadlockExample -x gdbcommands -batch

