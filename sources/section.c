/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:45:08 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/27 14:49:02 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int				sections_infos(void *header, char *path, int arch, size_t size,
					t_info *infos)
{
	uint64_t shoff = 0;
	uint16_t shstrndx = 0;
	uint32_t shsize = 0;
	uint32_t shnum = 0;

	Elf32_Shdr *shstrtab32 = NULL;
	Elf64_Shdr *shstrtab64 = NULL;

	Elf32_Shdr *cursh32 = NULL;
	Elf64_Shdr *cursh64 = NULL;

	// Variables assignment
	if (arch == 32) {
		// printf("ELF 32-Bits\n");
		shoff = swap_uint32((uint32_t)(((Elf32_Ehdr*)header)->e_shoff));
		shnum = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shnum));
		shsize = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shentsize));
		shstrndx = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shstrndx));
	}
	else {
		// printf("ELF 64-Bits\n");
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf64_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf64_Ehdr*)header)->e_shentsize);
		shstrndx = (uint16_t)(((Elf64_Ehdr*)header)->e_shstrndx);
	}
	// printf("Found %d sections with size: %d at offset: %ld\n", shnum, shsize, shoff);
	// printf("Checking: %ld > %ld || %ld > %ld\n",shoff+shnum*shsize, size, shoff+shstrndx*shsize, size);
	// Error check
	if (shoff+shnum*shsize > size || shoff+shstrndx*shsize > size)
		return error("invalid sections", path);

	infos->shdr = header+shoff;
	infos->section_size = shsize*shnum;

	while (shnum) {
		if (arch == 32) {
			cursh32 = (Elf32_Shdr *)(header + shoff + shnum*shsize);
			if (swap_uint32(cursh32->sh_type) == 0x2) { // SYMTAB
				infos->symtab_offset = swap_uint32(cursh32->sh_offset);
				infos->symtab_size = swap_uint32(cursh32->sh_size);
			}
			else if (swap_uint32(cursh32->sh_type) == 0x3) { // STRTAB
				shstrtab32 = (Elf32_Shdr *)((header + shoff)+(shstrndx*shsize));
				if (swap_uint32(shstrtab32->sh_offset) +
					swap_uint32(cursh32->sh_name) > size)
					return error("invalid sections", path);
				if (!ft_strcmp((char*)(header +
										swap_uint32(shstrtab32->sh_offset) +
										swap_uint32(cursh32->sh_name)), ".strtab")) {
						infos->strtab_offset = swap_uint32(cursh32->sh_offset);
						infos->strtab_size = swap_uint16(cursh32->sh_size);
					}
			}
		}
		else {
			cursh64 = (Elf64_Shdr *)(header + shoff + shnum*shsize);
			if (cursh64->sh_type == 0x2) { // SYMTAB
				infos->symtab_offset = cursh64->sh_offset;
				infos->symtab_size = cursh64->sh_size;
			}
			else if (cursh64->sh_type == 0x3) { // STRTAB
				shstrtab64 = (Elf64_Shdr *)((header + shoff)+(shstrndx*shsize));
				// printf("Checking: %ld+%d=%ld > %ld\n", shstrtab64->sh_offset, cursh64->sh_name,
					// shstrtab64->sh_offset + cursh64->sh_name, size);
				if (shstrtab64->sh_offset + cursh64->sh_name > size)
					return error("invalid sections", path);
				if (!ft_strcmp((char*)(header +
										shstrtab64->sh_offset +
										cursh64->sh_name), ".strtab")) {
						infos->strtab_offset = cursh64->sh_offset;
						infos->strtab_size = cursh64->sh_size;
					}
			}
		}
		shnum--;
	}
	return 0;
}