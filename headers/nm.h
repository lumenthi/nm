/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 11:50:22 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/20 16:00:35 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

// FOR DEBUG (printf), remove after
#include <stdio.h>

#include "libft.h"

#include <unistd.h> // STDERR_FILENO
#include <sys/mman.h> // MMAP FUNCTIONS
#include <sys/stat.h> // FSTAT
#include <fcntl.h> // OPEN
#include <elf.h> // ELF RELATED STRUCTURES
#include <stdlib.h> // MALLOC

/*
typedef uint32_t	Elf32_Addr;
typedef uint16_t	Elf32_Half;
typedef uint32_t	Elf32_Off;
typedef int32_t		Elf32_Sword;
typedef uint32_t	Elf32_Word;

typedef uint64_t	Elf32_Lword;
typedef uint64_t	Elf64_Addr;
typedef uint16_t	Elf64_Half;
typedef uint64_t	Elf64_Off;
typedef int32_t		Elf64_Sword;
typedef int64_t		Elf64_Sxword;
typedef uint32_t	Elf64_Word;
typedef uint64_t	Elf64_Lword;
typedef uint64_t	Elf64_Xword;
*/

// STRUCTURES
// HEADER
/* typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf32_Half      e_type;
        Elf32_Half      e_machine;
        Elf32_Word      e_version;
        Elf32_Addr      e_entry;
        Elf32_Off       e_phoff;
        Elf32_Off       e_shoff;
        Elf32_Word      e_flags;
        Elf32_Half      e_ehsize;
        Elf32_Half      e_phentsize;
        Elf32_Half      e_phnum;
        Elf32_Half      e_shentsize;
        Elf32_Half      e_shnum;
        Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf64_Half      e_type;
        Elf64_Half      e_machine;
        Elf64_Word      e_version;
        Elf64_Addr      e_entry;
        Elf64_Off       e_phoff;
        Elf64_Off       e_shoff;
        Elf64_Word      e_flags;
        Elf64_Half      e_ehsize;
        Elf64_Half      e_phentsize;
        Elf64_Half      e_phnum;
        Elf64_Half      e_shentsize;
        Elf64_Half      e_shnum;
        Elf64_Half      e_shstrndx;
} Elf64_Ehdr; */

// SECTIONS
/* typedef struct {
        Elf32_Word      sh_name;
        Elf32_Word      sh_type;
        Elf32_Word      sh_flags;
        Elf32_Addr      sh_addr;
        Elf32_Off       sh_offset;
        Elf32_Word      sh_size;
        Elf32_Word      sh_link;
        Elf32_Word      sh_info;
        Elf32_Word      sh_addralign;
        Elf32_Word      sh_entsize;
} Elf32_Shdr;

typedef struct {
        Elf64_Word      sh_name;
        Elf64_Word      sh_type;
        Elf64_Xword     sh_flags;
        Elf64_Addr      sh_addr;
        Elf64_Off       sh_offset;
        Elf64_Xword     sh_size;
        Elf64_Word      sh_link;
        Elf64_Word      sh_info;
        Elf64_Xword     sh_addralign;
        Elf64_Xword     sh_entsize;
} Elf64_Shdr; */

// SYMBOLS
/* struct Elf32_Sym {
	Elf32_Word			st_name;	// Symbol name (index into string table)
	Elf32_Addr			st_value;	// Value or address associated with the symbol
	Elf32_Word			st_size;	// Size of the symbol
	unsigned char		st_info;	// Symbol's type and binding attributes
	unsigned char		st_other;	// Must be zero; reserved
	Elf32_Half			st_shndx;	// Which section (header table index) it's defined in
};

struct Elf64_Sym {
	Elf64_Word			st_name;	// Symbol name (index into string table)
	unsigned char		st_info;	// Symbol's type and binding attributes
	unsigned char		st_other;	// Must be zero; reserved
	Elf64_Half			st_shndx;	// Which section (header tbl index) it's defined in
	Elf64_Addr			st_value;	// Value or address associated with the symbol
	Elf64_Xword			st_size;	// Size of the symbol
}; */

typedef struct			s_section {
	char				*sym_name;
	uint32_t			sh_name;
	uint32_t			sh_type;
	uint32_t			sh_flags;
	uint64_t			sh_offset;
	int					arch;
	struct s_section	*next;
}						t_section;

typedef struct {
		Elf64_Off       sh_offset;
		Elf64_Xword     sh_size;
}						t_Sym_Shdr;

// MAP.C
void	*map_file(char *path, size_t *size);

// LIST.C
void	display_symbols(t_section *symbols);

#endif
