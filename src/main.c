/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:47 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 21:10:00 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <locale.h>
#include "ft_nm.h"

static int		get_binary_file(t_nm_file *dest, char *path, t_nm_config *cfg)
{
	int			fd;
	struct stat	st;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (!!ft_dprintf(2, "%s: %s: %s\n",
			cfg->exec, path, strerror(errno)));
	else if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (!!ft_dprintf(2, "%s: %s\n", cfg->exec, strerror(errno)));
	}
	else if ((dest->size = st.st_size) < EI_NIDENT)
	{
		close(fd);
		return (!!ft_dprintf(2, "%s: file too small\n", cfg->exec));
	}
	else if ((dest->data = mmap(0, dest->size, PROT_READ, MAP_PRIVATE, fd, 0))
		== MAP_FAILED)
		ft_exit(EXIT_FAILURE, "mmap: %s", strerror(errno));
	close(fd);
	return (0);
}

void	nm(t_nm_config *cfg, char *path)
{
	t_nm_file	file = { 0 };

	if (get_binary_file(&file, path, cfg) || read_elf_header(&file, cfg)
		|| (!cfg->elf_mode && list_symbols(&file, cfg)))
	{
		cfg->exit_status = EXIT_FAILURE;
		if (file.data && munmap(file.data, file.size) < 0)
			ft_exit(EXIT_FAILURE, "munmap: %s", strerror(errno));
		return ;
	}
	if (cfg->elf_mode)
	{
		if (file.class == ELFCLASS32)
			print_elf32_header(&file.elf.hdr32);
		else
			print_elf64_header(&file.elf.hdr64);
	}
	if (munmap(file.data, file.size) < 0)
		ft_exit(EXIT_FAILURE, "munmap: %s", strerror(errno));
}

int		main(int argc, char **argv)
{
	char		**args = NULL;
	t_nm_config	cfg = {
		.exec = ft_exec_name(*argv),
		.sort = E_SORT_DEFAULT,
		.filter = FILTER_DEBUG,
		.exit_status = EXIT_SUCCESS,
	};

	ft_exitmsg((char *)cfg.exec);
	args = parse_arguments(&cfg, argc, argv);
	setlocale(LC_COLLATE, "");
	if (!args[0] || !args[1])
		nm(&cfg, args[0] ? args[0] : "a.out");
	else
		while (*args)
		{
			ft_printf("\n%s:\n", *args);
			nm(&cfg, *args++);
		}
	ft_exit(cfg.exit_status, NULL);
	return (EXIT_SUCCESS);
}
