#ifndef _AC_H_
#define _AC_H_

typedef struct ac ac_t;
struct acmat {
	const char *pos;
	const char *pat;
};

ac_t *acnew(void);
void acadd(ac_t *ac, const char *pat);
ac_t *acpre(ac_t *ac);
int acmat(ac_t *ac, struct acmat *mat, const char *txt);
void acdes(ac_t *ac);

#endif
