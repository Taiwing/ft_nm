/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:47 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/15 12:17:55 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	nm(t_nm_config *cfg, char *file)
{
	/* TODO:
	** - open file
	** - read elf magic number
	** - read and load the rest of the elf header in the appropriate struct
	** - if '-e' just print the header and LFG
	** - read the rest of the file and print symbols in the required order
	*/
	(void)cfg;
	(void)file;
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
