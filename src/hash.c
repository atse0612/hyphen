#include <string.h>

#include "hash.h"

static char *tab[HASH_MAX];

int hash(const char *s)
{
	const char *p = s;
	unsigned h = 0;
	while (*p) h = 31*h + *p++;
	h %= HASH_MAX;

	unsigned init = h;
	while (tab[h] && strcmp(tab[h], s)) {
		h = (h + 1) % HASH_MAX;
		if (h == init) return -1;
	}

	if (!tab[h]) tab[h] = strdup(s);
	return h;
}

const char *unhash(unsigned h)
{
	return h > HASH_MAX ? 0 : tab[h];
}
