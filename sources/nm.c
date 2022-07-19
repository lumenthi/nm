/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/19 18:29:33 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	process_symbol32(Elf32_Shdr *sheader32)
{
	printf("symbol name: %d\n", sheader32->sh_name);
	printf("symbol type: %d\n", sheader32->sh_type);
}

void	process_symbol64(Elf64_Shdr *sheader64)
{
	printf("symbol name: %d\n", sheader64->sh_name);
	printf("symbol type: %d\n", sheader64->sh_type);
}

void	symbols(void *header, char *path, int arch, size_t size)
{
	size_t shoff = 0;
	uint32_t shsize = 0;
	uint32_t shnum = 0;
	uint32_t i = 0;

	if (arch == 32) {
		printf("ELF 32-Bits\n");
		shoff = (uint32_t)(((Elf32_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf32_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf32_Ehdr*)header)->e_shentsize);
	}
	else {
		printf("ELF 64-Bits\n");
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf64_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf64_Ehdr*)header)->e_shentsize);
	}
	printf("Found %d sections with size: %d at offset: %ld\n", shnum, shsize, shoff);
	if (shoff+shnum*shsize > size) {
		ft_putstr_fd("error: nm: invalid sections: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		return;
	}
	while (i < shnum) {
		if (arch == 32)
			process_symbol32((Elf32_Shdr *)(header + shoff + i*shsize));
		else
			process_symbol64((Elf64_Shdr *)(header + shoff + i*shsize));
		i++;
	}
}

void	ft_nm(char *path, void *buffer, size_t size)
{
	Elf32_Ehdr *header = NULL;
	int arch = 32;

	if (size < sizeof(Elf32_Ehdr) || size < sizeof(Elf64_Ehdr)) {
		ft_putstr_fd("error: nm: invalid header: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		return;
	}
	header = (Elf32_Ehdr *)buffer;
	// 0x7f454c46
	if (*(uint32_t*)header==0x464c457f) { // ELF Magic number
		if (*(uint8_t*)((void*)header+4) == 2) // EI_CLASS = 2 (64 Bits)
			arch = 64;
		symbols((void*)header, path, arch, size);
	}
}

void	nm(char *path) {
	void	*buffer;
	size_t	size;

	size = 0;
	buffer = map_file(path, &size);
	if (buffer) {
		ft_nm(path, buffer, size);
		munmap(buffer, size);
	}
}

int		main(int argc, char **argv) {
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
