#!/bin/bash

MINISHELL=./minishell
TEST_DIR=test
TESTS=$TEST_DIR/tests.sh
BASH_OUT=$TEST_DIR/bash
MINI_OUT=$TEST_DIR/minishell
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

    # --- Run in minishell (with timeout + valgrind) ---
    VALGRIND_LOG="$MINI_OUT/$i.valgrind"

    if printf '%s\n' "$cmd" | timeout 2s valgrind --leak-check=full --errors-for-leak-kinds=definite \
        --log-file="$VALGRIND_LOG" $MINISHELL >"$MINI_OUT/$i.out" 2>"$MINI_OUT/$i.err"; then
        echo $? >"$MINI_OUT/$i.status"
    else
        status=$?
        if [[ $status -eq 124 ]]; then
            echo 124 >"$MINI_OUT/$i.status"
            echo "⏱️ Minishell hung (timeout)"
        elif [[ $status -ge 128 ]]; then
            echo $status >"$MINI_OUT/$i.status"
            echo "💥 Minishell crashed (signal $((status-128)))"
        fi
    fi

    # --- Check valgrind log ---
    if grep -q "ERROR SUMMARY: [^0]" "$VALGRIND_LOG"; then
        echo "💧 Memory errors detected (see $VALGRIND_LOG)"
    else
        rm -f "$VALGRIND_LOG"  # clean log if no issues
    fi

    # --- Compare outputs ---
    {
        diff -u "$BASH_OUT/$i.out" "$MINI_OUT/$i.out" || true
        # compare normalized error outputs
        sed -E 's/^(bash: line [0-9]+: |minishell: )//' "$BASH_OUT/$i.err" >"$BASH_OUT/$i.err.norm"
        sed -E 's/^(bash: line [0-9]+: |minishell: )//' "$MINI_OUT/$i.err" >"$MINI_OUT/$i.err.norm"
        diff -u "$BASH_OUT/$i.err.norm" "$MINI_OUT/$i.err.norm" || true
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