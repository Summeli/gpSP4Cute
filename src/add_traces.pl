# add_traces.pl (C) Marko Kivijärvi 2006
# Dummy checks
die "Specify an input file!\n" if $ARGV[0] eq "";
die "File not found!\n" unless -e $ARGV[0];
die "Incorrect file extension for a C/C++ file!\n" 
                if ( $ARGV[0] !~ /(.*)\.(c|cpp)$/ );
 
# Constants
my $INC_TRACER_H = "#include \"tracer.h\"\n";
my $TRACER = "TRACER";
 
# Parse output filename from the input filename
my $file = $ARGV[0];
my $origFile = $1."-orig.".$2;
system( "copy $file, $origFile" );
 
# Reset the input record separator (newline) so the entire file is read at once
undef $/;
 
# Read the input file
$_ = <>; # All there
 
# Adds a tracer macro after each function definition
s/
    (\b\w*?\b[&*]?)?            # Possible function return type
    \s+                         # One or more empty spaces
    (\b\w+?\b)                  # Class name
    \s*?                        # Possible empty space
    ::                          # ::
    \s*?                        # Possible empty space
    (~?\b\w+?\b)                # Function name
    \s*?                        # Possible empty space
    \(                          # Opening brace
    ([^)]*?)                    # Possible function parameters
    \)                          # Closing brace
    \s*?                        # Possible empty space
    (const)?                    # Possible 'const'
    [^{;\/]*?                   # Possible empty space or constructor
                                # initializer list
    \{                          # Opening curly brace
/
    Parse($&,$1,$2,$3,$4,$5)    # Print the match and add the macro
/gxe; # g = repeat, x = split regex to multiple lines, e = evaluate substitution
 
open OUT, ">$file" or die "Cannot open file $file $!\n";
print OUT $INC_TRACER_H;
print OUT;
close OUT;
 
exit 0;
 
sub Parse {
    my $match   = shift;
    my $ret     = shift;
    my $class   = shift;
    my $func    = shift;
    my $param   = shift;
    my $const   = shift;
 
    foreach ( $ret, $class, $func, $param ) {
        s/^\s+|\s+$//g;
        s/\n//g;
    }
 
    my $debug = $match."\n    ";
    $debug .= $TRACER."(\"";
    $debug .= $ret." " if defined $ret;
    $debug .= $class."::".$func."(";
    $debug .= $param if $param ne "";
    $debug .= ")";
    $debug .= " ".$const if defined $const;
    $debug .= "\")";
 
    return $debug;
}