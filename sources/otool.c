/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 11:23:25 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/14 17:43:31 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void		usage(void) {
	ft_putstr_fd("Usage: otool <object file>\n", STDERR_FILENO);
}

static void		empty_list(void) {
	ft_putstr_fd("error: otool:", STDERR_FILENO);
	ft_putstr_fd(" at least one file must be specified\n", STDERR_FILENO);
	usage();
}

void	otool(char *path) {
	void	*buffer;
	size_t	size;
	struct mach_header *s_header;

	size = 0;
	s_header = NULL;
	buffer = map_file(path, &size);
	if (buffer) {
		s_header = (struct mach_header *)buffer;
		iterate_commands(path, s_header, 2);
		munmap(buffer, size);
	}
}

int		main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	int i;

	i = 1;
	if (argc < 2)
		empty_list();
	else {
		while (i < argc)
			otool(argv[i++]);
	}
	return 0;
}
