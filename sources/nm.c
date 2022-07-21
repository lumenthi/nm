/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/21 17:05:43 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static void	append(t_symbol **head, t_symbol *new) {
	t_symbol *tmp = *head;
	if (*head == NULL)
		*head = new;
	else {
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

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
	append(symbols, new);
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
	append(symbols, new);
}

int		get_symbols(void *header, t_info infos, int arch, t_symbol **symbols)
{
	size_t i = 0;
	Elf32_Sym *Sym32 = NULL;
	Elf64_Sym *Sym64 = NULL;
	if (arch == 32) {
		while (i * sizeof(Elf32_Sym) < infos.symtab_size) {
			Sym32 = (Elf32_Sym *)(header+infos.symtab_offset+i*sizeof(Elf32_Sym));
			process_symbol32(header, Sym32, symbols, infos);
			i++;
		}
	}
	else {
		while (i * sizeof(Elf64_Sym) < infos.symtab_size) {
			Sym64 = (Elf64_Sym *)(header+infos.symtab_offset+i*sizeof(Elf64_Sym));
			process_symbol64(header, Sym64, symbols, infos);
			i++;
		}
	}
	return 0;
}

int		sections_infos(void *header, char *path, int arch, size_t size,
						t_info *infos)
{
	size_t shoff = 0;
	size_t shstrndx = 0;
	uint32_t shsize = 0;
	uint32_t shnum = 0;
	Elf64_Shdr *shstrtab;

	Elf64_Shdr *cursh64;
	Elf32_Shdr *cursh32;

	// Variables assignment
	if (arch == 32) {
		// printf("ELF 32-Bits\n");
		shoff = (uint32_t)(((Elf32_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf32_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf32_Ehdr*)header)->e_shentsize);
		shstrndx = (uint16_t)(((Elf64_Ehdr*)header)->e_shstrndx);
	}
	else {
		// printf("ELF 64-Bits\n");
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf64_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf64_Ehdr*)header)->e_shentsize);
		shstrndx = (uint16_t)(((Elf64_Ehdr*)header)->e_shstrndx);
	}
	// printf("Found %d sections with size: %d at offset: %ld\n", shnum, shsize, shoff);
	infos->shdr = header+shoff;
	infos->section_size = shsize*shnum;

	// Error check
	if (shoff+shnum*shsize > size || shoff+shstrndx*shsize > size)
		return error("invalid sections", path);

	// Calculate shstrtab, we use it to get section name
	shstrtab = (void*)(header + shoff)+(shstrndx*shsize);

	while (shnum) {
		/* sh_type is at the same offset and same size for 32 and 64bits structures */
		cursh32 = (Elf32_Shdr *)(header + shoff + shnum*shsize);
		if (cursh32->sh_type == 0x2) { // SYMTAB VALUE
			if (arch == 32) {
				infos->symtab_offset = cursh32->sh_offset;
				infos->symtab_size = cursh32->sh_size;
			}
			else {
				cursh64 = (Elf64_Shdr *)(header + shoff + shnum*shsize);
				infos->symtab_offset = cursh64->sh_offset;
				infos->symtab_size = cursh64->sh_size;
			}
		}
		else if (cursh32->sh_type == 0x3) { // STRTAB VALUE
			// Error check
			if (shstrtab->sh_offset+cursh32->sh_name > size)
				return error("invalid sections", path);

			if (!ft_strcmp((char*)(header+shstrtab->sh_offset+cursh32->sh_name), ".strtab")) {
				if (arch == 32) {
					infos->strtab_offset = cursh32->sh_offset;
					infos->strtab_size = cursh32->sh_size;
				}
				else {
					cursh64 = (Elf64_Shdr *)(header + shoff + shnum*shsize);
					infos->strtab_offset = cursh64->sh_offset;
					infos->strtab_size = cursh64->sh_size;
				}
			}
		}
		shnum--;
	}
	return 0;
}

static t_info	new_info()
{
	t_info infos;

	infos.shdr = NULL;
	infos.section_size = 0;
	infos.symtab_offset = 0;
	infos.symtab_size = 0;
	infos.strtab_offset = 0;
	infos.strtab_size = 0;

	return infos;
}

int		error(char *str, char *path)
{
	ft_putstr_fd("nm: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return 1;
}

int		ft_nm(char *path, void *buffer, size_t size)
{
	Elf32_Ehdr *header = NULL;
	t_info infos = new_info();
	t_symbol *symbols = NULL;
	int arch = 32;

	if (size < sizeof(Elf32_Ehdr) || size < sizeof(Elf64_Ehdr))
		return error("invalid header", path);
	header = (Elf32_Ehdr *)buffer;
	// 0x7f454c46
	if (*(uint32_t*)header==0x464c457f) { // ELF Magic number
		if (*(uint8_t*)((void*)header+4) == 2) // EI_CLASS = 2 (64 Bits)
			arch = 64;
		if (sections_infos((void*)header, path, arch, size, &infos) == -1)
			return -1;
		if (infos.symtab_offset == 0 || infos.symtab_size == 0) {
			return error("no symbols", path);
		}
		// printf("Found symbol table at offset: 0x%lx with size: 0x%lx\n", infos.symtab_offset,
			// infos.symtab_size);
		// printf("Found string table at offset: %lx with size: %lx\n", infos.strtab_offset,
			// infos.strtab_size);
		get_symbols((void*)header, infos, arch, &symbols);
		sort_symbols(&symbols);
		// t_symbols_display(symbols);
		display_symbols(symbols, infos);
		free_symbols(&symbols);
		// FREE LIST
	}
	else
		return error("file format not recognized", path);
	return 0;
}

int		nm(char *path)
{
	void	*buffer;
	size_t	size;
	int		ret;

	size = 0;
	ret = 0;
	buffer = map_file(path, &size);
	if (buffer) {
		ret = ft_nm(path, buffer, size);
		munmap(buffer, size);
	}
	return ret;
}

int		main(int argc, char **argv)
{
	int i;

	i = 1;
	if (argc < 2)
		nm("a.out");
	else {
		while (i < argc)
			nm(argv[i++]);
	}
	return 0;
}
