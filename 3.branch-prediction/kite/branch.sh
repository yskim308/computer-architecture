#!/bin/bash
############################################################
# Kite: Architecture Simulator for RISC-V Instruction Set  #
# Developed by William J. Song                             #
# Computer Architecture and Systems Lab, Yonsei University #
# Version: 1.12                                            #
############################################################

hw="Assignment 4"
rm_sh="tar.sh assembly.sh functions.sh"
patch_tar="branch.tar"
patch_path="https://casl.yonsei.ac.kr/teaching/eee3530"

install() {
    make clean
    rm -f *.h *.cc program_code reg_state mem_state Makefile $rm_sh
    wget $patch_path/$patch_tar
    tar xf $patch_tar
    rm -f $patch_tar
}

if [[ `basename $PWD` == "kite" && -d doc ]]; then
    if [[ ! `grep Branch program_code` =~ Branch ]]; then
        install
    else
        echo "Kite is already up to date for $hw"
        while true; do
            read -p "Do you want to update again? This will reset all the past work in kite [y/n]: " yn

            case $yn in
                [Yy]* )
                    install
                    break;;
                [Nn]* )
                    break;;
            esac
        done
    fi
    if [[ ! -f ~/.sid ]]; then
        read -p "Enter your 10-digit student ID: " sid
        echo "$sid" > ~/.sid
    fi
else
    echo "Error: $0 must run in the kite/ directory"
fi

