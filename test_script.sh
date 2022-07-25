#!/usr/bin/env bash
make
diff -y <(nm $1) <(./ft_nm $1) 2>&1
