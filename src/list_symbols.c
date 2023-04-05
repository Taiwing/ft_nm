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

#define NM_UNDEFINED_TYPES	"Uuvw"
#define NM_VALUE_PADDING_32	8
#define NM_VALUE_PADDING_64	16

static void			print(t_list *symbols, t_nm_file *file)
{
	for (t_list *ptr = symbols; ptr; ptr = ptr->next)
	{
		t_nm_symbol	*symbol = ptr->content;
		if (ft_strchr(NM_UNDEFINED_TYPES, symbol->type))
			ft_printf("%*c %c %s\n", file->class == ELFCLASS32
				? NM_VALUE_PADDING_32 : NM_VALUE_PADDING_64,
				' ', symbol->type, symbol->name);
		else
			ft_printf("%0*llx %c %s\n", file->class == ELFCLASS32
				? NM_VALUE_PADDING_32 : NM_VALUE_PADDING_64,
				symbol->value, symbol->type, symbol->name);
	}
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
	print(symbols, file);
	// CLEAN
	ft_lstdel(&symbols, delete_symbol);
	return (0);
}
