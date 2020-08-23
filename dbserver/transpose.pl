#!/usr/bin/perl
#
# Transpose content of a tab-delimited text file.
$infile = $ARGV[0];
$outfile = ">" . $ARGV[1];

open (infilep, "$infile");
open (outfilep, "$outfile");
$outfile = ">>" . $ARGV[1];
open (outfilep, "$outfile");

$counter = 0;

while(<infilep>)
{
  chomp;
  @var = split //;
  if($var[0] eq "#")
  {
  	next;
  }
  @line = split /\t/;
  if($counter == 0)
  {
  	$line[0] = $ARGV[2];
  }
  
  $lastcol_OLD = $lastcol;
  $lastcol = $#line if $#line > $lastcol;
  
  for(my $i = $lastcol_OLD; $i < $lastcol; $i++)
  {
     $outline[$i] = "\t" x $lastcol_OLD;
  }
   
  for(my $i = 0; $i <= $lastcol; $i++)
  {
     $outline[$i] .= "$line[$i]\t"
  }
      
  $counter = $counter + 1;
  
  open (outfile2, ">.trackITR.txt"); #keep track of iteration
  printf outfile2 "%d\n", $counter;
}

for(my $i = 0; $i <= $lastcol; $i++)
{
  $outline[$i] =~ s/\s*$//g;
  printf outfilep "%s\n", $outline[$i];
  
  open (outfile2, ">.trackITR.txt"); #keep track of iteration
  printf outfile2 "%d\n", $i;
}

open (outfile1, ">.statusout.txt");
printf outfile1 "%d\n", "1";
