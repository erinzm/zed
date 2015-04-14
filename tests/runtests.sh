echo
echo "Running unit tests"
for test in test/*_test do
	if [[ -f $test ]] then
		./$test 2>&1 /tmp/test.log
		if [[$? -eq 0]] then
			rm -f /tmp/test.log
		else
			cat /tmp/test.log
			rm -f /tmp/test.log
			exit 1
		fi
	fi
done