#!/usr/bin/env bash

# ------------- CONFIG -------------
PIPEX=./pipex
TEST_PREFIX="px_"

INFILE="${TEST_PREFIX}in.txt"
INFILE_EMPTY="${TEST_PREFIX}in_empty.txt"

# ------------- COLORS -------------
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
BOLD="\033[1m"
RESET="\033[0m"

CHECK_MARK="✓"
CROSS_MARK="✗"

# ------------- VALGRIND CONFIG -------------
# Set USE_VALGRIND=0 if you want to disable it quickly
USE_VALGRIND=1
VALGRIND="valgrind"
VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --quiet"

TOTAL=0
PASSED=0
FAILED=0

# Check if valgrind is available
if [ "$USE_VALGRIND" -eq 1 ]; then
    if ! command -v "$VALGRIND" >/dev/null 2>&1; then
        printf "%b[WARN]%b valgrind not found, disabling valgrind checks\n" "$YELLOW" "$RESET"
        USE_VALGRIND=0
    else
        printf "%b[INFO]%b valgrind checks enabled\n" "$BLUE" "$RESET"
    fi
fi

# ------------- CLEANUP -------------
cleanup() {
    printf "%b[CLEANUP]%b removing test files (keeping *.vg.log)\n" "$BLUE" "$RESET"
    rm -f "$INFILE" "$INFILE_EMPTY"
    rm -f "${TEST_PREFIX}"ref* "${TEST_PREFIX}"out* "${TEST_PREFIX}"*.err 2>/dev/null
}

# ------------- HELPERS -------------

run_ok_test() {
    desc="$1"
    shell_cmd="$2"
    pipex_cmd="$3"
    ref_file="$4"
    out_file="$5"

    TOTAL=$((TOTAL + 1))
    printf "%b[TEST %2d]%b %s\n" "$BLUE" "$TOTAL" "$RESET" "$desc"

    rm -f "$ref_file" "$out_file"
    local vg_log=""

    # reference with real shell pipeline
    eval "$shell_cmd"

    # run your pipex (maybe under valgrind)
    if [ "$USE_VALGRIND" -eq 1 ]; then
        vg_log="${out_file}.vg.log"
        rm -f "$vg_log"
        eval "$VALGRIND $VALGRIND_OPTS --log-file=$vg_log $pipex_cmd"
    else
        eval "$pipex_cmd"
    fi
    status=$?

    if [ "$status" -ne 0 ]; then
        printf "  %b%s%b exit code %d (expected 0)\n" "$RED" "$CROSS_MARK FAIL" "$RESET" "$status"
        FAILED=$((FAILED + 1))
        # still show valgrind info if we have it
        if [ "$USE_VALGRIND" -eq 1 ] && [ -n "$vg_log" ] && [ -f "$vg_log" ]; then
            if grep -q "ERROR SUMMARY: 0 errors" "$vg_log"; then
                printf "  %b[VALGRIND]%b no mem errors (see %s)\n" "$GREEN" "$RESET" "$vg_log"
            else
                printf "  %b[VALGRIND]%b memory errors detected (see %s)\n" "$RED" "$RESET" "$vg_log"
            fi
        fi
        return
    fi

    if diff -q "$ref_file" "$out_file" >/dev/null 2>&1; then
        printf "  %b%s%b outputs identical\n" "$GREEN" "$CHECK_MARK PASS" "$RESET"
        PASSED=$((PASSED + 1))
    else
        printf "  %b%s%b output differs from shell\n" "$RED" "$CROSS_MARK FAIL" "$RESET"
        echo "  --- diff ---"
        diff -u "$ref_file" "$out_file" || true
        FAILED=$((FAILED + 1))
    fi

    # Valgrind summary (does NOT affect PASS/FAILED counters, just info)
    if [ "$USE_VALGRIND" -eq 1 ] && [ -n "$vg_log" ] && [ -f "$vg_log" ]; then
        if grep -q "ERROR SUMMARY: 0 errors" "$vg_log"; then
            if grep -q "definitely lost: 0 bytes" "$vg_log"; then
                printf "  %b[VALGRIND]%b no leaks, no mem errors (see %s)\n" "$GREEN" "$RESET" "$vg_log"
            else
                printf "  %b[VALGRIND]%b no mem errors but some leaks (see %s)\n" "$YELLOW" "$RESET" "$vg_log"
            fi
        else
            printf "  %b[VALGRIND]%b memory errors detected (see %s)\n" "$RED" "$RESET" "$vg_log"
        fi
    fi
}

run_error_test() {
    desc="$1"
    pipex_cmd="$2"
    out_file="$3"

    TOTAL=$((TOTAL + 1))
    printf "%b[TEST %2d]%b %s\n" "$BLUE" "$TOTAL" "$RESET" "$desc"

    rm -f "$out_file"
    err_file="${out_file}.err"
    local vg_log=""

    # run pipex (maybe under valgrind), capture stderr
    if [ "$USE_VALGRIND" -eq 1 ]; then
        vg_log="${out_file}.vg.log"
        rm -f "$vg_log"
        eval "$VALGRIND $VALGRIND_OPTS --log-file=$vg_log $pipex_cmd 2> $err_file"
    else
        eval "$pipex_cmd" 2> "$err_file"
    fi
    status=$?

    if [ "$status" -eq 0 ]; then
        printf "  %b%s%b exit code 0 (expected non-zero)\n" "$RED" "$CROSS_MARK FAIL" "$RESET"
        FAILED=$((FAILED + 1))
    else
        if [ -s "$out_file" ]; then
            printf "  %b%s%b outfile is not empty (expected empty)\n" "$RED" "$CROSS_MARK FAIL" "$RESET"
            FAILED=$((FAILED + 1))
        else
            printf "  %b%s%b non-zero exit (%d) and empty outfile (OK)\n" "$GREEN" "$CHECK_MARK PASS" "$RESET" "$status"
            PASSED=$((PASSED + 1))
        fi
    fi

    # show stderr from program + valgrind
    if [ -s "$err_file" ]; then
        printf "  %bstderr:%b\n" "$YELLOW" "$RESET"
        sed 's/^/    /' "$err_file"
    fi

    # Valgrind summary
    if [ "$USE_VALGRIND" -eq 1 ] && [ -n "$vg_log" ] && [ -f "$vg_log" ]; then
        if grep -q "ERROR SUMMARY: 0 errors" "$vg_log"; then
            if grep -q "definitely lost: 0 bytes" "$vg_log"; then
                printf "  %b[VALGRIND]%b no leaks, no mem errors (see %s)\n" "$GREEN" "$RESET" "$vg_log"
            else
                printf "  %b[VALGRIND]%b no mem errors but some leaks (see %s)\n" "$YELLOW" "$RESET" "$vg_log"
            fi
        else
            printf "  %b[VALGRIND]%b memory errors detected (see %s)\n" "$RED" "$RESET" "$vg_log"
        fi
    fi
}

# ------------- BUILD STEP -------------

printf "%b[BUILD]%b running make\n" "$BLUE" "$RESET"

if [ ! -f Makefile ] && [ ! -f makefile ]; then
    printf "%bError:%b no Makefile found in this directory\n" "$RED" "$RESET"
    exit 1
fi

make
BUILD_STATUS=$?

if [ "$BUILD_STATUS" -ne 0 ]; then
    printf "  %b%s%b compilation failed (exit code %d)\n" "$RED" "$CROSS_MARK FAIL" "$RESET" "$BUILD_STATUS"
    exit 1
fi

if [ ! -x "$PIPEX" ]; then
    printf "%bError:%b %s not found or not executable after make\n" "$RED" "$RESET" "$PIPEX"
    exit 1
fi

printf "  %b%s%b compilation OK, found %s\n\n" "$GREEN" "$CHECK_MARK" "$RESET" "$PIPEX"

# ------------- PREP INPUT -------------

printf "%b[SETUP]%b creating test inputs\n" "$BLUE" "$RESET"

cat > "$INFILE" << 'EOF'
hello world
42 school
aaa bbb
EOF

: > "$INFILE_EMPTY"   # empty file

# ------------- TESTS (OK CASES) -------------

# 1) cat | cat : simple copy
run_ok_test \
  "cat | cat : copy whole file" \
  "< $INFILE cat | cat > ${TEST_PREFIX}ref1" \
  "$PIPEX $INFILE \"cat\" \"cat\" ${TEST_PREFIX}out1" \
  "${TEST_PREFIX}ref1" "${TEST_PREFIX}out1"

# 2) grep 42 | cat
run_ok_test \
  "grep 42 | cat : keep only '42 school'" \
  "< $INFILE grep 42 | cat > ${TEST_PREFIX}ref2" \
  "$PIPEX $INFILE \"grep 42\" \"cat\" ${TEST_PREFIX}out2" \
  "${TEST_PREFIX}ref2" "${TEST_PREFIX}out2"

# 3) grep o | wc -l
run_ok_test \
  "grep o | wc -l : count lines containing 'o'" \
  "< $INFILE grep o | wc -l > ${TEST_PREFIX}ref3" \
  "$PIPEX $INFILE \"grep o\" \"wc -l\" ${TEST_PREFIX}out3" \
  "${TEST_PREFIX}ref3" "${TEST_PREFIX}out3"

# 4) grep 42 | tr a-z A-Z
run_ok_test \
  "grep 42 | tr a-z A-Z" \
  "< $INFILE grep 42 | tr a-z A-Z > ${TEST_PREFIX}ref4" \
  "$PIPEX $INFILE \"grep 42\" \"tr a-z A-Z\" ${TEST_PREFIX}out4" \
  "${TEST_PREFIX}ref4" "${TEST_PREFIX}out4"

# 5) TRICKY: extra spaces in cmd1
run_ok_test \
  "extra spaces in cmd1 (\"   grep   42   \")" \
  "< $INFILE grep 42 | cat > ${TEST_PREFIX}ref5" \
  "$PIPEX $INFILE \"   grep    42   \" \"cat\" ${TEST_PREFIX}out5" \
  "${TEST_PREFIX}ref5" "${TEST_PREFIX}out5"

# 6) TRICKY: absolute path commands
run_ok_test \
  "absolute paths: /bin/cat | /bin/cat" \
  "< $INFILE /bin/cat | /bin/cat > ${TEST_PREFIX}ref6" \
  "$PIPEX $INFILE \"/bin/cat\" \"/bin/cat\" ${TEST_PREFIX}out6" \
  "${TEST_PREFIX}ref6" "${TEST_PREFIX}out6"

# 7) TRICKY: command with option + argument (head -n 1)
run_ok_test \
  "head -n 1 | cat" \
  "< $INFILE head -n 1 | cat > ${TEST_PREFIX}ref7" \
  "$PIPEX $INFILE \"head -n 1\" \"cat\" ${TEST_PREFIX}out7" \
  "${TEST_PREFIX}ref7" "${TEST_PREFIX}out7"

# 8) TRICKY: empty infile – grep should output nothing
run_ok_test \
  "empty infile: grep 42 | wc -l (should be 0)" \
  "< $INFILE_EMPTY grep 42 | wc -l > ${TEST_PREFIX}ref8" \
  "$PIPEX $INFILE_EMPTY \"grep 42\" \"wc -l\" ${TEST_PREFIX}out8" \
  "${TEST_PREFIX}ref8" "${TEST_PREFIX}out8"

# ------------- TESTS (ERROR CASES) -------------

# 9) missing infile
run_error_test \
  "missing infile" \
  "$PIPEX ${TEST_PREFIX}no_such_file \"cat\" \"cat\" ${TEST_PREFIX}out9" \
  "${TEST_PREFIX}out9"

# 10) invalid cmd1
run_error_test \
  "invalid cmd1" \
  "$PIPEX $INFILE \"asdflolnotacommand\" \"cat\" ${TEST_PREFIX}out10" \
  "${TEST_PREFIX}out10"

# 11) invalid cmd2
run_error_test \
  "invalid cmd2" \
  "$PIPEX $INFILE \"cat\" \"asdflolnotacommand\" ${TEST_PREFIX}out11" \
  "${TEST_PREFIX}out11"

# 12) outfile in non-existing directory
run_error_test \
  "outfile in non-existing directory" \
  "$PIPEX $INFILE \"cat\" \"cat\" ${TEST_PREFIX}no_such_dir/out.txt" \
  "${TEST_PREFIX}out12"

# ------------- SUMMARY -------------

echo
printf "%b[SUMMARY]%b\n" "$BOLD" "$RESET"
if [ "$FAILED" -eq 0 ]; then
    printf "  %bAll tests passed%b (%d/%d)\n" "$GREEN" "$RESET" "$PASSED" "$TOTAL"
else
    printf "  %bSome tests failed%b (%d/%d passed, %d failed)\n" "$RED" "$RESET" "$PASSED" "$TOTAL" "$FAILED"
fi

make fclean
cleanup
