metashell (msh)
===

Metashell (aka /bin/msh) is a word expanding shell.

EXAMPLE:

```sh

bash-3.2$ make
g++ -g --std=c++11 -c main.cpp
g++ -g --std=c++11 -o msh main.o

bash-3.2$ ./msh

Welcome to msh (metashell).

Type words to enter them into the line buffer.
Type 'go' to execute the line buffer in /bin/sh.
Type 'clear' to clear the line buffer
Type 'define <word> <args...> as <definition...> .' to define new words.
Type 'lookup <word> to get a word's definition.
Type 'words' to see all defined words.

[  ]
$ define list as ls
> go
[  ]
$ define list2 x as list x go
> > [  ]
$ list2 .
[ ls  .   ]
$ go
Makefile
README.md
main.cpp
main.o
msh
old
rd
[  ]
$ define list where as ls where go
> > [  ]
$ define status as git 'status' go
[  ]
$ status
[ git 'status'  ]
$ go
```

--

Burton Samograd - 2018