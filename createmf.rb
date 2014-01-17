#!/usr/bin/env ruby -w


#===============

INFILE="c_call_rb.c"

#===============
require "rbconfig"

OUTFILE="#{File.basename(INFILE).chomp(File.extname(INFILE))}#{RbConfig::CONFIG['EXEEXT']}"
CC="gcc"

INCFLAG="-I#{RbConfig::CONFIG['rubyhdrdir']} -I#{RbConfig::CONFIG['rubyarchhdrdir']}"
LDFLAG="-L#{RbConfig::CONFIG['libdir']}  -l#{RbConfig::CONFIG['RUBY_SO_NAME']}"




require 'rbconfig'

makefile=<<-EOS
# generate by createmf.rb
# #{Time.now}   #{RUBY_VERSION}-#{RUBY_PATCHLEVEL}

INFILE=#{INFILE}

OUTFILE=#{OUTFILE}
CC=#{CC}

INCFLAG=#{INCFLAG}
LDFLAG=#{LDFLAG}

#{OUTFILE}: #{INFILE}
\t$(CC) -Wall -o ${OUTFILE}  $(INFILE)  $(INCFLAG)  $(LDFLAG)

EOS

File.write('Makefile', makefile)

