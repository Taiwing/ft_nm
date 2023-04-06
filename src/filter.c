#include "ft_nm.h"

#define NM_DEBUG_TYPES		"aAnN"

int	filter_debug_out(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	if (symbol->name[0] == '.')
		return (0);
	return (!ft_strchr(NM_DEBUG_TYPES, symbol->type));
}

int	filter_extern_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	if (symbol->type == 'N')
		return (0);
	return ((symbol->type >= 'A' && symbol->type <= 'Z')
		|| ft_strchr("vw", symbol->type));
}

#define NM_UNDEFINED_TYPES	"Uuvw"

int	filter_undefined_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	return (!!ft_strchr(NM_UNDEFINED_TYPES, symbol->type));
}
