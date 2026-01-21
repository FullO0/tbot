#!/bin/bash
############################################################################
# DO NOT RUN THIS MANUALLY, RUN BY COMPILING test_logger WITH THE MAKEFILE #
############################################################################

TESTDIR="$1"
EXEC="$2"
INDIR="$TESTDIR/in"
OUTDIR="$TESTDIR/out"
EXPDIR="$TESTDIR/exp"
ERRDIR="$TESTDIR/err"
pass=0
fail=0

FAKETIME="2000-01-01 00:00:00"

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
			echo -e "\x1b[32mPASS\x1b[m"
			((pass++))
		else
			echo -e "\x1b[31mFAIL\x1b[m in .err file"
			((fail++))
		fi

		# Clear tbot.log for next test
		rm -rf ~/tbot.log
		touch ~/tbot.log
	fi
done

# Reset log file contents to original contents
cp ".tbot.log.tmp" ~/tbot.log
rm -rf ".tbot.log.tmp"

((fail+=pass))
echo -e "\x1b[32m$pass/$fail PASSED\x1b[m"
