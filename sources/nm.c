/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 10:51:30 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/14 12:42:52 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	nm(char *path) {
	void	*buffer;
	size_t	size;
	struct mach_header *s_header;

	size = 0;
	s_header = NULL;
	buffer = map_file(path, &size);
	if (buffer) {
		s_header = (struct mach_header *)buffer;
		iterate_commands(path, s_header, 1);
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
