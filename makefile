S := ${.CURDIR}

D =
PREFIX = /usr/local
EXEC_PREFIX = ${PREFIX}
BINDIR = ${EXEC_PREFIX:/=}/bin

CC_OPTS =
CC_OPTS += -std=c99
CC_OPTS += -D_POSIX_C_SOURCE=200112L
CC_OPTS +=  -O2
CC_OPTS += -static
CC_OPTS += -fPIC
CC_OPTS += -fstack-protector-all
CC_OPTS += -Wall -Wextra

.if exists(${S}/localconfig.h)
CC_OPTS += -DLOCALCONFIG="${S}/localconfig.h"
.endif

PN = suduo

${PN}: ${S}/${PN}.c ${S}/config.h
	gcc ${CC_OPTS} ${CPPOPTS} ${CFLAGS} -o ${@}.make_tmp ${S}/${PN}.c
	strip -s ${@}.make_tmp
	mv -f -- ${@}.make_tmp ${@}

clean:
	rm -f -- ./${PN} ./${PN}.make_tmp

install:
	install -D -m 4711 -- ./${PN} ${D:/=}${BINDIR}/${PN}

.PHONY: clean install
