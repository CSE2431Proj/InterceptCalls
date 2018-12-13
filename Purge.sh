#!/bin/bash

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
echo "│ Unload Kernel Module syscall │"
echo "└──────────────────────────────┘"
echo "# rmmod infection"
sudo rmmod infection
echo ""
echo "┌──────────────────────────────┐"
echo "│           dmesg              │"
echo "└──────────────────────────────┘"
echo "$ dmesg"
dmesg | tail -n 9
