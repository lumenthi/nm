/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 11:52:41 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/27 15:11:15 by lumenthi         ###   ########.fr       */
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
	//printf("value: 0x%lx\n", value);
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

static char		get_type32(t_symbol *symbol, t_info infos)
{
	/* ELF Sections: https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter6-28341 */
	unsigned char st_info = symbol->st_info;
	Elf32_Shdr *shdr32 = (Elf32_Shdr *)(infos.shdr);
	char letter = '?';
	uint32_t shtype = swap_uint32(shdr32[symbol->st_shndx].sh_type);
	uint32_t shflags = swap_uint32(shdr32[symbol->st_shndx].sh_flags);

	// printf("name: %s, shndx: 0x%x, type: 0x%x\n",
		// symbol->sym_name, symbol->st_shndx, shtype);

	/* absolute symbol */
	if (symbol->st_shndx == SHN_ABS)
		letter = 'A';
	/* common symbol*/
	else if (symbol->st_shndx == SHN_COMMON)
		letter = 'C';

	/* unique global symbol */
	else if (ELF32_ST_BIND(st_info) == STB_GNU_UNIQUE)
		letter = 'u';
	else if (ELF32_ST_BIND(st_info) == STB_WEAK) {
		letter = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'w';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		letter = 'U';
	else if (ELF32_ST_BIND(st_info) == STB_WEAK && ELF32_ST_TYPE(st_info) == STT_OBJECT) {
		letter = 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'v';
	}

	/* shdr related analysis */

	/* Read only data section */
	else if (shtype == SHT_PROGBITS && shflags == SHF_ALLOC)
		letter = 'R';
	/* initialized data section */
	else if (shtype == SHT_PROGBITS && shflags == (SHF_ALLOC | SHF_WRITE))
		letter = 'D';
	/* text code section */
	else if (shtype == SHT_PROGBITS && shflags == (SHF_ALLOC | SHF_EXECINSTR))
		letter = 'T';
	/* bss data section */
	else if (shtype == SHT_NOBITS && shflags == (SHF_ALLOC | SHF_WRITE))
		letter = 'B';
	else if (shtype == SHT_INIT_ARRAY ||
		shtype == SHT_FINI_ARRAY ||
		shtype == SHT_DYNAMIC)
		letter = 'D';

	/* If lowercase, the symbol is usually local; if uppercase, the symbol is global (external) */
	if (ELF32_ST_BIND(st_info) == STB_LOCAL && letter != '?')
		letter += 32;

	return letter;
}

static char		get_type64(t_symbol *symbol, t_info infos)
{
	/* ELF Sections: https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter6-28341 */
	unsigned char st_info = symbol->st_info;
	Elf64_Shdr *shdr64 = (Elf64_Shdr *)(infos.shdr);
	char letter = '?';

	// printf("name: %s, shndx: 0x%x, type: 0x%x\n",
		// symbol->sym_name, symbol->st_shndx, shdr64[symbol->st_shndx].sh_type);

	/* absolute symbol */
	if (symbol->st_shndx == SHN_ABS)
		letter = 'A';
	/* common symbol*/
	else if (symbol->st_shndx == SHN_COMMON)
		letter = 'C';

	/* unique global symbol */
	else if (ELF64_ST_BIND(st_info) == STB_GNU_UNIQUE)
		letter = 'u';
	else if (ELF64_ST_BIND(st_info) == STB_WEAK) {
		letter = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'w';
	}
	else if (symbol->st_shndx == SHN_UNDEF)
		letter = 'U';
	else if (ELF64_ST_BIND(st_info) == STB_WEAK && ELF64_ST_TYPE(st_info) == STT_OBJECT) {
		letter = 'V';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'v';
	}

	/* shdr related analysis */

	/* Read only data section */
	else if (shdr64[symbol->st_shndx].sh_type == SHT_PROGBITS
		&& shdr64[symbol->st_shndx].sh_flags == SHF_ALLOC)
		letter = 'R';
	/* initialized data section */
	else if (shdr64[symbol->st_shndx].sh_type == SHT_PROGBITS
		&& shdr64[symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		letter = 'D';
	/* text code section */
	else if (shdr64[symbol->st_shndx].sh_type == SHT_PROGBITS
		&& shdr64[symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		letter = 'T';
	/* bss data section */
	else if (shdr64[symbol->st_shndx].sh_type == SHT_NOBITS
		&& shdr64[symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		letter = 'B';
	else if (shdr64[symbol->st_shndx].sh_type == SHT_INIT_ARRAY ||
		shdr64[symbol->st_shndx].sh_type == SHT_FINI_ARRAY ||
		shdr64[symbol->st_shndx].sh_type == SHT_DYNAMIC)
		letter = 'D';

	/* If lowercase, the symbol is usually local; if uppercase, the symbol is global (external) */
	if (ELF64_ST_BIND(st_info) == STB_LOCAL && letter != '?')
		letter += 32;

	return letter;
}

void		display_symbols(t_symbol *symbols, t_info infos)
{
	t_symbol *tmp = symbols;
	char letter = '?';

	while (tmp) {
		if (tmp->arch == 64)
			letter = get_type64(tmp, infos);
		else
			letter = get_type32(tmp, infos);
		letter == 'U' ?  display_value(0, tmp->arch):display_value(tmp->st_value, tmp->arch);
		ft_putchar(' ');
		ft_putchar(letter);
		ft_putchar(' ');
		ft_putstr(tmp->sym_name);
		ft_putchar('\n');
		tmp = tmp->next;
	}
}
