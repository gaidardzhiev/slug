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


{ test_ackermann && test_increment; return="$?"; } || exit 1

[ "$return" -eq 0 ] 2>/dev/null || printf "%s\n" "$return"
