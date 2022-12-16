/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:42 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/16 17:10:50 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"

// ft_nm short options
#define	FT_NM_OPT	"eagurph"

/*
** ft_nm constants
*/
extern const t_opt		g_nm_opt[];
extern const char		*g_nm_help[];
extern const char		*g_nm_usage[];

/*
** ft_nm's global configuration
*/
typedef struct	s_nm_config
{
	const char	*exec;			// executable name
	int			elf_mode;		// boolean set to 1 if e option is passed
	int			exit_status;	// value to exit on (success or failure)
}				t_nm_config;

/*
** ft_nm's file type
*/
typedef struct	s_nm_file
{
	void		*data;	// file contents
	size_t		size;	// size of the file
	int			type;	// ELFCLASS32 or ELFCLASS64
	uint64_t	shoff;	// section header offset
}				t_nm_file;

/*
** functions
*/
char	**parse_arguments(t_nm_config *cfg, int argc, char **argv);
