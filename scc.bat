@echo off
cloc . --by-file --exclude-dir=include,lib --not-match-f="glad.c|scc" --quiet --report-file=scc.txt