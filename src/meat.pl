#!/usr/bin/env perl

use strict;
use warnings;

print "I shan't be printed.";

sub expo {
    my ($a, $b) = @_;
    return $a ** $b;
}

sub debug {
    my $name = "Larry Wall";

    printf("%s says: Hello from meat.pl!\n", $name);

    print "Calling ls:\n";
    my $dir = `ls -a`;
    
    # replace all occurences of '\n', except the last, with ", "
    $dir =~ (s/\n(?=.*+\n)/, /g);

    printf("%s", $dir);

    # print time
    printf("%d\n", time);

    return 666;
}
