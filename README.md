# GNU nm implementation
List symbols from object files

## Description
GNU nm lists the symbols from object files objfile....  
If no object files are listed as arguments, nm assumes the file a.out.  
For each symbol, nm shows:
- The symbol value, in the radix selected by options, or hexadecimal by default.
- The symbol type. If lowercase, the symbol is local; if uppercase, the symbol is global (external).
- The symbol name.

## Supported format
### Architecture
- 64-bit
- 32-bit
- Little endian
- Big endian
### File
- Linux executables
- Object files (.o)
- Shared objects (.so)

## Options
Implemented options: 
- -g | Display only external symbols.
- -p | Do not bother to sort the symbols in any order; print them in the order encountered.
- -u | Display only undefined symbols (those external to each object file).
- -r | Reverse the order of the sort (whether numeric or alphabetic); let the last come first.

## Make
Given Makefile contains all standard rules
- all
- clean
- fclean
- re

## Example output
```shell
$ make
 ~ | Compiled libft
 ~ | Compiled ft_nm
$ ./ft_nm tests/good/a.out 
0000000000601028 B __bss_start
0000000000601028 b completed.7698
0000000000601018 D __data_start
0000000000601018 W data_start
00000000004003f0 t deregister_tm_clones
00000000004003e0 T _dl_relocate_static_pie
0000000000400460 t __do_global_dtors_aux
0000000000600e58 d __do_global_dtors_aux_fini_array_entry
0000000000601020 D __dso_handle
0000000000600e60 d _DYNAMIC
0000000000601028 D _edata
0000000000601030 B _end
0000000000400524 T _fini
0000000000400490 t frame_dummy
0000000000600e50 d __frame_dummy_init_array_entry
000000000040063c r __FRAME_END__
0000000000601000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000400534 r __GNU_EH_FRAME_HDR
0000000000400390 T _init
0000000000600e58 d __init_array_end
0000000000600e50 d __init_array_start
0000000000400530 R _IO_stdin_used
0000000000400520 T __libc_csu_fini
00000000004004b0 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
00000000004004a0 T main
0000000000400420 t register_tm_clones
00000000004003b0 T _start
0000000000601028 D __TMC_END__
```
