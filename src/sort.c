#include "ft_nm.h"

int	sort_symbols_default(void *a, void *b)
{
	t_nm_symbol	*sym_a = a, *sym_b = b;
	int			cmp = strcoll(sym_a->name, sym_b->name);

	return (cmp ? cmp : sym_a->idx - sym_b->idx);
}

int	sort_symbols_reverse(void *a, void *b)
{
	t_nm_symbol	*sym_a = a, *sym_b = b;
	int			cmp = strcoll(sym_b->name, sym_a->name);

	return (cmp ? cmp : sym_a->idx - sym_b->idx);
}