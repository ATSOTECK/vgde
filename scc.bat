@echo off
cloc . --by-file --exclude-dir=include,lib,cmake-build-debug,cmake-build-release,.idea,.git --not-match-f="CMakeLists.txt|glad.c|scc" --quiet --report-file=scc.txt