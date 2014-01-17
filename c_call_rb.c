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
int help_flag = 0;

int main(int argc, char **argv) {
	int i,j;
	int res;
	crb_init();


	for (i=1; i < argc; i++) {
		if (strcmp("--", argv[i])==0) {
			VALUE vargv;
			vargv = rb_gv_get("$*");
			for (j = i+1; j < argc; j++)
			{
				rb_ary_push(vargv, rb_str_new2(argv[j]));
			}
			break;
		} else if ((strcmp("--debug", argv[i])==0) || (strcmp("-d", argv[i])==0)) {
			debug_flag = "true";
		} else if ((strcmp("--help", argv[i])==0) || (strcmp("-h", argv[i])==0)) {
			help_flag = 1;
		}
	}

	snprintf(evalStr, MAX-400,
		"require 'rbconfig'\n"
		"$DEBUG=%s\n"
		"\n"
		"puts %%Q{ARGV: #{ARGV}} if $DEBUG\n"
		"$:.unshift '.'\n"
		"puts %%Q{LOAD_PATH: #{$:}} if $DEBUG\n"
		"argv0 = %%q*%s*\n"
		"puts %%Q{ARGV0: #{argv0}} if $DEBUG\n"
		"rbfn = File.basename(argv0).chomp(RbConfig::CONFIG['EXEEXT'])\n"
		"puts %%Q{RBFN: #{rbfn}} if $DEBUG\n"
		"\n"
		"require rbfn\n"
		,debug_flag, argv[0]);

	if (help_flag) {
		printf(
		"%s syntax:\n"
		"\n"
		"  -h  --help                  show this message\n"
		"  -d  --debug                 set $DEBUG to true\n"
		"  --  [arg1 [arg2 [...]]]     all arguments pass to ruby program after '--'\n"
		"\n"
		"---- this program will run ----\n"
		"%s\n"
		"--------\n"
		, argv[0], evalStr);
		exit(0);
	}

	res = crb_eval_string(evalStr);
	crb_cleanup();

	if (res != 0) {
		fprintf(stderr, "\nerror %d\n", res);
	}

	return res;
}
