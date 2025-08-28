#!/bin/sh

fprint() {
	 printf "[%s] Test: %-20s Result: %s\n" "$(date '+%Y-%m-%d %H:%M:%S')" "$1" "$2"
}

test_ackermann() {
	capture=$(./slug scripts/ackermann.slg)
	[ "$capture" = "1021" ] && {
		fprint "Ackermann(3,7)" "PASSED";
		return 0;
	} || {
		fprint "Ackermann(3,7)" "FAILED";
		return 2;
	}
}

test_increment() {
	capture=$(./slug scripts/anon_func.slg)
	[ "$capture" = "8" ] && {
		fprint "Increment" "PASSED";
		return 0;
	} || {
		fprint "Increment" "FAILED";
		return 3;
	}
}

test_core_lang() {
	expected="20\n1\n0\n1\n2\n3\n4\n20\n15\n42"
	expected=$(printf '%b' "$expected")
	capture=$(./slug scripts/core_language_test.slg)
	[ "$capture" = "$expected" ] && {
		fprint "Core Language" "PASSED";
		return 0;
	} || {
		fprint "Core Language" "FAILED";
		return 4;
	}
}

test_turing() {
	capture=$(./slug scripts/turing.slg)
	[ "$capture" = "120" ] && {
		fprint "Turing Test" "PASSED";
		return 0;
	} || {
		fprint "Turing Test" "FAILED";
		return 5;
	}
}

{ test_ackermann && test_increment && test_core_lang && test_turing; return="$?"; } || exit 1

[ "$return" -eq 0 ] 2>/dev/null || printf "%s\n" "$return"
