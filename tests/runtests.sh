#!/bin/bash

echo "$(tput setaf 3)Running unit tests$(tput sgr0)"
for test in tests/*_test*
do
	if [ -f $test ]
		then
		./$test > /tmp/test.log 2>&1
		if [ $? -eq 0 ]
		then
			echo "$(tput setaf 2)$test passed $(tput bold)(✓)$(tput sgr0)"
			rm -f /tmp/test.log
		else
			echo "$(tput setaf 1)$test failed $(tput bold)(✗)$(tput sgr0)"
			echo "$(tput setaf 1)$test's output (stdout+stderror):$(tput sgr0)"
			cat /tmp/test.log
			rm -f /tmp/test.log
			exit 1
		fi
	fi
done

echo
echo "$(tput setaf 2)All tests passed! $(tput bold)(✔)$(tput sgr0)"
exit 0
