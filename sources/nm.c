/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/20 16:00:47 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/*static void	append(t_section **head, t_section *new) {*/
	/*t_section *tmp = *head;*/
	/*if (*head == NULL)*/
		/**head = new;*/
	/*else {*/
		/*while (tmp->next != NULL)*/
			/*tmp = tmp->next;*/
		/*tmp->next = new;*/
	/*}*/
/*}*/

/*static void	process_section32(Elf32_Shdr *sheader32, t_section **sections)*/
/*{*/
	/*t_section *new = (t_section *)malloc(sizeof(t_section));*/
	/*if (!new) {*/
		/*return;*/
	/*}*/
	/*new->sym_name = NULL;*/
	/*new->sh_name = sheader32->sh_name;*/
	/*new->sh_type = sheader32->sh_type;*/
	/*new->sh_flags = sheader32->sh_flags;*/
	/*new->sh_offset = sheader32->sh_offset;*/
	/*new->arch = 32;*/
	/*new->next = NULL;*/
	/*append(sections, new);*/
/*}*/

/*static void	process_section64(Elf64_Shdr *sheader64, t_section **sections)*/
/*{*/
	/*t_section *new = (t_section *)malloc(sizeof(t_section));*/
	/*if (!new) {*/
		/*return;*/
	/*}*/
	/*new->sym_name = NULL;*/
	/*new->sh_name = sheader64->sh_name;*/
	/*new->sh_type = sheader64->sh_type;*/
	/*new->sh_flags = sheader64->sh_flags;*/
	/*new->sh_offset = sheader64->sh_offset;*/
	/*new->arch = 64;*/
	/*new->next = NULL;*/
	/*append(sections, new);*/
/*}*/

int		process_symbols(void *header, t_Sym_Shdr Sym_Shdr, int arch)
{
	size_t i = 0;
	Elf32_Sym *Sym32 = NULL;
	Elf64_Sym *Sym64 = NULL;
	if (arch == 32) {
		while (i * sizeof(Elf32_Sym) < Sym_Shdr.sh_size) {
			Sym32 = (Elf32_Sym *)(header+Sym_Shdr.sh_offset+i*sizeof(Elf32_Sym));
			printf("Symbol name: %x, size: %d, i: %ld\n", Sym32->st_name, Sym32->st_size, i);
			i++;
		}
	}
	else {
		while (i * sizeof(Elf64_Sym) < Sym_Shdr.sh_size) {
			Sym64 = (Elf64_Sym *)(header+Sym_Shdr.sh_offset+i*sizeof(Elf64_Sym));
			printf("Symbol name: %x, size: %ld, i: %ld\n", Sym64->st_name, Sym64->st_size, i);
			i++;
		}
	}
	return 0;
}

int		symtab_infos(void *header, char *path, int arch, size_t size,
						t_Sym_Shdr *Sym_Shdr)
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
		return -1;
	}
	while (i < shnum) {
		if (((Elf64_Shdr *)(header + shoff + i*shsize))->sh_type == 0x2) { // SYMTAB VALUE
			if (arch == 32) {
				Sym_Shdr->sh_offset = ((Elf32_Shdr *)(header + shoff + i*shsize))->sh_offset;
				Sym_Shdr->sh_size = ((Elf32_Shdr *)(header + shoff + i*shsize))->sh_size;
			}
			else {
				Sym_Shdr->sh_offset = ((Elf64_Shdr *)(header + shoff + i*shsize))->sh_offset;
				Sym_Shdr->sh_size = ((Elf64_Shdr *)(header + shoff + i*shsize))->sh_size;
			}
		}
		i++;
	}
	return 0;
}

void	ft_nm(char *path, void *buffer, size_t size)
{
	Elf32_Ehdr *header = NULL;
	t_Sym_Shdr Sym_Shdr;
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
		if (symtab_infos((void*)header, path, arch, size, &Sym_Shdr) == -1)
			return;
		printf("Found symbol table at offset: %lx with size: %lx\n", Sym_Shdr.sh_offset, Sym_Shdr.sh_size);
		process_symbols((void*)header, Sym_Shdr, arch);
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
