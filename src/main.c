/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:47 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/17 20:36:35 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		swap_elf32_header(Elf32_Ehdr *dest)
{
	ft_memswap(&dest->e_type, sizeof(dest->e_type));
	ft_memswap(&dest->e_machine, sizeof(dest->e_machine));
	ft_memswap(&dest->e_version, sizeof(dest->e_version));
	ft_memswap(&dest->e_entry, sizeof(dest->e_entry));
	ft_memswap(&dest->e_phoff, sizeof(dest->e_phoff));
	ft_memswap(&dest->e_shoff, sizeof(dest->e_shoff));
	ft_memswap(&dest->e_flags, sizeof(dest->e_flags));
	ft_memswap(&dest->e_ehsize, sizeof(dest->e_ehsize));
	ft_memswap(&dest->e_phentsize, sizeof(dest->e_phentsize));
	ft_memswap(&dest->e_phnum, sizeof(dest->e_phnum));
	ft_memswap(&dest->e_shentsize, sizeof(dest->e_shentsize));
	ft_memswap(&dest->e_shnum, sizeof(dest->e_shnum));
	ft_memswap(&dest->e_shstrndx, sizeof(dest->e_shstrndx));
}

static void		swap_elf64_header(Elf64_Ehdr *dest)
{
	ft_memswap(&dest->e_type, sizeof(dest->e_type));
	ft_memswap(&dest->e_machine, sizeof(dest->e_machine));
	ft_memswap(&dest->e_version, sizeof(dest->e_version));
	ft_memswap(&dest->e_entry, sizeof(dest->e_entry));
	ft_memswap(&dest->e_phoff, sizeof(dest->e_phoff));
	ft_memswap(&dest->e_shoff, sizeof(dest->e_shoff));
	ft_memswap(&dest->e_flags, sizeof(dest->e_flags));
	ft_memswap(&dest->e_ehsize, sizeof(dest->e_ehsize));
	ft_memswap(&dest->e_phentsize, sizeof(dest->e_phentsize));
	ft_memswap(&dest->e_phnum, sizeof(dest->e_phnum));
	ft_memswap(&dest->e_shentsize, sizeof(dest->e_shentsize));
	ft_memswap(&dest->e_shnum, sizeof(dest->e_shnum));
	ft_memswap(&dest->e_shstrndx, sizeof(dest->e_shstrndx));
}

static int		load_elf32_header(Elf32_Ehdr *dest, Elf32_Ehdr *raw)
{
	if (ft_memcmp(raw->e_ident, ELFMAG, 4)
		|| (raw->e_ident[EI_DATA] != ELFDATA2LSB
		&& raw->e_ident[EI_DATA] != ELFDATA2MSB)
		|| raw->e_ident[EI_VERSION] != EV_CURRENT)
		return (1);
	ft_memcpy(dest, raw, sizeof(Elf32_Ehdr));
	if ((__BYTE_ORDER == __LITTLE_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2MSB)
		|| (__BYTE_ORDER == __BIG_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2LSB))
		swap_elf32_header(dest);
	if (dest->e_version != (uint32_t)dest->e_ident[EI_VERSION]
		|| dest->e_ehsize != sizeof(Elf32_Ehdr))
		return (1);
	//TODO: check the other fields
	return (0);
}

static int		load_elf64_header(Elf64_Ehdr *dest, Elf64_Ehdr *raw)
{
	if (ft_memcmp(raw->e_ident, ELFMAG, 4)
		|| (raw->e_ident[EI_DATA] != ELFDATA2LSB
		&& raw->e_ident[EI_DATA] != ELFDATA2MSB)
		|| raw->e_ident[EI_VERSION] != EV_CURRENT)
		return (1);
	ft_memcpy(dest, raw, sizeof(Elf64_Ehdr));
	if ((__BYTE_ORDER == __LITTLE_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2MSB)
		|| (__BYTE_ORDER == __BIG_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2LSB))
		swap_elf64_header(dest);
	if (dest->e_version != (uint32_t)dest->e_ident[EI_VERSION]
		|| dest->e_ehsize != sizeof(Elf64_Ehdr))
		return (1);
	//TODO: check the other fields
	return (0);
}

static int		read_elf_header(t_nm_file *dest, t_nm_config *cfg)
{
	dest->class = ((Elf32_Ehdr *)dest->data)->e_ident[EI_CLASS];
	if (dest->class != ELFCLASS32 && dest->class != ELFCLASS64)
		return (!!ft_dprintf(2, "%s: invalid elf header\n", cfg->exec));
	else if (dest->class == ELFCLASS32
		&& load_elf32_header(&dest->elf.hdr32, dest->data))
		return (!!ft_dprintf(2, "%s: invalid elf header\n", cfg->exec));
	else if (dest->class == ELFCLASS64
		&& load_elf64_header(&dest->elf.hdr64, dest->data))
		return (!!ft_dprintf(2, "%s: invalid elf header\n", cfg->exec));
	return (0);
}

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
