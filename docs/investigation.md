
grep -E "(.text |.rodata)" pigsnkings.map | awk 'BEGIN { FS = " " } ; { print $3 " "$4 }' | sort -n
