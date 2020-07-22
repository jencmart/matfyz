#!/usr/bin/perl

use common::sense;
use Data::Dumper;

############################################
## This script eats memory for breakfast! ##
############################################

#Image size is X = width, Y = height. That is $img->[$y]->[$x];

sub image_size {
	my ($img) = @_;
	return (scalar @{$img->[0]}, scalar @$img);
}

sub image_print {
	my ($img, $fh) = @_;
	$fh ||= \*STDOUT;
	my ($x, $y) = image_size($img);
	print $fh "P1\n$x $y\n";
	for my $line (@$img) {
		print $fh join(" ", @$line)."\n";
	}
}

sub image_init {
	my ($x, $y, $prefill) = @_;
	$prefill ||= 0;
	my @img;
	for (my $j = 0; $j < $y; $j++) {
		my @line = ($prefill) x $x;
		push @img, \@line;
	}
	return \@img;
}

sub image_copy {
	my ($dst, $src, $offx, $offy) = @_;
	my ($sizex, $sizey) = image_size($src);
	for (my $x = 0; $x < $sizex; $x++) {
		for (my $y = 0; $y < $sizey; $y++) {
			$dst->[$offy+$y]->[$offx+$x] = $src->[$y]->[$x];
		}
	}
}

sub image_border {
	my ($img, $sizex, $sizey, $fill) = @_;
	$sizey ||= $sizex;
	$fill ||= 0;
	for my $line (@$img) {
		for (my $i = 0; $i < $sizex; $i++) {
			push @$line, $fill;
			unshift @$line, $fill;
		}
	}
	my $lw = scalar @{$img->[0]};
	for (my $i = 0; $i < $sizey; $i++) {
			my @l1 = ($fill) x $lw;
			my @l2 = ($fill) x $lw;
			push @$img, \@l1;
			unshift @$img, \@l2;
	}
}

sub image_load {
	my ($file) = @_;
	open( my $fh, "<", $file) or die "Could not open file '$file'";
	my $format = <$fh>;
	chomp $format;
	die "Unsupported format $format." unless $format eq "P1";
	my ($x,$y) = split /\s+/, scalar <$fh>;

	my @img = ();

	while (<$fh>) {
		my @line = split /\s+/;
		die "Image line is not wide enough!" unless ($x == scalar @line);
		push @img, \@line;
	}

	die "Image not high enough!" unless ($y == scalar @img);

	printf STDERR "Found image $x,$y\n";
	return \@img;
}

sub image_store {
	my ($image, $file) = @_;
	open( my $fh, ">", $file) or die "Could not open file '$file' for writing";
	image_print($image, $fh);
	close($fh);
	
}

sub image_tile {
	my ($img, $nx, $ny) = @_;
	my ($x, $y) = image_size($img);
	my $out = image_init($nx*$x, $ny*$y);
	for (my $xp = 0; $xp < $nx; $xp++) {
		for (my $yp = 0; $yp < $ny; $yp++) {
			image_copy($out, $img, $xp*$x, $yp*$y);
		}
	}
	return $out;
}

my $image = image_load("meth.pbm");
image_border($image, 10);
my $out = image_tile($image, 10, 10);
image_border($image, 10);
image_store($out, "meth-big.pbm");



# Chessboard
#my $image = image_init(5,5,1);
#my $big = image_init(10, 10, 0);
#image_copy($big, $image, 0, 0);
#image_copy($big, $image, 5, 5);
#my $extra = image_tile($big, 4, 4);
#image_border($extra, 1, 1, 1);
#image_border($extra, 10);
#image_print($extra);
