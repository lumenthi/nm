/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:51:11 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/25 18:54:08 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	process_symbol32(void *header, Elf32_Sym *sheader32, t_symbol **symbols, t_info infos)
{
	/* skip symbols with no name and file symbols */
	if (!sheader32->st_name || sheader32->st_info == 0x4)
		return;
	t_symbol *new = (t_symbol *)malloc(sizeof(t_symbol));
	if (!new)
		return;
	new->sym_name = (char*)(header+infos.strtab_offset)+sheader32->st_name;

	new->st_name = sheader32->st_name;
	new->st_info = sheader32->st_info;
	new->st_other = sheader32->st_other;
	new->st_shndx = sheader32->st_shndx;
	new->st_value = sheader32->st_value;
	new->st_size = sheader32->st_size;
	new->arch = 32;
	new->next = NULL;
	append_symbol(symbols, new);
}

static void	process_symbol64(void *header, Elf64_Sym *sheader64, t_symbol **symbols, t_info infos)
{
	/* skip symbols with no name and file symbols */
	if (!sheader64->st_name || sheader64->st_info == 0x4)
		return;
	t_symbol *new = (t_symbol *)malloc(sizeof(t_symbol));
	if (!new) {
		return;
	}
	new->sym_name = (char*)(header+infos.strtab_offset)+sheader64->st_name;

	new->st_name = sheader64->st_name;
	new->st_info = sheader64->st_info;
	new->st_other = sheader64->st_other;
	new->st_shndx = sheader64->st_shndx;
	new->st_value = sheader64->st_value;
	new->st_size = sheader64->st_size;
	new->arch = 64;
	new->next = NULL;
	append_symbol(symbols, new);
}

int		get_symbols(void *header, t_info infos, int arch, t_symbol **symbols)
{
	size_t i = 0;
	Elf32_Sym *Sym32 = NULL;
	Elf64_Sym *Sym64 = NULL;
	if (arch == 32) {
		while (i * sizeof(Elf32_Sym) < infos.symtab_size) {
			//printf("Processing symbol: 0x%ld/0x%ld\n", i*sizeof(Elf32_Sym), infos.symtab_size);
			Sym32 = (Elf32_Sym *)(header+infos.symtab_offset+i*sizeof(Elf32_Sym));
			process_symbol32(header, Sym32, symbols, infos);
			i++;
		}
	}
	else {
		while (i * sizeof(Elf64_Sym) < infos.symtab_size) {
			//printf("Processing symbol: 0x%ld/0x%ld\n", i*sizeof(Elf64_Sym), infos.symtab_size);
			Sym64 = (Elf64_Sym *)(header+infos.symtab_offset+i*sizeof(Elf64_Sym));
			process_symbol64(header, Sym64, symbols, infos);
			i++;
		}
	}
	return 0;
}
