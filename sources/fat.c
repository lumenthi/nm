/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 13:17:20 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/14 17:36:36 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*#define FAT_MAGIC	0xcafebabe*/
/*#define FAT_CIGAM	NXSwapLong(FAT_MAGIC)*/

/*struct fat_header {*/
	/*unsigned long	magic;		[> FAT_MAGIC <]*/
	/*unsigned long	nfat_arch;	[> number of structs that follow <]*/
/*};*/

/*struct fat_arch {*/
	/*cpu_type_t	cputype;	[> cpu specifier (int) <]*/
	/*cpu_subtype_t	cpusubtype;	[> machine specifier (int) <]*/
	/*unsigned long	offset;		[> file offset to this object file <]*/
	/*unsigned long	size;		[> size of this object file <]*/
	/*unsigned long	align;		[> alignment as a power of 2 <]*/
/*};*/

void		handle_fat(char *arg, void *file, int cmd) {
	struct fat_header *header = (struct fat_header *)file;
	uint32_t nfat_arch = header->nfat_arch;
	if (header->magic == FAT_CIGAM) {
		nfat_arch = swap_uint32(nfat_arch);
	}
	struct fat_arch *fat_arch = (struct fat_arch *)(header + 1);
	uint32_t i = 0;
	while (i < nfat_arch) {
		uint32_t offset = swap_uint32(fat_arch->offset);
		void *object = file + offset;
		if (nfat_arch > 1) {
			if (i == 0) {
				ft_putchar('\n');
				ft_putstr(arg);
				ft_putstr(" (for architecture ppc):\n");
			}
			else if (i == 1) {
				ft_putchar('\n');
				ft_putstr(arg);
				ft_putstr(" (for architecture i386):\n");
			}
		}
		iterate_commands(arg, object, cmd);
		fat_arch++;
		i++;
	}
}
