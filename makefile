S := ${.CURDIR}

D =
PREFIX = /usr/local
EXEC_PREFIX = ${PREFIX}
BINDIR = ${EXEC_PREFIX:/=}/bin

PN = suduo

${PN}: ${S}/${PN}.c
	gcc -static -fPIC -std=c99 -O2 -Wall -Wextra ${CPPOPTS} ${CFLAGS} -o ${@}.make_tmp ${S}/${PN}.c
	strip -s ${@}.make_tmp
	mv -f -- ${@}.make_tmp ${@}

clean:
	rm -f -- ./${PN} ./${PN}.make_tmp

install:
	install -D -m 4711 -- ./${PN} ${D:/=}${BINDIR}/${PN}

.PHONY: clean install
