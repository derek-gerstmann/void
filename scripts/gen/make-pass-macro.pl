#!/usr/bin/perl
$max = int($ARGV[$1]);
$max = $max < 1 ? 128 : $max;
$i = $max;

# -- Generates: #define VD_PP_PASS_ARGS_2(op,a1,a2) op(a1,a2)

while($i >= 0) {
	$j = 1;
	print "#define VD_PP_PASS_ARGS_$i";
	print "(op,";
	if($i == 0) {
		print "a0)\n";
		$i--;
		next;
	}
	while($j <= $i) {
		print "a$j";
		if($j <= $i - 1) {
			printf ",";
		}
		$j++;
	}
	print ") op(";

	$j = 1;
	while($j <= $i) {
		print "a$j";
		if($j <= $i - 1) {
			printf ",";
		}
		$j++;
	}
	$i--;
	print ")\n";
}

