#!/usr/bin/env bash

if [ $# -eq 0 ]; then
	echo "Please give your secret key as an argument."
	exit 1
fi

key="$1"
i=0

echo "Working ..."
while :
do
	hash="$(echo -n "$key$i" | md5sum)"
	if [[ "$hash" == 000000* ]]; then
		echo "Found correct integer! $i"
		echo "$hash"
		break
	fi
	i=$((i + 1))		
done

