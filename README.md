Index Library
=============

Index is a tags based library for **GNU/Linux**.
This project is under **BSD** license.

Index is to store, sort and search documents with a powerful and simple command line interface. Each file is stored as an entry in the given directory (by default : */usr/share/index/entries*) and a sign is saved in the tag files in the tag directory (by default : */usr/share/index/tags*).

The user can use the four following commands :
  - index-add
  - index-search
  - index-remove
  - index-shell

index-shell is a simple shell that keep an environment in mind (tags to work with) and give very short aliases for the long index command names (by default : *add sch rm*).

Installation
------------

### Compilation

The *compile.sh* script can compile the whole project. If it is used without argument, each program will be compiled. Send the program name as argument will make *compile.sh* compile only the specified program. The possible arguments are the four commands presented before.

Example :
    compile.sh index-search

### Installation

If you want to choose the path of the entries and tags directories, edit the *index.conf* file. Else, don't do anything. Then, launch the *install.sh* script as superuser. The installation will operate.
By default, the main, entries and tags directories have *777* rights. You can change it yourself, but remember that if someone want to add an entry, **he must have the write right**. 
