dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])

# ====================
# Version informations
# ====================
m4_define([admesh_version_major],[0])
m4_define([admesh_version_minor],[97])
m4_define([admesh_version_micro],[3])
m4_define([admesh_version],[admesh_version_major.admesh_version_minor.admesh_version_micro])

# =============
# Automake init
# =============
AC_INIT([admesh],[admesh_version])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])
LT_INIT([disable-static pic-only])
AC_LANG([C])

# ===========================
# Find required base packages
# ===========================
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_SED
AC_PROG_MKDIR_P

# ================
# Check for cflags
# ================
AC_ARG_ENABLE([werror],
	[AS_HELP_STRING([--enable-werror], [Treat all warnings as errors, useful for development @<:@default=disabled@:>@])],
	[enable_werror="$enableval"],
	[enable_werror=no]
)
AS_IF([test x"$enable_werror" != "xno"], [
	CFLAGS="$CFLAGS -Werror"
	CXXFLAGS="$CXXFLAGS -Werror"
])
AS_IF([test x"$GCC" = xyes], [
	# Be tough with warnings and produce less careless code
	CFLAGS="$CFLAGS -Wall -Wextra -pedantic -Werror=format-security  -Wp,-D_FORTIFY_SOURCE=2"
	CXXFLAGS="$CXXFLAGS -Wall -Wextra -Wshadow -pedantic -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2"
])

# =========
# Find libs
# =========
AC_CHECK_LIB(m, main)

# =====================
# Prepare all .in files
# =====================
AC_CONFIG_FILES([
Makefile
libadmesh.pc
])

AC_OUTPUT

# ==============================================
# Display final informations about configuration
# ==============================================
AC_MSG_NOTICE([
==============================================================================
Build configuration:
	werror:	  ${enable_werror}
==============================================================================
])
