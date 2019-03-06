#! /usr/bin/env perl

use strict;
use warnings;

use File::Basename;
use Test::More tests => 6;

my $dirname = dirname(__FILE__);
chdir $dirname or die 'cannot chdir';

# test clang with all warnings enabled except for a specific whitelist
# global-constructors is disabled. The only global constructor in the C++ path is
# the std::atomic which is unavoidable

ok(0 == system('clang++ -o selftest_1.cpp.exe selftest_1.cpp -Weverything -Wno-global-constructors -fsanitize=address'));
ok(0 == system('./selftest_1.cpp.exe 1>/dev/null 2>/dev/null'));
ok(0 == system('prove ./selftest_1.cpp.exe'));

ok(0 == system('clang -o selftest_1.c.exe selftest_1.c -Weverything -fsanitize=address'));
ok(0 == system('./selftest_1.c.exe 1>/dev/null 2>/dev/null'));
ok(0 == system('prove ./selftest_1.c.exe'));
