#!/usr/bin/perl
use common::sense;


##C 18-cell 40514-generation methuselah
##C based on Richard Wobus' 15-cell marvel
#x = 78, y = 54, rule = B3/S23
#77bo$77bo$77bo21$3o20$3bo$3bo$3bo5$20b3o$9b3o10bo$22bo$21bo!


my $format = 1;

my $header = <>;
while ($header =~ m/^#/) { $header = <>; }
chomp $header;
$header =~ m/x\s*=\s([0-9]*)\s*,\s*y\s*=\s*([0-9]*)\s*,\s*rule\s*=\s*([^ ]*)/;
my ($x, $y, $rule) = ($1, $2, $3);
print STDERR "Have header: $x, $y, $rule\n";
print STDERR "Warning: rule '$rule' is lost in this conversion.\n" unless $rule eq "B3/S23";
my $rows_printed = 0;
my $image;
while (<>) {
	next if (m/^#/);
	chomp;
	my $source = $_;
    $source =~ s {(\d+)(.)} {$2 x $1}gse;
	my @lines = split /\$/,$source;
	for my $line (@lines) {
		$line =~ s/!//;
		$line =~ y/bo/01/;
		$image .= $line."0" x ($x-length $line)."\n";
		$rows_printed++;
	}
}
for (;$rows_printed < $y;$rows_printed) {
	$\ = "\n";
	$image .= "b" x ($x);
}

if ($format == 1) {
	$image =~ s/(.)/\1 /g;
	$image =~ s/ $//g;
	print "P1\n$x $y\n$image";
} else {
	print "Format $format not supported.";
}
