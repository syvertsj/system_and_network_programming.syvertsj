#!/usr/bin/env perl 

# perl distro archive script

sub usage { 
	print "$0 [-b|-s|-c|-h]\n"; 
}

sub help { 
	system("perldoc ./$0"); 
}

sub cln {  
	system("rm -r  *.tar"); 
	system("make clean 2>/dev/null"); 
}

sub distro { 
	my $type = shift;
	
	&queryRel($type);

	use Cwd;
	use File::Basename;
	my $cwd = cwd(); 
	my $basedir = basename($cwd); 

	if ($type eq "binary") { 
   	my $hostname = &queryHost;
   	&bin($hostname, $basedir);
   	print "\n$0: binary distribution complete \n\n"; exit 0; 
	} elsif ($type eq "source") { 
		&src($basedir);
   	print "\n$0: source distribution complete \n\n"; exit 0; 
	} else { 
		print "\n$0: $type unsupported distribution type! \n "; 
		die "\n$0 exiting... \n"; 
	}
}

sub bin { 
	my $hostname = shift;
	my $basedir  = shift;
	my $Release  = "smm.$hostname.bin.tar";

	if ( system("make install 2>/dev/null") != 0 ) {
		die "Error with make! $0 exiting...\n"; 
  	} 
	system("tar -C .. -cf $Release $basedir/bin 2>/dev/null");
}

sub src {
	my $basedir  = shift;
	my $Release  = "smm.src.tar";

	if ( system("make clean 2>/dev/null") != 0 ) {
		die "Error with make! $0 exiting...\n"; 
	}	
  	system("tar -C .. -cf $Release $basedir 2>/dev/null");
}

sub queryRel {
   my $type = shift;
   system("clear");
   do {
   	print "You have requested a $type release. Would you like to continue? [Y|N]: "; 
   	my $answer = <STDIN>; chomp($answer);
   	if    ( $answer =~ m/(N|n)/ ) { print "$0 exiting...\n"; exit 1; } 
   	elsif ( $answer =~ m/(Y|y)/ ) { return; } 
   	else  { print "Please answer \"yes\" or \"no\" [Y|N] \n"; }
   } while (1);
}

sub queryHost { 
   use Sys::Hostname; 
   my $hostname = hostname(); 
   do {
      print "Is the hostname: $hostname? [Y|N]: "; 
      my $answer = <STDIN>; chomp($answer);
      if ( $answer =~ m/(Y|y)/ ) { return $hostname; }
      if ( $answer =~ m/(N|n)/ ) { 
         print "Please enter the name of the host machine: ";
         $hostname = <STDIN>; chomp($hostname); }
      else {
         print "Please answer \"yes\" or \"no\" [Y|N] \n"; 
      }
   } while( $answer !~ m/(Y|y)/ ); 
}


## start ##
use Getopt::Std;

getopts("bsch");   # -b -s -c -h options supported
if    ($opt_b) { &distro("binary"); }
elsif ($opt_s) { &distro("source"); }
elsif ($opt_c) { &cln; }
elsif ($opt_h) { &help; exit 0; }
else           { &usage; exit 1; }


# 'perldoc' documentation
# in 'pod' ("plain old display") format, appears as nroff man page format

=head1 NAME

release - perl build script for binary and/or source project release

=head1 SYNOPSIS

    release.pl -s 	(create source distribution)
    release.pl -b 	(build and create binary distribution)

=head1 DESCRIPTION
 
Build and archive project for distribution on unix systems. 

Requires a top-level project Makefile. 

=over 4

=item Other options included:

	release.pl -c	(clean build files from source tree)
	release.pl -h	(show this help page)

=back

James A. Syvertsen 

=cut

