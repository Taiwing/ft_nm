/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:42 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 19:32:41 by yforeau          ###   ########.fr       */
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

// symbol filters
#define FILTER_DEBUG		(1 << 0)
#define FILTER_EXTERN		(1 << 1)
#define FILTER_UNDEFINED	(1 << 2)

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
	unsigned int	filter;			// symbol filter
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
** Unionize elf sections header pointers
*/
typedef union	u_elf_shdr
{
	Elf32_Shdr	*hdr32;
	Elf64_Shdr	*hdr64;
}				s_elf_shdr;

/*
** ft_nm's file type
*/
typedef struct	s_nm_file
{
	void		*data;			// file contents
	size_t		size;			// size of the file
	int			class;			// ELFCLASS32 or ELFCLASS64
	s_elf_hdr	elf;			// elf header loaded from file
	s_elf_shdr	sections;		// section headers loaded from file
	size_t		sections_count;	// count of section headers
	char		*strtab;		// string table loaded from file
	size_t		strtab_size;	// size of the string table
	char		*shstrtab;		// section string table loaded from file
	size_t		shstrtab_size;	// size of the section string table
}				t_nm_file;

/*
** ft_nm symbol
*/
typedef struct	s_nm_symbol
{
	char		*name;			// symbol name string
	char		type;			// nm type letter
	uint64_t	value;			// symbol value
	int			idx;			// symbol index
}				t_nm_symbol;

/*
** functions
*/
char	**parse_arguments(t_nm_config *cfg, int argc, char **argv);
int		read_elf_header(t_nm_file *dest, t_nm_config *cfg);
void	print_elf32_header(Elf32_Ehdr *hdr);
void	print_elf64_header(Elf64_Ehdr *hdr);
int		list_symbols(t_nm_file *file, t_nm_config *cfg);
int		list32(t_list **dest, t_nm_file *file);
int		list64(t_list **dest, t_nm_file *file);
t_list	*push_symbol32(t_list **dest, Elf32_Sym *elf_symbol,
	t_nm_file *file, int idx);
t_list	*push_symbol64(t_list **dest, Elf64_Sym *elf_symbol,
	t_nm_file *file, int idx);
