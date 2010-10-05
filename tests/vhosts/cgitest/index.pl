#!/usr/bin/perl
print "Content-type: text/html\r\n\r\n

<head>
	<title> CGI Tutorial: Environment variables script</title>
</head>
<body>
";

print "

<hr>
<h1> Environment variables</h1>
<hr>
<p>
<hr>
";
foreach (sort keys %ENV)
{
  print "<b>$_</b>: $ENV{$_}<br>\n";
}

1;

print "</body></html>";

