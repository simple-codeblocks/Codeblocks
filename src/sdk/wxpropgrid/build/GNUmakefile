# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.6 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# 'install' program location 
INSTALL ?= install

# Location where the package is installed by 'make install' 
prefix ?= /usr/local

# Destination root (/ is used if empty) 
DESTDIR ?= 

#  
AR ?= ar

#  
RANLIB ?= ranlib

# Compiler flags to link shared library 
LINK_DLL_FLAGS ?= -shared

# C++ compiler 
CXX = g++

# Standard flags for C++ 
CXXFLAGS ?= 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS ?= 

# Standard linker flags 
LDFLAGS ?= 

# Location and arguments of wx-config script 
WX_CONFIG ?= wx-config

# Port of the wx library to build against [gtk1,gtk2,msw,x11,motif,mgl,mac,dfb]
WX_PORT ?= $(shell $(WX_CONFIG) --selected-config | cut -d '-' -f 1)

# Use DLL build of wx library to use? [0,1]
WX_SHARED ?= $(shell if test -z `$(WX_CONFIG) --selected-config | cut -d '-' -f 5`; then echo 1; else echo 0; fi)

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE ?= $(shell $(WX_CONFIG) --selected-config | cut -d '-' -f 2 | sed 's/unicode/1/;s/ansi/0/')

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
WX_DEBUG ?= $(shell $(WX_CONFIG) --selected-config | cut -d '-' -f 3 | sed 's/debug/1/;s/release/0/')



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###

CPPDEPS = -MT$@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD -MP
WX_VERSION_MAJOR = $(shell echo $(WX_VERSION) | cut -c1,1)
WX_VERSION_MINOR = $(shell echo $(WX_VERSION) | cut -c2,2)
WX_CONFIG_FLAGS = $(WX_CONFIG_DEBUG_FLAG) $(WX_CONFIG_UNICODE_FLAG) \
	$(WX_CONFIG_SHARED_FLAG) --toolkit=$(WX_PORT) \
	--version=$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR)
PROPGRID_LIB_CXXFLAGS = $(____propgrid) $(____propgrid_3) -W -Wall -I../include \
	`$(WX_CONFIG) --cxxflags $(WX_CONFIG_FLAGS)` $(CPPFLAGS) $(CXXFLAGS)
PROPGRID_LIB_OBJECTS =  \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_advprops.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_editors.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_extras.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_manager.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_odcombo.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_propgrid.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_props.o
PROPGRID_LIB_HEADERS =  \
	include/wx/propgrid/advprops.h \
	include/wx/propgrid/editors.h \
	include/wx/propgrid/extras.h \
	include/wx/propgrid/manager.h \
	include/wx/propgrid/odcombo.h \
	include/wx/propgrid/pg_dox_mainpage.h \
	include/wx/propgrid/propdev.h \
	include/wx/propgrid/propgrid.h \
	include/wx/propgrid/props.h \
	include/wx/propgrid/xh_propgrid.h
PROPGRID_DLL_CXXFLAGS = $(____propgrid) $(____propgrid_3) -W -Wall -I../include \
	-DWXMAKINGDLL_PROPGRID -fPIC -DPIC `$(WX_CONFIG) --cxxflags \
	$(WX_CONFIG_FLAGS)` $(CPPFLAGS) $(CXXFLAGS)
PROPGRID_DLL_OBJECTS =  \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_advprops.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_editors.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_extras.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_manager.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_odcombo.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_propgrid.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_props.o
PROPGRID_DLL_HEADERS =  \
	include/wx/propgrid/advprops.h \
	include/wx/propgrid/editors.h \
	include/wx/propgrid/extras.h \
	include/wx/propgrid/manager.h \
	include/wx/propgrid/odcombo.h \
	include/wx/propgrid/pg_dox_mainpage.h \
	include/wx/propgrid/propdev.h \
	include/wx/propgrid/propgrid.h \
	include/wx/propgrid/props.h \
	include/wx/propgrid/xh_propgrid.h
PROPGRIDSAMPLE_CXXFLAGS = $(____propgrid) $(____propgrid_3) -W -Wall \
	-I../include `$(WX_CONFIG) --cxxflags $(WX_CONFIG_FLAGS)` $(CPPFLAGS) \
	$(CXXFLAGS)
PROPGRIDSAMPLE_OBJECTS =  \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_minimal.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_propgridsample.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_sampleprops.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_tests.o \
	gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_xh_propgrid.o

### Conditionally set variables: ###

ifeq ($(WX_DEBUG),0)
WX_CONFIG_DEBUG_FLAG = --debug=no
endif
ifeq ($(WX_DEBUG),1)
WX_CONFIG_DEBUG_FLAG = --debug=yes
endif
ifeq ($(WX_UNICODE),0)
WX_CONFIG_UNICODE_FLAG = --unicode=no
endif
ifeq ($(WX_UNICODE),1)
WX_CONFIG_UNICODE_FLAG = --unicode=yes
endif
ifeq ($(WX_SHARED),0)
WX_CONFIG_SHARED_FLAG = --static=yes
endif
ifeq ($(WX_SHARED),1)
WX_CONFIG_SHARED_FLAG = --static=no
endif
ifeq ($(WX_SHARED),0)
_BUILDDIR_SHARED_SUFFIX = 
endif
ifeq ($(WX_SHARED),1)
_BUILDDIR_SHARED_SUFFIX = _dll
endif
ifeq ($(WX_SHARED),0)
__propgrid_lib___depname = \
	../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a
endif
ifeq ($(WX_SHARED),0)
__install_propgrid_lib___depname = install_propgrid_lib
endif
ifeq ($(WX_SHARED),0)
__uninstall_propgrid_lib___depname = uninstall_propgrid_lib
endif
ifeq ($(WX_SHARED),0)
__install_propgrid_lib_headers___depname = install_propgrid_lib_headers
endif
ifeq ($(WX_SHARED),0)
__uninstall_propgrid_lib_headers___depname = uninstall_propgrid_lib_headers
endif
ifeq ($(WX_SHARED),1)
__propgrid_dll___depname = \
	../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so
endif
ifeq ($(WX_SHARED),1)
__install_propgrid_dll___depname = install_propgrid_dll
endif
ifeq ($(WX_SHARED),1)
__uninstall_propgrid_dll___depname = uninstall_propgrid_dll
endif
ifeq ($(WX_SHARED),1)
__install_propgrid_dll_headers___depname = install_propgrid_dll_headers
endif
ifeq ($(WX_SHARED),1)
__uninstall_propgrid_dll_headers___depname = uninstall_propgrid_dll_headers
endif
ifeq ($(WX_SHARED),1)
__propgridsample_RPATH = -Wl,-rpath,../lib/gcc_dll
endif
ifeq ($(WX_SHARED),0)
__propgrid = lib
endif
ifeq ($(WX_SHARED),1)
__propgrid = dll
endif
ifeq ($(WX_DEBUG),0)
____propgrid = -O2
endif
ifeq ($(WX_DEBUG),1)
____propgrid = -O0
endif
ifeq ($(WX_DEBUG),0)
____propgrid_3 = 
endif
ifeq ($(WX_DEBUG),1)
____propgrid_3 = -g
endif


all: gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)
gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX):
	@mkdir -p gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all: test_for_selected_wxbuild $(__propgrid_lib___depname) $(__propgrid_dll___depname) ../samples/propgridsample

install: $(__install_propgrid_lib___depname) $(__install_propgrid_lib_headers___depname) $(__install_propgrid_dll___depname) $(__install_propgrid_dll_headers___depname)

uninstall: $(__uninstall_propgrid_lib___depname) $(__uninstall_propgrid_lib_headers___depname) $(__uninstall_propgrid_dll___depname) $(__uninstall_propgrid_dll_headers___depname)

clean: 
	rm -f gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/*.o
	rm -f gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/*.d
	rm -f ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a
	rm -f ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so
	rm -f ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so
	rm -f ../samples/propgridsample

test_for_selected_wxbuild: 
	@$(WX_CONFIG) $(WX_CONFIG_FLAGS)

ifeq ($(WX_SHARED),0)
../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a: make_dir_propgrid_lib  $(PROPGRID_LIB_OBJECTS)
	rm -f $@
	$(AR) rcu $@ $(PROPGRID_LIB_OBJECTS)
	$(RANLIB) $@
endif

ifeq ($(WX_SHARED),0)
install_propgrid_lib: $(__propgrid_lib___depname)
	$(INSTALL) -d $(DESTDIR)$(prefix)/lib
	$(INSTALL) -m 644 ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a $(DESTDIR)$(prefix)/lib
endif

ifeq ($(WX_SHARED),0)
uninstall_propgrid_lib: 
	rm -f $(DESTDIR)$(prefix)/lib/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a
endif

make_dir_propgrid_lib: 
	@mkdir -p ../lib/gcc_$(__propgrid)

ifeq ($(WX_SHARED),0)
install_propgrid_lib_headers: 
	$(INSTALL) -d $(DESTDIR)$(prefix)
	for f in $(PROPGRID_LIB_HEADERS); do \
	if test ! -d $(DESTDIR)$(prefix)/`dirname $$f` ; then \
	$(INSTALL) -d $(DESTDIR)$(prefix)/`dirname $$f`; \
	fi; \
	$(INSTALL) -m 644 ../$$f $(DESTDIR)$(prefix)/$$f; \
	done
endif

ifeq ($(WX_SHARED),0)
uninstall_propgrid_lib_headers: 
	for f in $(PROPGRID_LIB_HEADERS); do \
	rm -f $(DESTDIR)$(prefix)/$$f; \
	done
endif

ifeq ($(WX_SHARED),1)
../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so: make_dir_propgrid_dll  $(PROPGRID_DLL_OBJECTS)
	$(CXX) $(LINK_DLL_FLAGS) -fPIC -o $@ $(PROPGRID_DLL_OBJECTS)  $(____propgrid_3) -L../lib $(LDFLAGS)  `$(WX_CONFIG) $(WX_CONFIG_FLAGS) --libs adv,core,base`
endif

ifeq ($(WX_SHARED),1)
install_propgrid_dll: $(__propgrid_dll___depname)
	$(INSTALL) -d $(DESTDIR)$(prefix)/lib
	$(INSTALL) -m 644 ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so $(DESTDIR)$(prefix)/lib
	install -c ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so $(DESTDIR)$(prefix)/lib
endif

ifeq ($(WX_SHARED),1)
uninstall_propgrid_dll: 
	rm -f $(DESTDIR)$(prefix)/lib/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so
	rm -f $(DESTDIR)$(prefix)/lib/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).so
endif

make_dir_propgrid_dll: 
	@mkdir -p ../lib/gcc_$(__propgrid)

ifeq ($(WX_SHARED),1)
install_propgrid_dll_headers: 
	$(INSTALL) -d $(DESTDIR)$(prefix)
	for f in $(PROPGRID_DLL_HEADERS); do \
	if test ! -d $(DESTDIR)$(prefix)/`dirname $$f` ; then \
	$(INSTALL) -d $(DESTDIR)$(prefix)/`dirname $$f`; \
	fi; \
	$(INSTALL) -m 644 ../$$f $(DESTDIR)$(prefix)/$$f; \
	done
endif

ifeq ($(WX_SHARED),1)
uninstall_propgrid_dll_headers: 
	for f in $(PROPGRID_DLL_HEADERS); do \
	rm -f $(DESTDIR)$(prefix)/$$f; \
	done
endif

../samples/propgridsample: $(PROPGRIDSAMPLE_OBJECTS) make_sample_dir_propgridsample $(__propgrid_lib___depname)
	$(CXX) -o $@ $(PROPGRIDSAMPLE_OBJECTS)  $(____propgrid_3) -L../lib  $(__propgridsample_RPATH) $(LDFLAGS)  ../lib/gcc_$(__propgrid)/libwxcode_$(WX_PORT)_propgrid-$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR).a `$(WX_CONFIG) $(WX_CONFIG_FLAGS) --libs xrc,html,xml,adv,core,base`

make_sample_dir_propgridsample: 
	@mkdir -p ../samples

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_advprops.o: ../src/advprops.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_editors.o: ../src/editors.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_extras.o: ../src/extras.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_manager.o: ../src/manager.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_odcombo.o: ../src/odcombo.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_propgrid.o: ../src/propgrid.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_lib_props.o: ../src/props.cpp
	$(CXX) -c -o $@ $(PROPGRID_LIB_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_advprops.o: ../src/advprops.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_editors.o: ../src/editors.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_extras.o: ../src/extras.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_manager.o: ../src/manager.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_odcombo.o: ../src/odcombo.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_propgrid.o: ../src/propgrid.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgrid_dll_props.o: ../src/props.cpp
	$(CXX) -c -o $@ $(PROPGRID_DLL_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_minimal.o: ../samples/minimal.cpp
	$(CXX) -c -o $@ $(PROPGRIDSAMPLE_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_propgridsample.o: ../samples/propgridsample.cpp
	$(CXX) -c -o $@ $(PROPGRIDSAMPLE_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_sampleprops.o: ../samples/sampleprops.cpp
	$(CXX) -c -o $@ $(PROPGRIDSAMPLE_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_tests.o: ../samples/tests.cpp
	$(CXX) -c -o $@ $(PROPGRIDSAMPLE_CXXFLAGS) $(CPPDEPS) $<

gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/propgridsample_xh_propgrid.o: ../src/xh_propgrid.cpp
	$(CXX) -c -o $@ $(PROPGRIDSAMPLE_CXXFLAGS) $(CPPDEPS) $<

.PHONY: all install uninstall clean install_propgrid_lib uninstall_propgrid_lib \
	install_propgrid_lib_headers uninstall_propgrid_lib_headers \
	install_propgrid_dll uninstall_propgrid_dll install_propgrid_dll_headers \
	uninstall_propgrid_dll_headers


# Dependencies tracking:
-include gcc$(WX_PORT)$(_BUILDDIR_SHARED_SUFFIX)/*.d
