UNAME 				:= 	$(shell uname)

#
#   Installation directory
#

# Since OSX 10.10 Yosemite, /usr/include gives problem
# So, let's switch to /usr/local as default instead.
ifeq ($(UNAME), Darwin)
  INSTALL_PREFIX		=	/usr/local
else
  INSTALL_PREFIX		=	/usr
endif

INSTALL_HEADERS			=	${INSTALL_PREFIX}/include
INSTALL_LIB			=	${INSTALL_PREFIX}/lib

#
#   Command to remove files, copy files, link files and create directories.
#
#   I've never encountered a *nix environment in which these commands do not work.
#   So you can probably leave this as it is
#

RM						=	rm -fr
CP						=	cp -fr
LN						=	ln -f -s
MKDIR					=	mkdir -p

#
#   End of the variables section. Here starts the list of instructions and
#   dependencies that are used by the compiler.
#

all: install

# The if statements below must be seen as single line by make

install:
	${MKDIR} ${INSTALL_HEADERS}/hyppox
	${CP} hyppox.h ${INSTALL_HEADERS}
	${CP} hyppox/* ${INSTALL_HEADERS}/hyppox

uninstall:
	${RM} ${INSTALL_HEADERS}/hyppox*

