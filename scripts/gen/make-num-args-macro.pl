#!/usr/bin/perl
$max = int($ARGV[$1]);
$max = $max < 1 ? 128 : $max;

$i = 0;
print "\n";
while($i < $max) {
	print "_$i,";
	$i++;
}

print "\n";
$i = $max;
while($i >= 0) {
	print "$i,";
	$i--;
}
print "\n";

