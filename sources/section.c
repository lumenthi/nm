/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:45:08 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/25 19:01:10 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int		sections_infos(void *header, char *path, int arch, size_t size,
						t_info *infos)
{
	uint64_t shoff = 0;
	uint16_t shstrndx = 0;
	uint32_t shsize = 0;
	uint32_t shnum = 0;
	Elf64_Shdr *shstrtab;

	Elf64_Shdr *cursh64;
	Elf32_Shdr *cursh32;

	uint64_t shtype = 0;

	// Variables assignment
	if (arch == 32) {
		printf("ELF 32-Bits\n");
		shoff = swap_uint32((uint32_t)(((Elf32_Ehdr*)header)->e_shoff));
		shnum = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shnum));
		shsize = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shentsize));
		shstrndx = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shstrndx));
	}
	else {
		printf("ELF 64-Bits\n");
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf64_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf64_Ehdr*)header)->e_shentsize);
		shstrndx = (uint16_t)(((Elf64_Ehdr*)header)->e_shstrndx);
	}
	printf("Found %d sections with size: %d at offset: %ld\n", shnum, shsize, shoff);
	printf("Checking: %ld > %ld || %ld > %ld\n",shoff+shnum*shsize, size, shoff+shstrndx*shsize, size);
	// Error check
	if (shoff+shnum*shsize > size || shoff+shstrndx*shsize > size)
		return error("invalid sections", path);

	infos->shdr = header+shoff;
	infos->section_size = shsize*shnum;

	// Calculate shstrtab, we use it to get section name
	shstrtab = (void*)(header + shoff)+(shstrndx*shsize);

	while (shnum) {
		// printf("Processing section: %d\n", shnum);
		/* sh_type is at the same offset and same size for 32 and 64bits structures */
		cursh32 = (Elf32_Shdr *)(header + shoff + shnum*shsize);
		shtype = arch == 32 ? swap_uint32(cursh32->sh_type) : cursh32->sh_type;
		if (shtype == 0x2) { // SYMTAB VALUE
			if (arch == 32) {
				infos->symtab_offset = swap_uint32(cursh32->sh_offset);
				infos->symtab_size = swap_uint32(cursh32->sh_size);
			}
			else {
				cursh64 = (Elf64_Shdr *)(header + shoff + shnum*shsize);
				infos->symtab_offset = cursh64->sh_offset;
				infos->symtab_size = cursh64->sh_size;
			}
		}
		else if (shtype == 0x3) { // STRTAB VALUE
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
