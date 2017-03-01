##
## @file Makefile
## Betriebsysteme myfind Makefile
## Beispiel 1
## @author Alexander Pirka <alexander.pirka@technikum-wien.at>
## @date 2017/02/26
##
## @version $Draft$
##
## @todo
## - understand details
## - find out why gcc52 is leading to compiler error on my local maschine
##
## URL: $HeadURL$
##
## Last Modified: $Author$
##
##
## ------------------------------------------------------------- variables --
##

INSTALL_BIN_DIR := /usr/local/bin
INSTALL_MAN_DIR := /usr/local/man/man1
INSTALL_ETC_DIR := /usr/local/etc
INSTALL_LIB_DIR := /usr/local/lib
INSTALL_SRC_DIR := /usr/local/src
INSTALL_DOC_DIR := /var/www/html

##gcc52 leading to compiler error
CC=gcc52
AWK=awk

CFLAGS    += -Wall
CFLAGS    += -Werror
CFLAGS    += -Wstrict-prototypes
CFLAGS    += -fno-common
CFLAGS    += -Wextra

# Optimization flags and other flags
# -O3 will enable further warnings like warnings w.r.t. use of
# uninitialized variables
CFLAGS    += -O3 -g -pedantic -std=gnu11
LDLIBS    +=

CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

##
## --------------------------------------------------------------- targets --
##

all: myfind

myfind: myfind.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *~

distclean: clean
	$(RM) -rf myfind doc

doc: html pdf

html:
	$(DOXYGEN) doxygen.dcf

pdf: html
	$(CD) doc/pdf && \
		$(MV) refman.tex refman_save.tex && \
		$(GREP) -v $(EXCLUDE_PATTERN) refman_save.tex > refman.tex && \
		$(RM) refman_save.tex && \
		make && \
		$(MV) refman.pdf refman.save && \
		$(RM) *.pdf *.html *.tex *.aux *.sty *.log *.eps *.out *.ind *.idx \
		      *.ilg *.toc *.tps Makefile && \
		$(MV) refman.save refman.pdf

##
## ---------------------------------------------------------- dependencies --
##

myfind.o: myfind.c

##
## =================================================================== eof ==
##
