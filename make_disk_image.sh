#!/bin/bash

rm -f target/disk.img
dd if=/dev/zero of=target/disk.img bs=1024 count=10240
printf 'o\nn\np\n1\n\n\nw\n' | fdisk target/disk.img
losetup -f target/disk.img

device=$(sudo fdisk -l | awk '{print$2}' | grep /dev/ |tail -1 | awk -F ':' '{print $1}')
devicep1=$(sudo fdisk -l | tail -1 | awk '{print$1}')
partprobe $device
mkfs.ext2 $devicep1
mount $devicep1 /mnt

grub-install --target=i386-pc --boot-directory=/mnt/boot $device
cp target/os-image /mnt/boot/
echo "menuentry 'my' {" > /mnt/boot/grub.cfg
echo "  set root='hd0,1'" >> /mnt/boot/grub.cfg
echo "  multiboot /boot/os-image" >> /mnt/boot/grub.cfg
echo "  boot" >> /mnt/boot/grub.cfg
echo "}" >> /mnt/boot/grub.cfg
umount /mnt
losetup -d $device
