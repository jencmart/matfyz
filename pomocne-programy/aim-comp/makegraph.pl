#!/usr/bin/perl

use common::sense;
use Data::Dumper;
use List::Util qw(reduce);

my ($nodes, $adeg) = @ARGV;

die "Usage: ./makegraph.pl num_nodes avg_deg." unless defined $nodes && defined $adeg;

srand(42);

my @graph;

my $th = $nodes/$adeg;

for (my $i = 0; $i < $nodes; $i++) {
	for (my $j = int(rand($th)); $j < $nodes; $j += 1+int(rand($th))) {
		$graph[$i]->{$j} = 1;
		$graph[$j]->{$i} = 1;
	}
}

print "$nodes\n";
for (my $i = 0; $i < $nodes; $i++) {
	my $sum = scalar keys %{$graph[$i]};
	print "$i $sum ".join(" ", keys %{$graph[$i]})."\n";
}
