#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <ctype.h>

#include "hash.h"
#include "ac.h"

#define _S(x) "%"#x"s"
#define S(x) _S(x)

#define PATH_MAX 4096
#define PAT_MAXN HASH_MAX
#define PAT_MAXL 127
#define WRD_MAXL 127

#define ERR_OPT 1
#define ERR_LIM 2

#define MAX(x, y) ((x) > (y) ? (x) : (y))

static char patfl[PATH_MAX + 1] = DESTDIR"/usr/local/share/hyphen/pattern";

static ac_t *pat;
static int patwei[PAT_MAXN][PAT_MAXL + 1];

static void prsarg(int argc, char **argv);
static void ldpat(void);
static char *hyphen(char *dst, const char *src);
static int help(int ret);
static int about(void);

int main(int argc, char **argv)
{
	prsarg(argc, argv);
	ldpat();

	char wrd[WRD_MAXL + 1];
	while (EOF != scanf(S(WRD_MAXL), wrd)) {
		char buf[2*WRD_MAXL + 1];
		puts(hyphen(buf, wrd));
	}

	return 0;
}

static void prsarg(int argc, char **argv)
{
	char c;
	while ((c = getopt(argc, argv, "p:hv")) != -1)
		switch (c) {
			case 'p':
				strncpy(patfl, optarg, sizeof patfl);
				patfl[sizeof patfl - 1] = 0;
				break;
			case 'h':
				exit(help(0));
			case 'v':
				exit(about());
			default:
				exit(help(ERR_OPT));
		}
}

static void ldpat(void)
{
	pat = acnew();
	FILE *pf = fopen(patfl, "r");
	char p[PAT_MAXL + 1];
	while (EOF != fscanf(pf, S(PAT_MAXL), p)) {
		char ps[PAT_MAXL + 1] = {0};
		int pw[PAT_MAXL + 1] = {0};
		int n = 0;

		for (char *s = p; *s; ++s)
			if (isdigit(*s))
				pw[n] = *s - '0';
			else
				ps[n++] = *s;

		acadd(pat, ps);
		int h = hash(ps);
		if (h < 0) exit(help(ERR_LIM));
		memcpy(patwei[h], pw, sizeof patwei[h]);
	}
	acpre(pat);
	fclose(pf);
}

static char *hyphen(char *dst, const char *src)
{
	char lsrc[WRD_MAXL + 2 + 1] = {0};
	sprintf(lsrc, ".%s.", src);
	for (char *s = lsrc; *s; ++s) *s = tolower(*s);

	struct acmat mat[WRD_MAXL * WRD_MAXL];
	int nm = acmat(pat, mat, lsrc);
	int w[WRD_MAXL + 1] = {0};

	for (int i = 0; i < nm; ++i) {
		struct acmat *m = mat + i;
		//printf("%s: %s\n", m->pos, m->pat);
		int h = hash(m->pat);
		int p = m->pos - lsrc;
		int l = strlen(m->pat);
		for (int j = 0; j <= l; ++j) w[p + j] = MAX(w[p + j], patwei[h][j]);
	}

	char *pd = dst;
	for (char *s = lsrc + 1; *s != '.'; ++s) {
		if (s - lsrc > 2 && *(s + 1) != '.' && 0 != w[s - lsrc] % 2) *pd++ = '-';
		*pd++ = src[s - lsrc - 1];
	}

	*pd = 0;
	return dst;
}

static int help(int ret)
{
	puts(
		"USAGE:\n"
		"	hyphen [-p pattern-file] [-h | -v]\n"
		"\n"
		"OPTIONS:\n"
		"	-p pattern-file\n"
		"		The pattern-file contains patterns which indicates hyphen how to hyphenate words\n"
		"		If not specified, "DESTDIR"/usr/local/share/hyphen/pat will be used\n"
		"	-h\n"
		"		Print this brief manumal\n"
		"	-v\n"
		"		Print the version"
	);

	return ret;
}

static int about(void)
{
	puts(
		"hyphen 0.0.0\n"
		"Written by Yuxuan Dong <www.dyx.name>"
	);

	return 0;
}
