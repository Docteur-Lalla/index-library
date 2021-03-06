#!/bin/bash

echo "Making installation directories..."

if [ ! -e /usr/share/index ]
then
	mkdir /usr/share/index
fi

if [ ! -e /usr/share/index/entries ]
then
	mkdir /usr/share/index/entries
fi

if [ ! -e /usr/share/index/tags ]
then
	mkdir /usr/share/index/tags
fi

echo "Changing rights on the previous directories..."
chmod -R 777 /usr/share/index

echo "Installing index' manuals..."
cp doc/index*.1.gz /usr/share/man/man1/
cp doc/index*.5.gz /usr/share/man/man5/
cp doc/index*.7.gz /usr/share/man/man7/

echo "Installing index' programs..."
cp bin/index-* /usr/bin/

echo "Installing index.conf in /etc directory..."
cp index.conf /etc/index.conf

echo "Index Library is now installed on your computer !"
echo "Please don't forget to read the manual !"
echo ""
echo "Send me some feed back about Index at shmirnoff(at)gmail(dot)com !"
