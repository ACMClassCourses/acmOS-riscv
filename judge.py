#!/usr/bin/env python

import re
from gradelib import *

r = Runner(save("acmOS-core.out"),
           stop_breakpoint("gdb_suspend"))


@test(0, "running acmOS")
def test_start():
    r.run_qemu(5)


@test(100, parent=test_start)
def test_kernel_print():
    file_name = open('acmOS-core.out', 'r').read()
    if 'Reaching Suspend point.' in file_name:
        print('Successfully start.')
        exit(0)
    else:
        exit(-1)


run_tests()
print('Successfully start.')
