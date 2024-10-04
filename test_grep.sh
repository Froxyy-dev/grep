#!/bin/bash

# Function to run a single test case
run_test() {
    input="$1"
    pattern="$2"
    expected_exit_code="$3"
    
    echo -n "$input" | ./server -E "$pattern"
    actual_exit_code=$?

    if [ $actual_exit_code -eq $expected_exit_code ]; then
        echo "Test passed: '$pattern' with input '$input'"
    else
        echo "Test failed: '$pattern' with input '$input'. Expected $expected_exit_code but got $actual_exit_code."
        exit 1
    fi
}

# Test cases
run_test "'cat and cat' is the same as 'cat and cat'" "('(cat) and \\2') is the same as \\1" 0
run_test "'cat and cat' is the same as 'cat and dog'" "('(cat) and \\2') is the same as \\1" 1
run_test "grep 101 is doing grep 101 times, and again grep 101 times" "((\\w\\w\\w\\w) (\\d\\d\\d)) is doing \\2 \\3 times, and again \\1 times" 0
run_test "$?! 101 is doing $?! 101 times, and again $?! 101 times" "((\\w\\w\\w) (\\d\\d\\d)) is doing \\2 \\3 times, and again \\1 times" 1
run_test "grep yes is doing grep yes times, and again grep yes times" "((\\w\\w\\w\\w) (\\d\\d\\d)) is doing \\2 \\3 times, and again \\1 times" 1
run_test "abc-def is abc-def, not efg, abc, or def" "(([abc]+)-([def]+)) is \\1, not ([^xyz]+), \\2, or \\3" 0
run_test "efg-hij is efg-hij, not klm, efg, or hij" "(([abc]+)-([def]+)) is \\1, not ([^xyz]+), \\2, or \\3" 1
run_test "abc-def is abc-def, not xyz, abc, or def" "(([abc]+)-([def]+)) is \\1, not ([^xyz]+), \\2, or \\3" 1
run_test "apple pie is made of apple and pie. love apple pie" "^((\\w+) (\\w+)) is made of \\2 and \\3. love \\1$" 0
run_test "pineapple pie is made of apple and pie. love apple pie" "^((apple) (\\w+)) is made of \\2 and \\3. love \\1$" 1
run_test "apple pie is made of apple and pie. love apple pies" "^((\\w+) (pie)) is made of \\2 and \\3. love \\1$" 1
run_test "'howwdy hey there' is made up of 'howwdy' and 'hey'. howwdy hey there" "'((how+dy) (he?y) there)' is made up of '\\2' and '\\3'. \\1" 0
run_test "'hody hey there' is made up of 'hody' and 'hey'. hody hey there" "'((how+dy) (he?y) there)' is made up of '\\2' and '\\3'. \\1" 1
run_test "'howwdy heeey there' is made up of 'howwdy' and 'heeey'. howwdy heeey there" "'((how+dy) (he?y) there)' is made up of '\\2' and '\\3'. \\1" 1
run_test "cat and fish, cat with fish, cat and fish" "((c.t|d.g) and (f..h|b..d)), \\2 with \\3, \\1" 0
run_test "bat and fish, bat with fish, bat and fish" "((c.t|d.g) and (f..h|b..d)), \\2 with \\3, \\1" 1

