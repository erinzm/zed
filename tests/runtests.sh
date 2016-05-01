#!/bin/bash

test_failed=false

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
      test_failed=true
			echo "$(tput setaf 1)$test failed $(tput bold)(✗)$(tput sgr0)"
			echo "$(tput setaf 1)$test's output (stdout+stderror):$(tput sgr0)"
			cat /tmp/test.log
			rm -f /tmp/test.log
		fi
	fi
done

echo
if [ "$test_failed" != "true" ]; then
  echo "$(tput setaf 2)All tests passed! $(tput bold)(✔)$(tput sgr0)"
else
  echo "$(tput setaf 1)Some tests failed! $(tput bold)(✓)$(tput sgr0)"
fi

exit 0
