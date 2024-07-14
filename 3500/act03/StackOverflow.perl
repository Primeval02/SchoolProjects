#!/usr/bin/perl

#************************************************
# NAME: Bradley Tate
# ASGT: Activity 3
# ORGN: CSUB - CMPS 3500
# FILE: StackOverflow.perl
# DATE: 03/07/2023
# ***********************************************

$infile = "StackOverflow.csv";
#$weboutfile = "web_regex.txt";
#$emailoutfile = "email_regex.txt";

open(INFILE, $infile);

while(<INFILE>) {
    chomp;

    my $webs = 0;
    my $emails = 0;
    my $HQ = 0;

    $line = $_;

    $line =~ s/ +/ /g;

    ($field1, $field2, $field3, $field4, $field5, $field6) = split ",", $line;

    if ($field3 =~ /[a-zA-Z0-9.-]+@[a-zA-Z]+\.(com|edu|net|gov)/) {
        $emails += 1;
        print "Email found ";
    }

    if ($field6 =~ /HQ/) {
        $HQ += 1;
    }

    print $field1;

}

print "There are $HQ high quality entries in StackOverflow.csv\n";

close(INFILE);
