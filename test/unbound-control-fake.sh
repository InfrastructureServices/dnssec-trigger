#!/bin/bash

if [ $# -eq 3 ]
then
	if [ $1 = "local_zone" ] && [ $2 = "test" ] && [ $3 = "static" ]
	then
		echo ok
	else
		echo fail
	fi
else
	echo fail
fi

