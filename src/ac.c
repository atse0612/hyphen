#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ac.h"

typedef unsigned char uchar;

struct lnk {
	ac_t *val;
	struct lnk *prv;
	struct lnk *nxt;
};

static ac_t *pop(struct lnk **hd);
static void push(struct lnk **hd, ac_t *val);

struct ac {
	char chr;
	ac_t *par;
	ac_t *st[UCHAR_MAX + 1];
	ac_t *fl;
	ac_t *out;
	char *str;
	int len;
};

ac_t *acnew(void)
{
	ac_t *ret = memset(malloc(sizeof(ac_t)), 0, sizeof(ac_t));
	return ret->fl = ret;
}

void acadd(ac_t *ac, const char *pat)
{
	uchar c = (uchar)*pat;
	if (!ac->st[c]) {
		ac_t *s = ac->st[c] = acnew();
		s->chr = c;
		s->par = ac;
		s->len = ac->len + 1;
		if (!c) {
			s->str = (char *)malloc(s->len);
			ac_t *p = s;
			for (int i = s->len - 1; i >= 0; --i, p = p->par)
				s->str[i] = p->chr;
		}
	}
	if (c) acadd(ac->st[c], pat + 1);
}

ac_t *acpre(ac_t *ac)
{
	struct lnk *hd = (struct lnk *)malloc(sizeof *hd);
	struct lnk *tl = hd;
	memset(hd, 0, sizeof *hd);
	push(&tl, ac);

	while (hd != tl) {
		ac_t *cur = pop(&hd);

		if (cur == ac || cur->par == ac)
			cur->out = cur->fl = ac;
		else {
			uchar uc = (uchar)cur->chr;

			ac_t *p = cur->par->fl;
			while (p != ac && !p->st[uc]) p = p->fl;
			if (p->st[uc]) p = p->st[uc];
			cur->fl = p;

			while (p->par && !*p->st) p = p->fl;
			cur->out = p;
		}

		for (int i = 0; i <= UCHAR_MAX; ++i)
			if (cur->st[i])
				push(&tl, cur->st[i]);
	}

	free(hd);
	return ac;
}

int acmat(ac_t *ac, struct acmat *mat, const char *txt)
{
	int ret = 0;

	for(const uchar *t = (const uchar *)txt; *t; ++t) {
		while (!ac->st[*t] && ac->par) ac = ac->fl;
		if (ac->st[*t]) {
			ac = ac->st[*t];
			if (*ac->st)
				mat[ret++] = (struct acmat){(const char *)t - ac->len + 1, ac->st[0]->str};
			for (ac_t *p = ac->out; p->par; p = p->out)
				mat[ret++] = (struct acmat){(const char *)t - p->len + 1, p->st[0]->str};
		}
	}

	return ret;
}

void acdes(ac_t *ac)
{
	for (int i = 0; i <= UCHAR_MAX; ++i)
		if (ac->st[i])
			acdes(ac->st[i]);
	if (ac->str) free(ac->str);
	free(ac);
}

static ac_t *pop(struct lnk **hd)
{
	struct lnk *nxt = (*hd)->nxt;
	nxt->prv = 0;

	ac_t *ret = (*hd)->val;
	free(*hd);
	*hd = nxt;
	return ret;
}

static void push(struct lnk **tl, ac_t *val)
{
	(*tl)->val = val;
	struct lnk *nd = (struct lnk *)malloc(sizeof *nd);
	memset(nd, 0, sizeof *nd);
	nd->prv = *tl;
	(*tl)->nxt = nd;
	*tl = nd;
}
