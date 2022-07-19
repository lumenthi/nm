/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 10:55:07 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/12 15:01:48 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*[> Constant for the magic field of the mach_header (32-bit architectures) <]*/

/*#define			MH_MAGIC		0xfeedface	[> the mach magic number <]*/
/*#define 			MH_CIGAM		NXSwapInt(MH_MAGIC)*/

/*[> Constant for the magic field of the mach_header_64 (64-bit architectures) <]*/

/*#define 			MH_MAGIC_64		0xfeedfacf	[> the 64-bit mach magic number <]*/
/*#define 			MH_CIGAM_64		NXSwapInt(MH_MAGIC_64)*/

/*[> Constants for the cmd field of all load commands, the type <]*/

/*#define			LC_SEGMENT			0x1		[> segment of this file to be mapped <]*/
/*#define			LC_SEGMENT_64		0x19	[> 64-bit segment of this file to be mapped <]*/
/*#define			LC_SYMTAB			0x2		[> link-edit stab symbol table info <]*/
/*#define			LC_SYMSEG			0x3		[> link-edit gdb symbol table info (obsolete) <]*/
/*#define			LC_THREAD			0x4		[> thread <]*/
/*#define			LC_UNIXTHREAD		0x5		[> unix thread (includes a stack) <]*/
/*#define			LC_LOADFVMLIB		0x6		[> load a specified fixed VM shared library <]*/
/*#define			LC_IDFVMLIB			0x7		[> fixed VM shared library identification <]*/
/*#define			LC_IDENT			0x8		[> object identification info (obsolete) <]*/
/*#define 			LC_FVMFILE			0x9		[> fixed VM file inclusion (internal use) <]*/
/*#define 			LC_PREPAGE			0xa		[> prepage command (internal use) <]*/
/*#define			LC_DYSYMTAB			0xb		[> dynamic link-edit symbol table info <]*/
/*#define			LC_LOAD_DYLIB		0xc		[> load a dynamicly linked shared library <]*/
/*#define			LC_ID_DYLIB			0xd		[> dynamicly linked shared lib identification <]*/
/*#define			LC_LOAD_DYLINKER	0xe		[> load a dynamic linker <]*/
/*#define			LC_ID_DYLINKER		0xf		[> dynamic linker identification <]*/
/*#define			LC_PREBOUND_DYLIB	0x10	[> modules prebound for a dynamicly <]*/

/* * Constants for the filetype field of the mach_header*/
 /**/
/*#define	MH_OBJECT	0x1		[> relocatable object file <]*/
/*#define	MH_EXECUTE	0x2		[> demand paged executable file <]*/
/*#define	MH_FVMLIB	0x3		[> fixed VM shared library file <]*/
/*#define	MH_CORE		0x4		[> core file <]*/
/*#define	MH_PRELOAD	0x5		[> preloaded executable file <]*/
/*#define	MH_DYLIB	0x6		[> dynamicly bound shared library file<]*/
/*#define	MH_DYLINKER	0x7		[> dynamic link editor <]*/
/*#define	MH_BUNDLE	0x8		[> dynamicly bound bundle file <]*/

/*struct mach_header {*/
	/*uint32_t		magic;			[> mach magic number identifier <]*/
	/*cpu_type_t	cputype;		[> cpu specifier <]*/
	/*cpu_subtype_t	cpusubtype;		[> machine specifier <]*/
	/*uint32_t		filetype;		[> type of file <]*/
	/*uint32_t		ncmds;			[> number of load commands <]*/
	/*uint32_t		sizeofcmds;		[> the size of all the load commands <]*/
	/*uint32_t		flags;			[> flags <]*/
/*};*/

/*
 * The 64-bit mach header appears at the very beginning of object files for
 * 64-bit architectures.
 */
/*struct mach_header_64 {*/
	/*uint32_t	magic;		[> mach magic number identifier <]*/
	/*cpu_type_t	cputype;	[> cpu specifier <]*/
	/*cpu_subtype_t	cpusubtype;	[> machine specifier <]*/
	/*uint32_t	filetype;	[> type of file <]*/
	/*uint32_t	ncmds;		[> number of load commands <]*/
	/*uint32_t	sizeofcmds;	[> the size of all the load commands <]*/
	/*uint32_t	flags;		[> flags <]*/
	/*uint32_t	reserved;	[> reserved <]*/
/*};*/

// LOAD COMMAND

/*struct load_command {*/
	/*unsigned long	cmd;			[> type of load command <]*/
	/*unsigned long	cmdsize;		[> total size of command in bytes <]*/
/*};*/

// SYMTAB COMMAND

/*struct symtab_command {*/
	/*unsigned long	cmd;			[> LC_SYMTAB <]*/
	/*unsigned long	cmdsize;		[> sizeof(struct symtab_command) <]*/
	/*unsigned long	symoff;			[> symbol table offset <]*/
	/*unsigned long	nsyms;			[> number of symbol table entries <]*/
	/*unsigned long	stroff;			[> string table offset <]*/
	/*unsigned long	strsize;		[> string table size in bytes <]*/
/*};*/

void	handle_macho(void *file, t_parsed_symbol **parsed_symbols,
	t_found_section **parsed_sections, int architecture) {
	void					*header;
	struct load_command		*cmd;
	struct symtab_command	*symcmd;
	uint32_t				ncmds;
	int						swap;

	header = file;
	if (architecture == 64) {
		cmd = (struct load_command *)(header + sizeof(struct mach_header_64));
		ncmds = ((struct mach_header_64 *)header)->ncmds;
	}
	else {
		cmd = (struct load_command *)(header + sizeof(struct mach_header));
		ncmds = ((struct mach_header *)header)->ncmds;
	}
	swap = 0;
	if (((struct mach_header *)header)->magic == MH_CIGAM ||
		((struct mach_header *)header)->magic == MH_CIGAM_64 ||
		((struct mach_header *)header)->magic == FAT_CIGAM)
			swap = 1;
	if (swap)
		ncmds = swap_uint32(ncmds);
	uint32_t type;
	while (ncmds) {
		type = swap ? swap_uint32(cmd->cmd) : cmd->cmd;
		// CHECK BITS REVERSE ??
		if (type == LC_SEGMENT_64) { //64bits SEGMENT
			handle_segment64(swap, cmd, parsed_sections);
		}
		else if (type == LC_SEGMENT) { // 32bits SEGMENT
			handle_segment32(swap, cmd, parsed_sections);
		}
		else if (type == LC_SYMTAB) { // symbol table SEGMENT
			symcmd = (struct symtab_command *)cmd;
			struct nlist	*symtab;
			char			*strtab;
			if (swap) {
				symtab = (struct nlist *)(file + swap_uint32(symcmd->symoff));
				strtab = (char *)(file + swap_uint32(symcmd->stroff));
			}
			else {
				symtab = (struct nlist *)(file + symcmd->symoff);
				strtab = (char *)(file + symcmd->stroff);
			}
			if (architecture == 64)
				load_symbols64(parsed_symbols, symtab, strtab, *symcmd, swap);
			else
				load_symbols32(parsed_symbols, symtab, strtab, *symcmd, swap);
		}
		if (swap)
			cmd = (struct load_command *)((void *)cmd + swap_uint32(cmd->cmdsize));
		else
			cmd = (struct load_command *)((void *)cmd + cmd->cmdsize);
		ncmds--;
	}
}
