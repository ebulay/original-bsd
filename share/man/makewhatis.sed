#!/bin/sh -
#
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
#
# %sccs.include.redist.sh%
#
#	@(#)makewhatis.sed	8.2 (Berkeley) 11/16/93
#

/^[a-zA-Z][a-zA-Z0-9]*(\([a-zA-Z0-9]*\).*/ {
	s;^[a-zA-Z0-9]*(\([a-zA-Z0-9]*\).*;\1;
	h
	d
}

/^NNAAMMEE/!d

:name
	s;.*;;
	N
	s;\n;;
	# some twits underline the command name
	s;_;;g
	/^[^	 ]/b print
	H
	b name

:print
	x
	s;\n;;g
	/-/!d
	s;.;;g
	s;\([a-z][A-z]\)-[	 ][	 ]*;\1;
	s;\([a-zA-Z0-9,]\)[	 ][	 ]*;\1 ;g
	s;[^a-zA-Z0-9]*\([a-zA-Z0-9]*\)[^a-zA-Z0-9]*\(.*\) - \(.*\);\2 (\1) - \3;
	p
	q
