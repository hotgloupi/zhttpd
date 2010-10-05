#!/bin/sh
file=image.jpg
printf "Content-Type: image/jpeg\r\n"
printf "Content-Disposition: filename=pif.jpg\r\n"
printf "Content-Length: `du -b $file | cut -f1`\r\n\r\n"

cat $file

