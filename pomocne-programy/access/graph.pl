#!/usr/bin/perl

my $min_mem = 8;	# Minimum memory block size (KB)
my $max_mem = 262144;	# Maximum memory block size (KB), must fit in physical memory
my @item_sizes = (16,64,128,1024,4096);	# Sizes of items
my @randomized = (0,1);	# Try randomized accesses?
my @modify = (0,1);	# Try read-write accesses?
my $measure_ms = 1000;	# Duration of measurement in ms
my $array = 1;		# Items are accessed as an array instead of a list
my $huge = 0;		# Use huge pages (hugetlbfs required)

# If you want to include profiling information (cache misses etc.) in detailed
# graphs, ask for specific events here. You must have the "perf" utility installed,
# with a recent enough kernel. The set of available events depends on the exact
# cpu type, see "man perf-list" for further details. Also, it might be necessary
# to increase $measure_ms in order to gather enough samples.

# Events and their names
my %perf_events = (
	# 'cpu-cycles' => 'CPU cycles',
	# 'cache-misses' => 'Cache misses',
	'L1-dcache-load-misses' => 'L1D load misses',
	'LLC-loads' => 'LLC loads',
	# 'dTLB-load-misses' => 'DTLB load misses',
	'mem-loads' => 'Memory loads',
	'r412E' => 'LLC misses',
);
my @perf_events = sort keys %perf_events;

# How to call "perf"
my $perf_tool = "perf_3.16";

# Use --graph to disable all calculations and just re-use the log files
my $graph_only = 0;
if (@ARGV && $ARGV[0] eq "--graph") {
	$graph_only = 1;
	shift @ARGV;
}

# Use ./graph.pl <directory> to store results in a given directory.
# Otherwise, "out" is used.
my $dir = $ARGV[0] // 'out';
-d $dir or mkdir $dir or die "Cannot create $dir";
chdir $dir or die;
for my $f ("access.c", "Makefile") {
	-f $f or symlink "../$f", $f or die;
}

### Get machine description, including layout of caches ###

my $hostname = `hostname`;
chomp $hostname;

my $cpu = `grep -m1 '^model name[[:space:]]\\+:' /proc/cpuinfo`;
chomp $cpu;
$cpu =~ s{^model name\s+:\s*}{};
$cpu = " $cpu ";
$cpu =~ s{\(tm\)|\(r\)}{}gi;
$cpu =~ s{ (Intel|AMD|CPU|Processor) }{ }g;
$cpu =~ s{ @.*}{};
$cpu =~ s{\s+}{ }g;
$cpu =~ s{^\s+|\s+$}{}g;

my @options = ();
push @options, ($array ? "Array" : "List");
push @options, "HugePages" if $huge;
$machine = '(' . join(" ", @options) . ')';

our $c;
my @caches = ();
my $prev_L = 0;
for $c (</sys/devices/system/cpu/cpu0/cache/index*>) {
	sub rd($) {
		my ($f) = @_;
		open F, "$c/$f" or return "";
		my $x = <F>;
		close F;
		chomp $x;
		return $x;
	}

	my $l = rd("level");
	my $w = rd("ways_of_associativity");
	my $s = rd("size");
	my $t = rd("type");
	$t =~ s/(.).*/$1/;
	$t =~ s/U//;
	$t eq "I" and next;
	push @caches, "L$l$t:$s/$w-way";
	$prev_L = $L;
}
$machine .= " $hostname [" . join(" ", $cpu, @caches) . "]";

### Profiling ###

my @profile = ();

sub run_profiled(@) {
	if (!@perf_events) {
		my $cmd = join(" ", @_);
		my $out = `$cmd`;
		die if $?;
		return $out;
	}

	my @cmd = ($perf_tool, qw(stat -o perf.out -x :), (map +( '-e', $_ ), @perf_events), @_);
	my $cmd = join(" ", @cmd);
	my $out = `$cmd`;
	die if $?;

	@profile = ();
	open my $perf, '<', 'perf.out' or die;
	while (<$perf>) {
		chomp;
		next if /^$/ || /^#/;
		my @fields = split /:/;
		push @profile, $fields[0];
	}
	close $perf;
	@profile == @perf_events or die "Perf returned wrong number of values";

	return $out;
}

### Measure and create logs ###

if (!$graph_only) {
	for my $r (@randomized) {
		for my $m (@modify) {
			for my $s (@item_sizes) {
				my $f = "a-$r-$m-$s.log";
				next if -f $f;
				my @o = ( "-DSIZE=$s", "-DMS=$measure_ms" );
				push @o, "-DRANDOMIZE" if $r;
				push @o, "-DMODIFY" if $m;
				push @o, "-DARRAY" if $array;
				push @o, "-DHUGE" if $huge;
				my $o = join(" ", @o);
				`rm -f access`; die if $?;
				`make access F="$o"`; die if $?;
				open D, ">$f" or die;
				for (my $mem=$min_mem; $mem<=$max_mem; $mem*=2) {
					print "$mem ($o): ";
					my $out = run_profiled('./access', $mem);
					my ($t, $attempts) = split /\s+/, $out;
					my @prof = map { $_ / $attempts } @profile;
					chomp $t;
					print join(" ", $t,
						map { $perf_events[$_] . ":" . sprintf("%.3f", $prof[$_]) } 0..$#perf_events),
						"\n";
					print D join("\t", $mem, $t, @prof), "\n";
				}
				close D;
			}
		}
	}
}

### The plots ###

# Generic header
open GP, ">plot.gp" or die;
print GP <<EOF ;
set terminal png large size 1024,768
set logscale x 2
set key left
set xlabel "data set size [KiB]"
set ylabel "time per access [ns]"
set grid
set yrange [0:15]	# Change if necessary

EOF

# For each access type, show different sizes
for my $r (@randomized) {
	for my $m (@modify) {
		my @plots = ();
		for my $s (@item_sizes) {
			push @plots, "'a-$r-$m-$s.log' title '${s}B items' with linespoints";
		}
		print GP "set output 'a-$r-$m.png'\n";
		my $title = ($r ? "Random" : "Sequential") . " " . ($m ? "Read-Write" : "Read-Only") . " Accesses $machine";
		print GP "set title '$title'\n";
		print GP "plot ", join(", ", @plots), "\n\n";
	}
}

# For each item size, show all access types
for my $s (@item_sizes) {
	print GP "set output 'a-size-$s.png'\n";
	print GP "set title 'Item size $s $machine'\n";
	my @a = ();
	for my $r (@randomized) {
		for my $m (@modify) {
			my $t = ($r ? "Random" : "Sequential") . " " . ($m ? "R+W" : "R");
			push @a, "'a-$r-$m-$s.log' title '$t' with linespoints";
		}
	}
	print GP "plot ", join(", ", @a), "\n\n";
}

# For each item size and access type, show full profiling information
if (@perf_events) {
	for my $s (@item_sizes) {
		for my $r (@randomized) {
			for my $m (@modify) {
				print GP "set output 'a-prof-$s-$r-$m.png'\n";
				my $title = "Profile for " . ($r ? "Random" : "Sequential") . " " . ($m ? "Read-Write" : "Read-Only") . " $s bytes $machine";
				print GP "set title '$title'\n";
				print GP "set y2label 'number of events'\n";
				print GP "set y2tics\n";
				print GP "set y2range [0:3]\n";
				my @a = ();
				push @a, "'a-$r-$m-$s.log' title 'Time' with linespoints";
				for my $i (0..$#perf_events) {
					push @a, "'' using 1:" . ($i+3) . " axes x1y2 title '" . $perf_events{$perf_events[$i]} . "' with linespoints";
				}
				print GP "plot ", join(", ", @a), "\n\n";
			}
		}
	}
}

close GP;
