#!/bin/sh

fprint() {
	 printf "[%s] Test: %-20s Result: %s\n" "$(date '+%Y-%m-%d %H:%M:%S')" "$1" "$2"
}

check_ackermann() {
	capture=$(./slug scripts/ackermann.slg)
	[ "$capture" = "1021" ] && {
		fprint "Ackermann(3,7)" "PASS";
		return 0;
	} || {
		fprint "Ackermann(3,7)" "NOT PASS";
		return 2;
	}
}

{ check_ackermann; return="$?"; } || exit 1

[ "$return" -eq 0 ] 2>/dev/null || printf "%s\n" "$return"
