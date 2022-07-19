/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/19 16:16:29 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	symbols(void *header, char *path, int arch, size_t size)
{
	Elf32_Shdr *sheader32 = NULL;
	Elf64_Shdr *sheader64 = NULL;
	size_t shoff = 0;

	(void)path;
	(void)sheader32;
	(void)sheader64;
	(void)shoff;
	if (arch == 32) {
		printf("ELF 32-Bits\n");
		shoff = (uint32_t)(((Elf32_Ehdr*)header)->e_shoff);
		sheader32 = (Elf32_Shdr *)(header + ((Elf32_Ehdr*)header)->e_shoff);
	}
	else {
		printf("ELF 64-Bits\n");
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		sheader64 = (Elf64_Shdr *)(header + ((Elf64_Ehdr*)header)->e_shoff);
	}
	printf("Found section header offset: %ld\n", ((Elf64_Ehdr*)header)->e_shoff);
	if (shoff > size) {
		ft_putstr_fd("error: nm: invalid section offset: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		return;
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
		// TODO: CHECK LENGTH TO SEE IF WE CAN ACCESS SH OFFSET
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
