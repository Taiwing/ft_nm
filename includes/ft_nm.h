/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:42 by yforeau           #+#    #+#             */
/*   Updated: 2023/03/29 18:08:38 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <elf.h>
#include <linux/elf-em.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <endian.h>
#include "libft.h"

// ft_nm short options
#define	FT_NM_OPT		"eagurph"

// ELF max valid value
#define ELF_OSABI_MAX	0x12

/*
** ft_nm constants
*/
extern const t_opt		g_nm_opt[];
extern const char		*g_nm_help[];
extern const char		*g_nm_usage[];
extern const char		*g_nm_osabi[ELF_OSABI_MAX + 1];
extern const char		*g_nm_type[];
extern const char		*g_nm_machine[];

#define EM_MACHINE_MAX	EM_CSKY

// symbol sorts
enum e_nm_sorts { E_SORT_NONE = 0, E_SORT_DEFAULT, E_SORT_REVERSE };

/*
** ft_nm's global configuration
*/
typedef struct		s_nm_config
{
	const char		*exec;			// executable name
	int				elf_mode;		// boolean set to 1 if e option is passed
	enum e_nm_sorts	sort;			// symbol sort
	int				exit_status;	// value to exit on (success or failure)
}					t_nm_config;

/*
** Unionize elf headers, because it's easier this way and because Karl said so
*/
typedef union	u_elf_hdr
{
	Elf32_Ehdr	hdr32;
	Elf64_Ehdr	hdr64;
}				s_elf_hdr;

/*
** ft_nm's file type
*/
typedef struct	s_nm_file
{
	void		*data;			// file contents
	size_t		size;			// size of the file
	int			class;			// ELFCLASS32 or ELFCLASS64
	s_elf_hdr	elf;			// elf header loaded from file
}				t_nm_file;

/*
** functions
*/
char	**parse_arguments(t_nm_config *cfg, int argc, char **argv);
int		read_elf_header(t_nm_file *dest, t_nm_config *cfg);
void	print_elf32_header(Elf32_Ehdr *hdr);
void	print_elf64_header(Elf64_Ehdr *hdr);
