/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 11:52:41 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/31 11:18:12 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static size_t	hex_len(size_t nb)
{
	size_t count = 0;
	if (nb == 0)
		return 1;
	while (nb) {
		nb >>= 4;
		count++;
	}
	return count;
}

static void		display_value(uint64_t value, int arch)
{
	size_t len;
	size_t display_len = arch / 4;

	if (value == 0) {
		while (display_len) {
			ft_putchar(' ');
			display_len--;
		}
		return;
	}
	len = hex_len(value);
	while (len < display_len) {
		ft_putchar('0');
		len++;
	}
	ft_puthex(0, 0, value);
}

static char		section_finder(t_symbol *symbol, uint32_t shtype, uint32_t shflags)
{
	char letter = '?';
	/* shdr related analysis */

	if ((shtype == SHT_PROGBITS && shflags == (SHF_ALLOC | SHF_WRITE)) ||
		(symbol->sect_name && (
			!ft_strncmp(symbol->sect_name, ".data\0", 6) ||
			!ft_strncmp(symbol->sect_name, ".got.plt\0", 9) ||
			!ft_strncmp(symbol->sect_name, ".init_array\0", 12) ||
			!ft_strncmp(symbol->sect_name, ".dynamic\0", 9) ||
			!ft_strncmp(symbol->sect_name, ".fini_array\0", 12) ||
			!ft_strncmp(symbol->sect_name, ".data.rel.ro\0", 13) ||
			!ft_strncmp(symbol->sect_name, ".ctors\0", 7) ||
			!ft_strncmp(symbol->sect_name, ".dtors\0", 7) ||
			!ft_strncmp(symbol->sect_name, ".got\0", 5) ||
			!ft_strncmp(symbol->sect_name, ".tm_clone_table\0", 16) ||
			!ft_strncmp(symbol->sect_name, ".preinit_array\0", 15)
		))
	)
		letter = 'D';
	else if ((symbol->sect_name && (
			!ft_strncmp(symbol->sect_name, ".rodata\0", 8) ||
			!ft_strncmp(symbol->sect_name, ".eh_frame\0", 10) ||
			!ft_strncmp(symbol->sect_name, ".interp\0", 8) ||
			!ft_strncmp(symbol->sect_name, ".eh_frame_hdr\0", 14) ||
			!ft_strncmp(symbol->sect_name, ".gcc_except_table\0", 18) ||
			!ft_strncmp(symbol->sect_name, ".rodata.str", 11) ||
			!ft_strncmp(symbol->sect_name, ".rodata.cst", 11)
		))
	)
		letter = 'R';
	else if ((shtype == SHT_PROGBITS && shflags == (SHF_ALLOC | SHF_EXECINSTR)) ||
		(symbol->sect_name && (
			!ft_strncmp(symbol->sect_name, ".text\0", 6) ||
			!ft_strncmp(symbol->sect_name, ".init\0", 6) ||
			!ft_strncmp(symbol->sect_name, ".fini\0", 6) ||
			!ft_strncmp(symbol->sect_name, ".plt\0", 5)
		))
	)
		letter = 'T';
	else if (symbol->sect_name && (
		!ft_strncmp(symbol->sect_name, ".bss\0", 5) ||
		!ft_strncmp(symbol->sect_name, ".tbss\0", 6)
	))
		letter = 'B';
	return letter;
}

static char		get_type32(t_symbol *symbol, t_info infos)
{
	/* ELF Sections: https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter6-28341 */
	Elf32_Shdr *shdr32 = (Elf32_Shdr *)(infos.shdr);
	unsigned char st_info = symbol->st_info;
	uint32_t shtype = 0x00;
	uint32_t shflags = 0x00;

	char letter = '?';

	if (symbol->st_shndx < infos.e_shnum) {
		shtype = swap_uint32(shdr32[symbol->st_shndx].sh_type, infos.swap);
		shflags = swap_uint32(shdr32[symbol->st_shndx].sh_flags, infos.swap);
	}

	/* absolute symbol */
	if (symbol->st_shndx == SHN_ABS)
		letter = 'A';
	/* common symbol */
	else if (symbol->st_shndx == SHN_COMMON)
		letter = 'C';

	/* special symbols */
	else if (ELF32_ST_BIND(st_info) == STB_GNU_UNIQUE)
		letter = 'u';
	else if (ELF32_ST_BIND(st_info) == STB_WEAK && ELF32_ST_TYPE(st_info) == STT_OBJECT) {
		letter = 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'v';
	}	else if (ELF32_ST_BIND(st_info) == STB_WEAK) {
		letter = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'w';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		letter = 'U';

	/* sections related symbols */
	else
		letter = section_finder(symbol, shtype, shflags);

	/* If lowercase, the symbol is usually local; if uppercase, the symbol is global (external) */
	if (ELF32_ST_BIND(st_info) == STB_LOCAL && letter != '?')
		letter += 32;

	return letter;
}

static char		get_type64(t_symbol *symbol, t_info infos)
{
	/* ELF Sections: https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter6-28341 */
	Elf64_Shdr *shdr64 = (Elf64_Shdr *)(infos.shdr);
	unsigned char st_info = symbol->st_info;
	uint32_t shtype = 0x00;
	uint32_t shflags = 0x00;

	char letter = '?';

	if (symbol->st_shndx < infos.e_shnum) {
		shtype = shdr64[symbol->st_shndx].sh_type;
		shflags = shdr64[symbol->st_shndx].sh_flags;
	}

	/* absolute symbol */
	if (symbol->st_shndx == SHN_ABS)
		letter = 'A';
	/* common symbol */
	else if (symbol->st_shndx == SHN_COMMON)
		letter = 'C';

	/* special symbols */
	else if (ELF64_ST_BIND(st_info) == STB_GNU_UNIQUE)
		letter = 'u';
	else if (ELF64_ST_BIND(st_info) == STB_WEAK && ELF64_ST_TYPE(st_info) == STT_OBJECT) {
		letter = 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'v';
	}
	else if (ELF64_ST_BIND(st_info) == STB_WEAK) {
		letter = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'w';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		letter = 'U';

	/* sections related symbols */
	else
		letter = section_finder(symbol, shtype, shflags);

	/* If lowercase, the symbol is usually local; if uppercase, the symbol is global (external) */
	if (ELF64_ST_BIND(st_info) == STB_LOCAL && letter != '?')
		letter += 32;

	return letter;
}

static void		display_zeroes(int arch)
{
	size_t display_len = arch / 4;

	while (display_len) {
		ft_putchar('0');
		display_len--;
	}
}

void		display_symbols(t_symbol *symbols, t_info infos)
{
	t_symbol *tmp = symbols;
	char letter = '?';
	uint8_t debug = infos.args & 0x10;
	uint8_t ext = infos.args & 0x08;
	uint8_t undef = infos.args & 0x04;

	while (tmp) {
		/* skip symbols with no name and file symbols */
		if (tmp->st_name &&
			((!debug && (tmp->st_info != 0x4))||(debug)) &&
			(!ext || (ext && ELF64_ST_BIND(tmp->st_info) != 0x00)) &&
			(!undef || (undef && tmp->st_shndx == STN_UNDEF))
		) {
			if (tmp->arch == 64)
				letter = get_type64(tmp, infos);
			else
				letter = get_type32(tmp, infos);
			if (letter == 'U' || letter == 'w')
				display_value(0, tmp->arch);
			else if (!tmp->st_value)
				display_zeroes(tmp->arch);
			else
				display_value(tmp->st_value, tmp->arch);
			ft_putchar(' ');
			ft_putchar(letter);
			ft_putchar(' ');
			ft_putstr(tmp->sym_name);
			ft_putchar('\n');
		}
		tmp = tmp->next;
	}
}
