#!/bin/bash

g++ --std=c++17 -O3 -Wall -Wextra -Werror `pkgconf --cflags smartcols` -l smartcols  dnf_repoinfo_smartcols.cpp -o repoinfo
g++ --std=c++17 -O3 -Wall -Wextra -Werror `pkgconf --cflags smartcols` -l smartcols  dnf_repolist_smartcols.cpp -o repolist
g++ --std=c++17 -O3 -Wall -Wextra -Werror `pkgconf --cflags smartcols` -l smartcols  dnf_info_smartcols.cpp     -o info
g++ --std=c++17 -O3 -Wall -Wextra -Werror `pkgconf --cflags smartcols` -l smartcols  transactionsmartcols.cpp   -o transaction

