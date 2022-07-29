/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:45:08 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/29 18:52:09 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

int				sections_infos(void *header, char *path, t_info *infos)
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
	if (infos->arch == 32) {
		shoff = swap_uint32((uint32_t)(((Elf32_Ehdr*)header)->e_shoff), infos->swap);
		shnum = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shnum), infos->swap);
		shsize = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shentsize), infos->swap);
		shstrndx = swap_uint16((uint16_t)(((Elf32_Ehdr*)header)->e_shstrndx), infos->swap);
	}
	else {
		shoff = (uint64_t)(((Elf64_Ehdr*)header)->e_shoff);
		shnum = (uint32_t)(((Elf64_Ehdr*)header)->e_shnum);
		shsize = (uint32_t)(((Elf64_Ehdr*)header)->e_shentsize);
		shstrndx = (uint16_t)(((Elf64_Ehdr*)header)->e_shstrndx);
	}

	// Error check
	if (shoff+shnum*shsize > infos->size || shoff+shstrndx*shsize > infos->size)
		return error("invalid sections", path);

	infos->shdr = header+shoff;
	infos->section_size = shsize*shnum;
	infos->e_shnum = shnum;

	while (shnum) {
		if (infos->arch == 32) {
			cursh32 = (Elf32_Shdr *)(header + shoff + shnum*shsize);

			/* Corrupted section check */
			if (swap_uint16(cursh32->sh_size, infos->swap) >= infos->size)
				return error("corrupted section", path);

			if (swap_uint32(cursh32->sh_type, infos->swap) == 0x2) { // SYMTAB
				infos->symtab_offset = swap_uint32(cursh32->sh_offset, infos->swap);
				infos->symtab_size = swap_uint32(cursh32->sh_size, infos->swap);
			}
			else if (swap_uint32(cursh32->sh_type, infos->swap) == 0x3) { // STRTAB
				shstrtab32 = (Elf32_Shdr *)((header + shoff)+(shstrndx*shsize));
				if (swap_uint32(shstrtab32->sh_offset, infos->swap) +
					swap_uint32(cursh32->sh_name, infos->swap) > infos->size)
					return error("invalid sections", path);
				if (!ft_strncmp((char*)(header +
										swap_uint32(shstrtab32->sh_offset, infos->swap) +
										swap_uint32(cursh32->sh_name, infos->swap)), ".strtab\0", 8)) {
						infos->strtab_offset = swap_uint32(cursh32->sh_offset, infos->swap);
						infos->strtab_size = swap_uint16(cursh32->sh_size, infos->swap);
					}
				else if (!ft_strncmp((char*)(header +
										swap_uint32(shstrtab32->sh_offset, infos->swap) +
										swap_uint32(cursh32->sh_name, infos->swap)), ".shstrtab\0", 10)) {
						infos->shstrtab_offset = swap_uint32(cursh32->sh_offset, infos->swap);
						infos->shstrtab_size = swap_uint16(cursh32->sh_size, infos->swap);
					}
			}
		}
		else {
			cursh64 = (Elf64_Shdr *)(header + shoff + shnum*shsize);

			/* Corrupted section check */
			if (cursh64->sh_size >= infos->size)
				return error("corrupted section", path);

			if (cursh64->sh_type == 0x2) { // SYMTAB
				infos->symtab_offset = cursh64->sh_offset;
				infos->symtab_size = cursh64->sh_size;
			}
			else if (cursh64->sh_type == 0x3) { // STRTAB
				shstrtab64 = (Elf64_Shdr *)((header + shoff)+(shstrndx*shsize));
				if (shstrtab64->sh_offset + cursh64->sh_name > infos->size)
					return error("invalid sections", path);
				if (!ft_strncmp((char*)(header +
										shstrtab64->sh_offset +
										cursh64->sh_name), ".strtab\0", 8)) {
						infos->strtab_offset = cursh64->sh_offset;
						infos->strtab_size = cursh64->sh_size;
					}
				else if (!ft_strncmp((char*)(header +
										shstrtab64->sh_offset +
										cursh64->sh_name), ".shstrtab\0", 10)) {
						infos->shstrtab_offset = cursh64->sh_offset;
						infos->shstrtab_size = cursh64->sh_size;
					}
			}
		}
		shnum--;
	}
	return 0;
}
