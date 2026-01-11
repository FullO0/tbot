#!/bin/bash

ROOT="../.."
INDIR="./in"
OUTDIR="./out"
EXPDIR="./exp"
ERRDIR="./err"
EXEC="$ROOT/bin/test_logger"
pass=0
fail=0

FAKETIME="2000-01-01 00:00:00"

# Check if compiler exits
if [ ! -f "$EXEC" ]; then
	echo -e "\033[33mtest_logger does not exist, making test_logger..."
	cd "$ROOT/"
	make test_logger
	cd "./tests/logger/"
	echo -n -e "\x1b[m"
fi

# Save log file
cp ~/tbot.log ".tbot.log.tmp"

echo "Testing logger.c..."
# Loop over all files in the input directory
for filepath in $INDIR/*.in; do
	filename=$(basename "$filepath" .in)

	echo -n "Testing $filename... "

	# Run test_logger with said script
	faketime -f "$FAKETIME" "$EXEC" $filepath 2> "$ERRDIR/$filename.err"
	cp ~/tbot.log "$OUTDIR/$filename.log"

	# Check if Log function is correct
	diff -w "$OUTDIR/$filename.log" "$EXPDIR/$filename.log" > /dev/null

	if [ $? -ne 0 ]; then
		echo -e "\x1b[31mFAIL\x1b[m in .log file"
		((fail++))
	else
		diff -w "$EXPDIR/$filename.err" "$ERRDIR/$filename.err" > /dev/null

		# Check exit status for diff
		if [ $? -eq 0 ]; then
			echo -e "\x1b[32mPASSED\x1b[m"
			((pass++))
		else
			echo -e "\x1b[31mFAIL\x1b[m in .err file"
			((fail++))
		fi
	fi
done

# Reset log file contents to original contents
cp ".tbot.log.tmp" ~/tbot.log
rm -rf ".tbot.log.tmp"

((fail+=pass))
echo -e "\x1b[32m$pass/$fail PASSED\x1b[m"
