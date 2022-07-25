/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 11:50:22 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/25 18:42:12 by lumenthi         ###   ########.fr       */
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

// STRUCTURES
// HEADER
/*	typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	Elf32_Half		e_type;
	Elf32_Half		e_machine;
	Elf32_Word		e_version;
	Elf32_Addr		e_entry;
	Elf32_Off		e_phoff;
	Elf32_Off		e_shoff;
	Elf32_Word		e_flags;
	Elf32_Half		e_ehsize;
	Elf32_Half		e_phentsize;
	Elf32_Half		e_phnum;
	Elf32_Half		e_shentsize;
	Elf32_Half		e_shnum;
	Elf32_Half		e_shstrndx;
}					Elf32_Ehdr;

typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	Elf64_Half		e_type;
	Elf64_Half		e_machine;
	Elf64_Word		e_version;
	Elf64_Addr		e_entry;
	Elf64_Off		e_phoff;
	Elf64_Off		e_shoff;
	Elf64_Word		e_flags;
	Elf64_Half		e_ehsize;
	Elf64_Half		e_phentsize;
	Elf64_Half		e_phnum;
	Elf64_Half		e_shentsize;
	Elf64_Half		e_shnum;
	Elf64_Half		e_shstrndx;
}					Elf64_Ehdr; */

// SECTIONS
/*	typedef struct {
	Elf32_Word		sh_name;
	Elf32_Word		sh_type;
	Elf32_Word		sh_flags;
	Elf32_Addr		sh_addr;
	Elf32_Off		sh_offset;
	Elf32_Word		sh_size;
	Elf32_Word		sh_link;
	Elf32_Word		sh_info;
	Elf32_Word		sh_addralign;
	Elf32_Word		sh_entsize;
}					Elf32_Shdr;

typedef struct {
	Elf64_Word		sh_name;
	Elf64_Word		sh_type;
	Elf64_Xword		sh_flags;
	Elf64_Addr		sh_addr;
	Elf64_Off		sh_offset;
	Elf64_Xword		sh_size;
	Elf64_Word		sh_link;
	Elf64_Word		sh_info;
	Elf64_Xword		sh_addralign;
	Elf64_Xword		sh_entsize;
}					Elf64_Shdr; */

// SYMBOLS
/*	struct Elf32_Sym {
	Elf32_Word		st_name;	// Symbol name (index into string table)
	Elf32_Addr		st_value;	// Value or address associated with the symbol
	Elf32_Word		st_size;	// Size of the symbol
	unsigned char	st_info;	// Symbol's type and binding attributes
	unsigned char	st_other;	// Must be zero; reserved
	Elf32_Half		st_shndx;	// Which section (header table index) it's defined in
};

struct Elf64_Sym {
	Elf64_Word		st_name;	// Symbol name (index into string table)
	unsigned char	st_info;	// Symbol's type and binding attributes
	unsigned char	st_other;	// Must be zero; reserved
	Elf64_Half		st_shndx;	// Which section (header tbl index) it's defined in
	Elf64_Addr		st_value;	// Value or address associated with the symbol
	Elf64_Xword		st_size;	// Size of the symbol
}; */

typedef struct			s_symbol {
	// Display values
	char				*sym_name;
	char				*sym_type;

	// Raw data
	Elf64_Word			st_name;
	unsigned char		st_info;
	/*The symbol's type and binding attributes. A list of the values and meanings appears in Table 7-19.
		The following code shows how to manipulate the values, defined in sys/elf.h:
			#define ELF32_ST_BIND(info)          ((info) >> 4)
			#define ELF32_ST_TYPE(info)          ((info) & 0xf)
			#define ELF32_ST_INFO(bind, type)    (((bind)<<4)+((type)&0xf))

			#define ELF64_ST_BIND(info)          ((info) >> 4)
			#define ELF64_ST_TYPE(info)          ((info) & 0xf)
			#define ELF64_ST_INFO(bind, type)    (((bind)<<4)+((type)&0xf))*/
	unsigned char		st_other;
	Elf64_Half			st_shndx;
	Elf64_Addr			st_value;
	Elf64_Xword			st_size;
	int					arch;

	struct s_symbol		*next;
}						t_symbol;

typedef struct {
		void			*shdr;
		Elf64_Word		section_size;
		Elf64_Off		symtab_offset;
		Elf64_Xword		symtab_size;
		Elf64_Off		strtab_offset;
		Elf32_Xword		strtab_size;
}						t_info;

// SWAP.C
uint16_t	swap_uint16(uint16_t nb);
uint32_t	swap_uint32(uint32_t nb);

// MAP.C
void		*map_file(char *path, size_t *size);

// LIST.C
void		sort_symbols(t_symbol **head);
void		free_symbols(t_symbol **symbols);

// DISPLAY.C
void		display_symbols(t_symbol *symbols, t_info infos);

// NM.C
int			error(char *str, char *path);

#endif
