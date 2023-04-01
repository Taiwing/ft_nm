/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 18:52:16 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 21:52:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int			list(t_list **dest, t_nm_file *file)
{
	if (file->class == ELFCLASS32)
		return (list32(dest, file));
	else if (file->class == ELFCLASS64)
		return (list64(dest, file));
	return (1);
}

int					list_symbols(t_nm_file *file, t_nm_config *cfg)
{
	t_list	*symbols = NULL;

	(void)cfg;
	// LIST
	if (list(&symbols, file) || !symbols)
	{
		ft_lstdel(&symbols, delete_symbol);
		return (1);
	}
	// SORT (with strcoll)
	// PRINT
	for (t_list *ptr = symbols; ptr; ptr = ptr->next)
	{
		t_nm_symbol	*symbol = ptr->content;
		ft_printf("%016llx %c %s\n", symbol->value, symbol->type, symbol->name);
	}
	// CLEAN
	ft_lstdel(&symbols, delete_symbol);
	return (0);
}
