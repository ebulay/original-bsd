/*-
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)edit.c	8.3 (Berkeley) 04/02/94";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <paths.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pw_scan.h>
#include <pw_util.h>

#include "chpass.h"

extern char *tempname;

void
edit(pw)
	struct passwd *pw;
{
	struct stat begin, end;

	for (;;) {
		if (stat(tempname, &begin))
			pw_error(tempname, 1, 1);
		pw_edit(1);
		if (stat(tempname, &end))
			pw_error(tempname, 1, 1);
		if (begin.st_mtime == end.st_mtime) {
			warnx("no changes made");
			pw_error(NULL, 0, 0);
		}
		if (verify(pw))
			break;
		pw_prompt();
	}
}

/*
 * display --
 *	print out the file for the user to edit; strange side-effect:
 *	set conditional flag if the user gets to edit the shell.
 */
void
display(fd, pw)
	int fd;
	struct passwd *pw;
{
	FILE *fp;
	char *bp, *p, *ttoa();

	if (!(fp = fdopen(fd, "w")))
		pw_error(tempname, 1, 1);

	(void)fprintf(fp,
	    "#Changing user database information for %s.\n", pw->pw_name);
	if (!uid) {
		(void)fprintf(fp, "Login: %s\n", pw->pw_name);
		(void)fprintf(fp, "Password: %s\n", pw->pw_passwd);
		(void)fprintf(fp, "Uid [#]: %d\n", pw->pw_uid);
		(void)fprintf(fp, "Gid [# or name]: %d\n", pw->pw_gid);
		(void)fprintf(fp, "Change [month day year]: %s\n",
		    ttoa(pw->pw_change));
		(void)fprintf(fp, "Expire [month day year]: %s\n",
		    ttoa(pw->pw_expire));
		(void)fprintf(fp, "Class: %s\n", pw->pw_class);
		(void)fprintf(fp, "Home directory: %s\n", pw->pw_dir);
		(void)fprintf(fp, "Shell: %s\n",
		    *pw->pw_shell ? pw->pw_shell : _PATH_BSHELL);
	}
	/* Only admin can change "restricted" shells. */
	else if (ok_shell(pw->pw_shell))
		/*
		 * Make shell a restricted field.  Ugly with a
		 * necklace, but there's not much else to do.
		 */
		(void)fprintf(fp, "Shell: %s\n",
		    *pw->pw_shell ? pw->pw_shell : _PATH_BSHELL);
	else
		list[E_SHELL].restricted = 1;
	bp = pw->pw_gecos;
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Full Name: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Location: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Office Phone: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Home Phone: %s\n", p ? p : "");

	(void)fchown(fd, getuid(), getgid());
	(void)fclose(fp);
}

int
verify(pw)
	struct passwd *pw;
{
	ENTRY *ep;
	char *p;
	struct stat sb;
	FILE *fp;
	int len;
	char buf[LINE_MAX];

	if (!(fp = fopen(tempname, "r")))
		pw_error(tempname, 1, 1);
	if (fstat(fileno(fp), &sb))
		pw_error(tempname, 1, 1);
	if (sb.st_size == 0) {
		warnx("corrupted temporary file");
		goto bad;
	}
	while (fgets(buf, sizeof(buf), fp)) {
		if (!buf[0] || buf[0] == '#')
			continue;
		if (!(p = strchr(buf, '\n'))) {
			warnx("line too long");
			goto bad;
		}
		*p = '\0';
		for (ep = list;; ++ep) {
			if (!ep->prompt) {
				warnx("unrecognized field");
				goto bad;
			}
			if (!strncasecmp(buf, ep->prompt, ep->len)) {
				if (ep->restricted && uid) {
					warnx(
					    "you may not change the %s field",
						ep->prompt);
					goto bad;
				}
				if (!(p = strchr(buf, ':'))) {
					warnx("line corrupted");
					goto bad;
				}
				while (isspace(*++p));
				if (ep->except && strpbrk(p, ep->except)) {
					warnx(
				   "illegal character in the \"%s\" field",
					    ep->prompt);
					goto bad;
				}
				if ((ep->func)(p, pw, ep)) {
bad:					(void)fclose(fp);
					return (0);
				}
				break;
			}
		}
	}
	(void)fclose(fp);

	/* Build the gecos field. */
	len = strlen(list[E_NAME].save) + strlen(list[E_BPHONE].save) +
	    strlen(list[E_HPHONE].save) + strlen(list[E_LOCATE].save) + 4;
	if (!(p = malloc(len)))
		err(1, NULL);
	(void)sprintf(pw->pw_gecos = p, "%s,%s,%s,%s", list[E_NAME].save,
	    list[E_LOCATE].save, list[E_BPHONE].save, list[E_HPHONE].save);

	if (snprintf(buf, sizeof(buf),
	    "%s:%s:%d:%d:%s:%ld:%ld:%s:%s:%s",
	    pw->pw_name, pw->pw_passwd, pw->pw_uid, pw->pw_gid, pw->pw_class,
	    pw->pw_change, pw->pw_expire, pw->pw_gecos, pw->pw_dir,
	    pw->pw_shell) >= sizeof(buf)) {
		warnx("entries too long");
		return (0);
	}
	return (pw_scan(buf, pw));
}
