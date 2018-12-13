#!/bin/bash

echo "┌──────────────────────────────┐"
echo "│  Load Kernel Module syscall  │"
echo "└──────────────────────────────┘"
echo "# insmod infection.ko"
sudo insmod ./lkm/infection.ko
echo ""
echo "┌──────────────────────────────┐"
echo "│           lsmod              │"
echo "└──────────────────────────────┘"
echo "$ lsmod | grep 'infection'"
lsmod | grep "infection"
echo ""
echo "┌──────────────────────────────┐"
echo "│           Test               │"
echo "└──────────────────────────────┘"
echo "$ ./test $1"
./test/test $1
echo ""
echo "┌──────────────────────────────┐"
echo "│           dmesg              │"
echo "└──────────────────────────────┘"
echo "$ dmesg"
dmesg | tail -n 9
