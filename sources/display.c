/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 11:52:41 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/21 14:53:08 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static size_t	hex_len(size_t nb) {
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

static void		display_type32(t_symbol *symbol, t_info infos)
{
	(void)symbol;
	(void)infos;
}

static void		display_type64(t_symbol *symbol, t_info infos)
{
	/* ELF Sections: https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter6-28341 */
	unsigned char st_info = symbol->st_info;
	Elf64_Shdr *shdr64 = (Elf64_Shdr *)(infos.shdr);
	char letter = '?';

	/* unique global symbol */
	if (ELF64_ST_BIND(st_info) == STB_GNU_UNIQUE)
		letter = 'u';
	else if (ELF64_ST_BIND(st_info) == STB_WEAK) {
		letter = 'W';
		if (symbol->st_shndx == SHN_UNDEF)
			letter = 'w';
	}
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
	else if (shdr64[symbol->st_shndx].sh_type == SHT_DYNAMIC)
		letter = 'D';

	/* section index member analysis */

	/* undefined symbol */
	else if (symbol->st_shndx == SHN_UNDEF)
		letter = 'U';
	/* absolute symbol */
	else if (symbol->st_shndx == SHN_ABS)
		letter = 'A';
	/* common symbol*/
	else if (symbol->st_shndx == SHN_COMMON)
		letter = 'C';

	/* If lowercase, the symbol is usually local; if uppercase, the symbol is global (external) */
	if (ELF64_ST_BIND(st_info) == STB_LOCAL && letter != '?')
		letter += 32;
	ft_putchar(letter);
}

void		display_symbols(t_symbol *symbols, t_info infos)
{
	t_symbol *tmp = symbols;

	while (tmp) {
		display_value(tmp->st_value, tmp->arch);
		ft_putchar(' ');
		if (tmp->arch == 64)
			display_type64(tmp, infos);
		else
			display_type32(tmp, infos);
		ft_putchar(' ');
		ft_putstr(tmp->sym_name);
		ft_putchar('\n');
		tmp = tmp->next;
	}
}
