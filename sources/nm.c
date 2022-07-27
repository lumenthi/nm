/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/27 17:36:59 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static t_info	new_info()
{
	t_info infos;

	infos.swap = 0;
	infos.arch = 32;
	infos.size = 0;
	infos.e_shnum = 0;
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

	infos.size = size;

	if (size < sizeof(Elf32_Ehdr) || size < sizeof(Elf64_Ehdr))
		return error("invalid header", path);
	header = (Elf32_Ehdr *)buffer;
	// 0x7f454c46
	if (*(uint32_t*)header==0x464c457f) { // ELF Magic number
		if (*(uint8_t*)((void*)header+4) == 2) // EI_CLASS = 2 (64 Bits)
			infos.arch = 64;
		if (*(uint8_t*)((void*)header+5) == 2) // EI_CLASS = 2 (64 Bits)
			infos.swap = 1;
		if (sections_infos((void*)header, path, &infos) == -1)
			return -1;
		if (infos.symtab_offset == 0 || infos.symtab_size == 0)
			return error("no symbols", path);
		//printf("Found symbol table at offset: 0x%lx with size: 0x%lx\n", infos.symtab_offset,
			//infos.symtab_size);
		//printf("Found string table at offset: 0x%lx with size: 0x%lx\n", infos.strtab_offset,
			//infos.strtab_size);
		get_symbols((void*)header, infos, &symbols);
		sort_symbols(&symbols);
		display_symbols(symbols, infos);
		free_symbols(&symbols);
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
	int ret;

	i = 1;
	ret = 0;
	if (argc < 2)
		nm("a.out");
	else {
		if (argc == 2)
			ret = nm(argv[1]);
		else {
			while (i < argc) {
				ft_putstr(argv[i]);
				ft_putstr(":\n");
				ret = nm(argv[i]);
				if (i+1 != argc && ret != 0)
					ft_putchar('\n');
				i++;
			}
		}
	}
	return ret;
}
