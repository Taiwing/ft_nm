/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:18:20 by yforeau           #+#    #+#             */
/*   Updated: 2023/03/29 18:48:43 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const t_opt	g_nm_opt[] = {
	{ "elf",					0,	NULL,	'e'	},
	{ "debug-syms",				0,	NULL,	'a'	},
	{ "extern-only",			0,	NULL,	'g'	},
	{ "undefined-only",			0,	NULL,	'u'	},
	{ "reverse-sort",			0,	NULL,	'r'	},
	{ "no-sort",				0,	NULL,	'p'	},
	{ "help",					0,	NULL,	'h'	},
	{ NULL,						0,	NULL,	0	},
};

const char	*g_nm_help[] = {
	"Print elf header (same behavior as readelf -h).",
	"Display all symbols, including debugger-only symbols.",
	"Display only external symbols.",
	"Display only undefined symbols (those external to each object file).",
	"Reverse the order of the sort.",
	"Do not sort symbols, print them in the order encountered.",
	"Print this.",
	NULL,
};

const char	*g_nm_usage[] = {
	"[-" FT_NM_OPT "] host ...",
	NULL,
};

static void		usage(const char *exec, int exit_value)
{
	const t_opt	*opts = g_nm_opt;
	const char	**help = g_nm_help;
	const char	**usage = g_nm_usage;

	ft_printf("Usage:\n");
	while (*usage)
	{
		ft_printf("\t%s %s\n", exec, *usage);
		++usage;
	}
	ft_printf("\nOptions:\n");
	while (opts->name && *help)
	{
		if (ft_isalnum(opts->val))
			ft_printf("\t-%c, --%s\n", opts->val, opts->name);
		else
			ft_printf("\t--%s\n", opts->name);
		ft_printf("\t\t%s\n", *help);
		++opts;
		++help;
	}
	ft_exit(exit_value, NULL);
}

char			**parse_arguments(t_nm_config *cfg, int argc, char **argv)
{
	int			opt;
	t_optdata	optd = { 0 };

	init_getopt(&optd, FT_NM_OPT, (t_opt *)g_nm_opt, NULL);
	while ((opt = ft_getopt_long(argc, argv, &optd)) >= 0)
		switch (opt)
		{
			case 'e': cfg->elf_mode = 1;								break;
			case 'a': cfg->filter ^= FILTER_DEBUG;						break;
			case 'g': cfg->filter |= FILTER_EXTERN;						break;
			case 'u': cfg->filter |= FILTER_UNDEFINED;					break;
			case 'r': cfg->sort = E_SORT_REVERSE;						break;
			case 'p': cfg->sort = E_SORT_NONE;							break;
			//TODO: parse other options
			default: usage(cfg->exec, opt != 'h');						break;
		}
	return (argv + optd.optind);
}
