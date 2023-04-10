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
./ft_nm ft_nm
```

## Description

ft\_nm lists the symbols of every objfile it is provided (a.out by default). It
does this by reading the *.symtab* section of the binaries. A symbol can be a
variable, a function or some arbitrary debugging information (like file names
for example).

### Types

Each symbol is associated a letter for its type. A lowercase letter represents a
local symbol (this is typically true for static functions), and uppercase types
are globals. The only exception to this are the "u", "v" and "w" symbols which
have a special meaning.

## Usage

```
Usage:
	ft_nm [-eagurph] host ...

Options:
	-e, --elf
		Print elf header (same behavior as readelf -h).
	-a, --debug-syms
		Display all symbols, including debugger-only symbols.
	-g, --extern-only
		Display only external symbols.
	-u, --undefined-only
		Display only undefined symbols (those external to each object file).
	-r, --reverse-sort
		Reverse the order of the sort.
	-p, --no-sort
		Do not sort symbols, print them in the order encountered.
	-h, --help
		Print this.
```

#### example:

```C
#include <stdio.h>

int	main(void) {
	printf("I'm a very simple C program!\n");
	return (0);
}
```

Compile this program and list its symbols:

```shell
gcc simple.c
./ft_nm
```

possible output:

```
000000000000038c r __abi_tag
0000000000004010 B __bss_start
0000000000004010 b completed.0
                 w __cxa_finalize@GLIBC_2.2.5
0000000000004000 D __data_start
0000000000004000 W data_start
0000000000001090 t deregister_tm_clones
0000000000001100 t __do_global_dtors_aux
0000000000003dc0 d __do_global_dtors_aux_fini_array_entry
0000000000004008 D __dso_handle
0000000000003dc8 d _DYNAMIC
0000000000004010 D _edata
0000000000004018 B _end
0000000000001164 T _fini
0000000000001140 t frame_dummy
0000000000003db8 d __frame_dummy_init_array_entry
0000000000002100 r __FRAME_END__
0000000000003fb8 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000002024 r __GNU_EH_FRAME_HDR
0000000000001000 T _init
0000000000002000 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 U __libc_start_main@GLIBC_2.34
0000000000001149 T main
                 U puts@GLIBC_2.2.5
00000000000010c0 t register_tm_clones
0000000000001060 T _start
0000000000004010 D __TMC_END__
```
