/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 12:50:11 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/28 12:31:14 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static size_t	file_size(int fd)
{
	struct stat		s_stats;
	size_t			size;

	size = 0;
	if (fstat(fd, &s_stats) != -1)
		size = s_stats.st_size;
	return size;
}

void	*map_file(char *path, size_t *size)
{
	void	*buffer;
	int		fd;

	fd = open(path, O_RDONLY);
	buffer = NULL;
	if (fd == -1)
		error("No such file", path);
	else {
		if ((*size = file_size(fd))) {
			buffer = mmap(0, *size, PROT_READ, MAP_PRIVATE, fd, 0);
			if (buffer == MAP_FAILED) {
				error("file is a directory or mmap failed", path);
				buffer = NULL;
			}
		}
		close(fd);
	}
	return buffer;
}
