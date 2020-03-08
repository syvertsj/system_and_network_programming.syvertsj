#!/usr/bin/env bash

prefix=./src/bin
   
install_data () {
    for i in $prefix/install_data/input_data.*; do 
        $prefix/install_data/install_data $i
        sleep 2;
    done
}
   
monitor_shm () {
    $prefix/monitor_shm/monitor_shm  
}
   
install_and_monitor () {
    default=30
    for i in $prefix/install_and_monitor/input_data.*; do 
        $prefix/install_and_monitor/install_and_monitor $i $default
        sleep 2
    done
}

case $1 in 
   install_data)        install_data ;;
   monitor_shm)         monitor_shm ;;
   install_and_monitor) install_and_monitor ;;
   *) echo "usage: $0 [install_data|monitor_shm|install_and_monitor]"; exit 0 ;;
esac
