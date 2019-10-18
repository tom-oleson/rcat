# rcat
Concatenate files to remote and print remote input on standard output



<pre>rcat [destination] port [FILE]...</pre>

Examples:

// read from standard input and print remote input on standard output
<pre>$ rcat localhost 1234 </pre>


// pipe message with single quotes and print remote input on standard output
<pre>$ echo $"name=\'Tom Oleson\'" | rcat localhost 1234 </pre>


// send input file to remote and save remote input to output file
<pre>$ rcat localhost 1234 request.txt > response.txt</pre>