#!/usr/bin/env bash
make
# diff -y <(nm "$@") <(./ft_nm "$@") 2>&1
diff <(nm "$@") <(./ft_nm "$@") 2>&1
