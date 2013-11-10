#!/usr/bin/perl -w

my $len = -s STDIN;
print pack('l', $len);
