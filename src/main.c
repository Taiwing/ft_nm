/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:47 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/16 17:41:53 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		check_elf32(Elf32_Ehdr *hdr)
{
	if (ft_memcmp(hdr->e_ident, ELFMAG, 4)
		|| !(hdr->e_ident[EI_DATA] == ELFDATA2LSB
		|| hdr->e_ident[EI_DATA] == ELFDATA2MSB))
		return (1);
	return (0);
}

int		read_elf_header(t_nm_file *dest, t_nm_config *cfg)
{
	dest->type = ((Elf32_Ehdr *)dest->data)->e_ident[EI_CLASS];
	if (dest->type != ELFCLASS32 && dest->type != ELFCLASS64)
		return (!!ft_dprintf(2, "%s: invalid elf header\n", cfg->exec));
	//TODO: check header 32 or header 64 and load shoff value
	return (0);
}

int		get_binary_file(t_nm_file *dest, char *path, t_nm_config *cfg)
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
	else if ((dest->size = st.st_size) < sizeof(Elf32_Ehdr))
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
	/* TODO:
	** - open file and load binary data [DONE]
	** - read elf magic number
	** - read and load the rest of the elf header in the appropriate struct
	** - if '-e' just print the header and LFG
	** - read the rest of the file and print symbols in the required order
	** - free the file and return
	*/
	t_nm_file	file = { 0 };

	if (get_binary_file(&file, path, cfg) || read_elf_header(&file, cfg))
	{
		cfg->exit_status = EXIT_FAILURE;
		if (file.data && munmap(file.data, file.size) < 0)
			ft_exit(EXIT_FAILURE, "munmap: %s", strerror(errno));
		return ;
	}
	//TODO: do the things, all the things
	if (munmap(file.data, file.size) < 0)
		ft_exit(EXIT_FAILURE, "munmap: %s", strerror(errno));
}

int		main(int argc, char **argv)
{
	char		**args = NULL;
	t_nm_config	cfg = {
		.exec = *argv,
		.exit_status = EXIT_SUCCESS,
	};

	ft_exitmsg((char *)cfg.exec);
	args = parse_arguments(&cfg, argc, argv);
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
