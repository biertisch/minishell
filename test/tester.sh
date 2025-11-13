#!/bin/bash

MINISHELL=./minishell
TEST_DIR=test
TESTS=$TEST_DIR/tests
RESULTS_DIR=$TEST_DIR/results
touch $TEST_DIR/no_permission
chmod 000 $TEST_DIR/no_permission
TEST_FILES="$TEST_DIR/file1 $TEST_DIR/file2 $TEST_DIR/no_permission"

mkdir -p "$RESULTS_DIR"

# --- Handle Ctrl+C cleanly ---
cleanup() {
    echo ""
    echo "Interrupted. Cleaning up..."
    rm -f "$TEST_DIR"/test_block_*.sh $TEST_FILES
    exit 1
}
trap cleanup INT

# --- Split tests into numbered blocks ---
awk '
  BEGIN { i = 0 }
  /^###/ { i++; x = sprintf("'"$TEST_DIR"'/test_block_%03d.sh", i); next }
  !/^#/ { print > x }   # optional: skip comment lines
' "$TESTS"

i=1
success=0
total=0

for f in "$TEST_DIR"/test_block_*.sh; do
    total=$((total + 1))
    id=$(printf "%03d" $i)
    test_dir="$RESULTS_DIR/$id"
    mkdir -p "$test_dir"

    # --- Save the test file ---
    cp "$f" "$test_dir/command.txt"

    # --- Construct the command line for display ---
    cmd=""
    in_here=0
    delim=""
    while IFS= read -r line || [ -n "$line" ]; do
        # Skip commented lines
        [[ "$line" =~ ^# ]] && continue

        if [[ $in_here -eq 0 ]]; then
            if echo "$line" | grep -q '<<'; then
                delim=$(echo "$line" | awk -F'<<' '{gsub(/^[ \t]+|[ \t]+$/, "", $2); print $2}')
                in_here=1
                cmd+="$line // "
                continue
            fi
        else
            cmd+="$line // "
            [[ "$line" == "$delim" ]] && in_here=0
            continue
        fi

        # Normal line
        [[ -n "$line" ]] && cmd+="$line // "
    done < "$f"
    cmd="${cmd% // }"  # remove trailing " // "

    # --- Run in bash ---
    bash "$f" >"$test_dir/bash.out" 2>"$test_dir/bash.err"
    echo $? >"$test_dir/bash.status"

    # --- Run in minishell (under valgrind) ---
    VALGRIND_LOG="$test_dir/valgrind.log"
    timeout 3s valgrind --quiet \
        --leak-check=full --show-leak-kinds=all --track-fds=yes \
        --log-file="$VALGRIND_LOG" \
        "$MINISHELL" <"$f" >"$test_dir/mini.out" 2>"$test_dir/mini.err"
    echo $? >"$test_dir/mini.status"

    bash_status=$(cat "$test_dir/bash.status")
    mini_status=$(cat "$test_dir/mini.status")

    stdout_diff=$(diff -q "$test_dir/bash.out" "$test_dir/mini.out")
    stderr_diff=$(diff -q "$test_dir/bash.err" "$test_dir/mini.err")

   # --- Memory leaks detection ---
	valgrind_errors=0
	if ! grep -q "ERROR SUMMARY: 0 errors" "$VALGRIND_LOG"; then
		valgrind_errors=1
	fi

	definitely_lost=$(grep -Po "(?<=definitely lost: )\d+" "$VALGRIND_LOG" | head -1)
	if [ -n "$definitely_lost" ] && [ "$definitely_lost" -gt 0 ]; then
		valgrind_errors=1
	fi

	# --- Build warning reasons ---
	warn_reasons=()
	[[ -n "$stdout_diff" ]] && warn_reasons+=("stdout mismatch")
	[[ -n "$stderr_diff" ]] && warn_reasons+=("stderr mismatch")
	[[ "$bash_status" != "$mini_status" ]] && warn_reasons+=("exit status mismatch")
	[[ "$valgrind_errors" -eq 1 ]] && warn_reasons+=("memory errors")

    # --- Save diffs for inspection ---
    diff -u "$test_dir/bash.out" "$test_dir/mini.out" >"$test_dir/diff.stdout" || true
    diff -u "$test_dir/bash.err" "$test_dir/mini.err" >"$test_dir/diff.stderr" || true

    # --- Evaluate result ---
    warn_reasons=()
    [[ -n "$stdout_diff" ]] && warn_reasons+=("stdout mismatch")
    [[ -n "$stderr_diff" ]] && warn_reasons+=("stderr mismatch")
    [[ "$bash_status" != "$mini_status" ]] && warn_reasons+=("exit status mismatch")
    [[ "$leaks_found" -eq 1 ]] && warn_reasons+=("memory leaks")

    # --- Special rule: green tick if only stderr differs ---
    only_stderr_warn=false
    if [[ ${#warn_reasons[@]} -eq 1 && "${warn_reasons[0]}" == "stderr mismatch" ]]; then
        only_stderr_warn=true
    fi

    if [[ ${#warn_reasons[@]} -eq 0 || $only_stderr_warn == true ]]; then
        printf "[%03d] ✅  %s\n" "$i" "$cmd"
        success=$((success + 1))
        [[ $only_stderr_warn == true ]] && echo "      stderr mismatch"
        [[ $leaks_found -eq 1 ]] && echo "      memory leaks"
    else
        printf "[%03d] ⚠️  %s\n" "$i" "$cmd"
        for reason in "${warn_reasons[@]}"; do
            echo "      $reason"
        done
    fi

    # --- Save summary for each test ---
    {
        echo "Test: $id"
        echo "Command: $cmd"
        echo "Bash exit: $bash_status"
        echo "Mini exit: $mini_status"
        [[ -n "$stdout_diff" ]] && echo "stdout differs"
        [[ -n "$stderr_diff" ]] && echo "stderr differs"
        [[ "$leaks_found" -eq 1 ]] && echo "memory leaks detected"
    } >"$test_dir/result.txt"

    i=$((i + 1))
done

# --- Summary ---
echo ""
echo "Summary: $success / $total tests passed ✅"

# --- Clean temporary test block files ---
rm -f $TEST_DIR/test_block_*.sh $TEST_FILES
