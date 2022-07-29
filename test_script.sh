#!/usr/bin/env bash
make
# diff -y <(nm "$@" 2>/dev/null) <(./ft_nm "$@" 2>/dev/null)
diff <(nm "$@" 2>/dev/null) <(./ft_nm "$@" 2>/dev/null)
