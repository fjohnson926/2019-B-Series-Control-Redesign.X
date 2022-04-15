#include "sprintfoverride.h"

/* Override sprintf implement to optimize */

static const unsigned m_val[] = {1000000000u, 100000000u, 10000000u, 1000000u, 100000u, 10000u, 1000u, 100u, 10u, 1u};
int                   sprintu(char *s, unsigned u)
{
	char tmp_buf[12];
	int  i, n = 0;
	int  m;

	if (u == 0) {
		*s = '0';
		return 1;
	}

	for (i = 0; i < 10; i++) {
		for (m = 0; m < 10; m++) {
			if (u >= m_val[i]) {
				u -= m_val[i];
			} else {
				break;
			}
		}
		tmp_buf[i] = m + '0';
	}
	for (i = 0; i < 10; i++) {
		if (tmp_buf[i] != '0') {
			break;
		}
	}
	for (; i < 10; i++) {
		*s++ = tmp_buf[i];
		n++;
	}
	return n;
}

int sprintf(char *s, const char *fmt, ...)
{
	int     n = 0;
	va_list ap;
	va_start(ap, fmt);
	while (*fmt) {
		if (*fmt != '%') {
			*s = *fmt;
			s++;
			fmt++;
			n++;
		} else {
			fmt++;
			switch (*fmt) {
			case 'c': {
				char valch = va_arg(ap, int);
				*s         = valch;
				s++;
				fmt++;
				n++;
				break;
			}
			case 'd': {
				int vali = va_arg(ap, int);
				int nc;

				if (vali < 0) {
					*s++ = '-';
					n++;
					nc = sprintu(s, -vali);
				} else {
					nc = sprintu(s, vali);
				}

				s += nc;
				n += nc;
				fmt++;
				break;
			}
			case 'u': {
				unsigned valu = va_arg(ap, unsigned);
				int      nc   = sprintu(s, valu);
				n += nc;
				s += nc;
				fmt++;
				break;
			}
			case 's': {
				char *vals = va_arg(ap, char *);
				while (*vals) {
					*s = *vals;
					s++;
					vals++;
					n++;
				}
				fmt++;
				break;
			}
			default:
				*s = *fmt;
				s++;
				fmt++;
				n++;
			}
		}
	}
	va_end(ap);
	*s = 0;
	return n;
}
