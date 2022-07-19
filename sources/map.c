/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 12:50:11 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/19 13:34:11 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

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
