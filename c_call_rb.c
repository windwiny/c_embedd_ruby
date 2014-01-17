#include <stdio.h>
#include "ruby.h"

void crb_init() {
	int argc2 = 0;
	char **argv2 = NULL;

	ruby_sysinit(&argc2, &argv2);
	RUBY_INIT_STACK;
	ruby_init();
	ruby_init_loadpath();
}

void crb_cleanup() {
	ruby_cleanup(0);
}

int crb_eval_string(const char *rbstr) {
	int error;
	rb_eval_string_protect(rbstr, &error);
	return error;
}

#define MAX 4096
char evalStr[MAX];
char *debug_flag = "false";

int main(int argc, char **argv) {
	int res;

	for (res=1; res<argc; res++) {
		if ((strcmp("--debug", argv[res])==0) || (strcmp("-d", argv[res])==0)) {
			debug_flag = "true";
			break;
		}
	}

	snprintf(evalStr, MAX-400,
		"require 'rbconfig'\n"
		"$DEBUG=%s\n"
		"\n"
		"$:.unshift '.'\n"
		"puts %%Q{LOAD_PATH: #{$:}} if $DEBUG\n"
		"argv0 = %%q*%s*\n"
		"puts %%Q{ARGV0: #{argv0}} if $DEBUG\n"
		"rbfn = File.basename(argv0).chomp(RbConfig::CONFIG['EXEEXT'])\n"
		"puts %%Q{RBFN: #{rbfn}} if $DEBUG\n"
		"\n"
		"require rbfn\n"
		,debug_flag, argv[0]);

	for (res=1; res<argc; res++) {
		if ((strcmp("--help", argv[res])==0) || (strcmp("-h", argv[res])==0)) {
			printf(
			"%s syntax:\n"
			"\n"
			"  -h  --help     show this message\n"
			"  -d  --debug    set $DEBUG to true\n"
			"\n"
			"---- this program will run ----\n"
			"%s\n"
			"--------\n"
			, argv[0], evalStr);
			exit(0);
		}
	}

	crb_init();
	res = crb_eval_string(evalStr);
	crb_cleanup();

	if (res != 0) {
		fprintf(stderr, "\nerror %d\n", res);
	}

	return res;
}
