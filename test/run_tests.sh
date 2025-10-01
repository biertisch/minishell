#!/bin/bash

MINISHELL=./minishell
TEST_DIR=test
TESTS=$TEST_DIR/tests.sh
BASH_OUT=$TEST_DIR/bash
MINI_OUT=$TEST_DIR/mini
DIFF_OUT=$TEST_DIR/diffs
TEST_FILES="$TEST_DIR/file1 $TEST_DIR/file2"

mkdir -p "$BASH_OUT" "$MINI_OUT" "$DIFF_OUT"

i=1
while IFS= read -r cmd; do
    # Skip empty lines and comments
    [[ -z "$cmd" || "$cmd" =~ ^# ]] && continue

    echo "[$i] Testing: $cmd"

    # --- Run in bash ---
    echo "$cmd" | bash >"$BASH_OUT/$i.out" 2>"$BASH_OUT/$i.err"
    echo $? >"$BASH_OUT/$i.status"

    # --- Run in minishell ---
    echo "$cmd" | $MINISHELL >"$MINI_OUT/$i.out" 2>"$MINI_OUT/$i.err"
    echo $? >"$MINI_OUT/$i.status"

    # --- Compare outputs ---
    {
        diff -u "$BASH_OUT/$i.out" "$MINI_OUT/$i.out" || true
        diff -u "$BASH_OUT/$i.err" "$MINI_OUT/$i.err" || true
        diff -u "$BASH_OUT/$i.status" "$MINI_OUT/$i.status" || true
    } >"$DIFF_OUT/$i.diff"

    if [[ -s "$DIFF_OUT/$i.diff" ]]; then
        echo "❌ Difference found (see $DIFF_OUT/$i.diff)"
    else
        echo "✅ Match"
        rm "$DIFF_OUT/$i.diff"  # Clean diffs when there’s no mismatch
    fi
	echo ""

    i=$((i+1))
done < "$TESTS"

rm -f $TEST_FILES