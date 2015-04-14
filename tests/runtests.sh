echo "Running unit tests"
for test in test/*_test
do
	if [[ -f $test ]]
		then
		./$test 2>&1 /tmp/test.log
		if [[$? -eq 0]]
		then
			echo "$(tput setaf 2)$test passed $(tput bold)(✓)$(tput sgr0)"
			rm -f /tmp/test.log
		else
			echo "$(tput setaf 1)$test failed $(tput bold)(✗)$(tput sgr0)"
			cat /tmp/test.log
			rm -f /tmp/test.log
			exit 1
		fi
	fi
done

echo
echo "$(tput setaf 2)All tests passed! $(tput bold)(✔)$(tput sgr0)"
exit 0