# PIPES
ls | grep minishell
ls | wc -l
cat Makefile | head -n 5
grep -i include src/main.c | sort | uniq
cat Makefile | grep SRC | cut -d'=' -f2 | tr ' ' '\n' | wc -l
find . -type f | wc -l
ls | sort | uniq | rev | rev
cat test/tests.sh | grep world | wc -w
ls -la | tail -n 3 | head -n 1
cat Makefile | tr a-z A-Z | grep SRC | wc -c
ls /bin | grep sh | head -n 3 | tail -n 1
seq 1 100 | grep 42 | wc -l
yes | head -n 100 | wc -l

# invalid/incomplete
| ls
ls | | wc
ls |
| | |
idontexist | wc -l
ls | idontexist

