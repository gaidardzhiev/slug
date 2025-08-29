#!/bin/sh

G='\033[0;32m'
R='\033[0;31m'
N='\033[0m'

[ ! -f slug ] && make

fprint() {
	 printf "[%s] Test: %-20s Result: %b\n" "$(date '+%Y-%m-%d %H:%M:%S')" "$1" "$2"
}

test_ackermann() {
	capture=$(./slug scripts/ackermann.slg)
	[ "$capture" = "1021" ] && {
		fprint "Ackermann(3,7)" "${G}PASSED${N}";
		return 0;
	} || {
		fprint "Ackermann(3,7)" "${R}FAILED${N}";
		return 2;
	}
}

test_increment() {
	capture=$(./slug scripts/anon_func.slg)
	[ "$capture" = "8" ] && {
		fprint "Increment" "${G}PASSED${N}";
		return 0;
	} || {
		fprint "Increment" "${R}FAILED${N}";
		return 3;
	}
}

test_core_lang() {
	expected="20\n1\n0\n1\n2\n3\n4\n20\n15\n42"
	expected=$(printf '%b' "$expected")
	capture=$(./slug scripts/core_language_test.slg)
	[ "$capture" = "$expected" ] && {
		fprint "Core Language" "${G}PASSED${N}";
		return 0;
	} || {
		fprint "Core Language" "${R}FAILED${N}";
		return 4;
	}
}

test_turing() {
	capture=$(./slug scripts/turing.slg)
	[ "$capture" = "120" ] && {
		fprint "Turing Completeness" "${G}PASSED${N}";
		return 0;
	} || {
		fprint "Turing Completeness" "${R}FAILED${N}";
		return 5;
	}
}

test_hof() {
	capture=$(./slug scripts/higher_order_functions_and_closures.slg)
	[ "$capture" = "25" ] && {
		fprint "Higher Order" "${G}PASSED${N}";
		return 0;
	} || {
		fprint "Higher Order" "${R}FAILED${N}";
		return 6;
	}
}

test_recursion() {
	capture=$(./slug scripts/recursion.slg)
	[ "$capture" = "120" ] && {
		fprint "Recursion" "${G}PASSED${N}";
		return 0;
	} || { 
		fprint "Recursion" "${R}FAILED${N}";
		return 7;
	}
}

{ test_ackermann && test_increment && test_core_lang && test_turing && test_hof && test_recursion; return="$?"; } || exit 1

[ "$return" -eq 0 ] 2>/dev/null || printf "%s\n" "$return"
