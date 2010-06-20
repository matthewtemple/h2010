.PHONY: force
compiler = gcc
machine = $(shell uname -m)
uname = $(shell uname)
os = $(strip $(uname))
ifeq ($(os),Linux)
	os = linux
endif
repository = $(shell h-git-name)

library = lib$(repository).a
demo_archive_dir = $(repository)_demo_$(os)_$(machine)
demo_archive = $(demo_archive_dir).tgz
release_archive_dir = $(repository)_$(os)_$(machine)
release_archive = $(release_archive_dir).tgz

ifeq ($(MAKECMDGOALS),release)
	build = -DH_BUILD_RELEASE
	debugging = -DNDEBUG
	optimization = -O3
	mode_identifier = \#
	warnings = -Wall -Werror
	archive = $(release_archive)
	archive_dir = $(release_archive_dir)
else ifeq ($(MAKECMDGOALS),demo)
	build = -DH_BUILD_DEMO
	debugging = -DNDEBUG
	optimization = -O3
	mode_identifier = ~
	warnings = -Wall -Werror
	archive = $(demo_archive)
	archive_dir = $(demo_archive_dir)
else
	build = -DH_BUILD_DEVELOPMENT
	debugging = -g
	optimization = -O0
	mode_identifier = +
	warnings = -Wall -Werror
endif

library_paths = -L. $(H_LIBRARY_PATHS)
ifeq ($(os),linux)
  library_paths += -L/usr/local/lib -L/usr/lib
endif
include_paths = -I$(H)/.. -I$(H_C_STANDARD_INCLUDE_DIR) -I$(H_POSTGRESQL_INCLUDE_DIR)
libraries = $(H_LIBRARIES) -lcrypto -ljpeg -lm -lncurses -lpq -lpthread -luuid
link_options = -ansi $(build) $(debugging) $(optimization) $(warnings) $(include_paths) -D_GNU_SOURCE $(library_paths) $(libraries)
compile_options = -ansi $(build) $(debugging) $(optimization) $(warnings) $(include_paths) -D_GNU_SOURCE

object_bases = $(shell find . -name "*.c" | grep -v "\.test.c" | grep -v "\.impl.c" | grep -v "\.bin.c" | sed 's/\(.*\)\..*/\1/')
object_sources := $(foreach object_base, $(object_bases), $(object_base).c)
#object_headers := $(foreach object_base, $(object_bases), $(object_base).h)
objects := $(foreach object_base, $(object_bases), $(object_base).o)
object_tests := $(foreach object_base, $(object_bases), $(object_base).test)
#object_test_sources := $(foreach object_base, $(object_bases), $(object_base).test.c)
programs = $(shell find . -name "*.bin.c" | sed 's/\(.*\)\..*/\1/')
#program_sources := $(foreach program, $(programs), $(program).c)
clean = $(shell cat .clean)

development: _all _system_headers

demo: clean _all _system_headers _archive

release: clean _all _system_headers _archive

docs: _clean_docs
	@doxygen $(H)/bin/h_doxygen.conf

stats:
	@h-stats

tags:
	@h-make-etags

todo:
	@h-show-todos

clean: _clean_docs
	@rm -f $(library)
	@rm -rf $(release_archive_dir)
	@rm -rf $(demo_archive_dir)
	@rm -f $(release_archive)
	@rm -f $(demo_archive)
	@find . -name "*~" -exec rm {} \;
	@find . -name ".*~" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*.test" -exec rm {} \;
	@find . -name "*.bin" -exec rm {} \;
	@rm -f .etags
	@echo "" > .makedepend
	@rm -f .makedepend.bak
	@rm -rf $(clean)

help:
	@echo "make         : make objects, tests; run tests; make programs, library"
	@echo "make demo    : make demo archive with docs"
	@echo "make release : make release archive with docs"
	@echo "make docs    : make docs (in doxygen/)"
	@echo "make tags    : make emacs tags (in .etags)"
	@echo "make clean   : clean"
	@echo "make stats   : show code statistics"
	@echo "make todo    : show todo items from code"

_all: _init _objects _test _programs _library

_archive:
	@mkdir $(archive_dir)
	@rsync -avz --exclude ".*" --exclude "*~" --exclude "$(archive_dir)/" --include "*/" --include "*.h" --include "*.a" --include-from=.distribute --exclude "*" . $(archive_dir)
	@cd $(archive_dir) && doxygen $(H)/bin/h_doxygen.conf
	@tar -czf $(archive) $(archive_dir)

_clean_docs:
	@rm -rf doxygen

_debug:
	@echo $(objects)

_init:
	@makedepend -f.makedepend $(include_paths) $(object_sources)

_objects: $(objects)

_test: _tests
	@h-run-tests $(object_tests)

_tests: $(object_tests)

_programs: $(programs)

_system_headers:
	@h-create-system-headers

_library:
	@ar rcs $(library) $(objects)

%.o: %.c
	$(compiler) $< -o $@ -c $(compile_options)

%.test: %.test.c force
	$(compiler) $< -o $@ $(objects) $(compile_options) $(link_options)

%.bin: %.bin.c force
	$(compiler) $< -o $@ $(objects) $(compile_options) $(link_options)

include .makedepend
