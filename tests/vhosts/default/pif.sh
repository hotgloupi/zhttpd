#!/bin/sh
printf "Content-Type: text/html\r\n\r\n"
date
echo '<pre>'
ps aux | grep ./bin/server | sed 's/</\&lt;/g'
echo '</pre>'

