# ft\_nm

Implementation of the nm command in C. This program reads ELF binaries and lists
the symbols they contain.

## Setup

```shell
# clone it with the libft submodule
git clone --recurse-submodules https://github.com/Taiwing/ft_nm
# build it
cd ft_nm/ && make
# run it
sudo ./ft_nm ft_nm
```

As shown above this program needs sudo rights. If you do not have root access
on your machine but docker is available, then execute the following commands to
run ft\_nm:

```shell
# build docker image and run it
./setup-docker.bash
# run ft_nm inside the container
./ft_nm ft_nm
```

## Description

ft\_nm lists the symbols of every objfile it is provided (a.out by default). It
does this by reading the *.symtab* and *.dynsym* sections of the binaries. A
symbol can be a variable, a function or some arbitrary debugging information
(like file names for example).

### Types

Each symbol is associated a letter which represents its type (where it resides
in the binary, if it has been initialized, wether it is global or local,
etc...). A lowercase letter represents a local symbol (except for "u", "v" and
"w" which have a special meaning), and an uppercase letter is a global symbol.
The *st_info* field has bit that can be set to either *STB_LOCAL* or
*STB_GLOBAL* to represent this.
