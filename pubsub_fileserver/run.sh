#!/usr/bin/env bash

# syvertsj

proj_root=`pwd`
# network ip for macos below (likely need to replace if using different *nix system)
ip=$(ifconfig en0 | grep -w inet | awk '{print $2}')
PORT=7406
article=sample.data

confirm() { diff -q $1 $2 && echo "pub-sub success" || echo "pub-sub error"; }

verify() {
   ls -l $proj_root/Articles/$1
   ls -l $1
   sleep 1
   #diff -q $proj_root/$article $proj_root/Articles/$article
   confirm $proj_root/$article $proj_root/Articles/$article
}

tcp_socket() {
   $proj_root/bin/tcp_publisher & 
   tcp_pid=$?
   sleep 1
   $proj_root/bin/tcp_subscriber $article $ip
   sleep 3
   pgrep tcp_publisher && pkill tcp_publisher
   pgrep tcp_subscriber && pkill tcp_subscriber
   verify $article
}

unix_socket() {
   $proj_root/bin/unix_publisher  & 
   sleep 1
   $proj_root/bin/unix_subscriber $article
   sleep 3
   pgrep unix_publisher && pkill unix_publisher
   pgrep unix_subscriber && pkill unix_subscriber
   rm -f /tmp/port$PORT
   verify $article
}

case $1 in
   tcp)  tcp_socket  ;;
   unix) unix_socket ;;
   *)    echo "usage: $0 [tcp|unix]" ;;
esac

