/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 17:25:07 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:34 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	invalid_file(char *file) {
	ft_putstr_fd("error: nm: can't open file: ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(" (No such file or directory)\n", STDERR_FILENO);
}

static size_t	file_size(int fd) {
	struct stat		s_stats;
	size_t			size;

	size = 0;
	if (fstat(fd, &s_stats) != -1)
		size = s_stats.st_size;
	return size;
}

void	*map_file(char *path, size_t *size) {
	void	*buffer;
	int		fd;

	fd = open(path, O_RDONLY);
	buffer = NULL;
	if (fd == -1)
		invalid_file(path);
	else {
		if ((*size = file_size(fd))) {
			buffer = mmap(0, *size, PROT_READ, MAP_PRIVATE, fd, 0);
			if (buffer == MAP_FAILED) {
				ft_putstr_fd("error: nm: can't map file: " , STDERR_FILENO);
				ft_putstr_fd(path, STDERR_FILENO);
				ft_putstr_fd(" (Invalid argument)\n", STDERR_FILENO);
				buffer = NULL;
			}
		}
		close(fd);
	}
	return buffer;
}

int		iterate_commands(char *arg, struct mach_header *header, int cmd) {
	t_found_section				*parsed_sections;
	t_parsed_symbol				*parsed_symbols;

	parsed_sections = NULL;
	parsed_symbols = NULL;
	// ft_puthex(1, 0, header->magic);
	// ft_putchar('\n');
	if (header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64) { // MACHO 64 BITS HEADER
		// ft_putstr("macho64\n");
		handle_macho(header, &parsed_symbols, &parsed_sections, 64);
	}
	else if (header->magic == MH_MAGIC || header->magic == MH_CIGAM) { // MACHO 32 BITS HEADER
		// ft_putstr("macho32\n");
		handle_macho(header, &parsed_symbols, &parsed_sections, 32);
	}
	else if (header->magic == FAT_MAGIC || header->magic == FAT_CIGAM) {
		// ft_putstr("fat\n");
		handle_fat(arg, header, cmd);
	}
	else if (!ft_strncmp((char*)&header->magic, ARMAG, SARMAG)) // AR FILE (static library)
		handle_archive(arg, header, cmd);
	else {
		ft_putstr_fd("error: nm: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(", format not supported.\n", STDERR_FILENO);
		return 0;
	}
	// print_sections(parsed_sections);
	if (cmd == 1) { // nm
		symbols_sort_alpha(&parsed_symbols);
		display_symbols(parsed_symbols, parsed_sections);
	}
	else if (cmd == 2) { // otool
		display_section((void *)header, "__text", parsed_sections);
	}
	delete_sections(&parsed_sections);
	delete_symbols(&parsed_symbols);
	return 1;
}
