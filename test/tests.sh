# SIMPLE COMMANDS
ls -l
foo

# QUOTES
/bin/echo "hello world"
/bin/echo "hello"
/bin/echo "   spaced      "
/bin/echo he"ll"o
/bin/echo "hello 'world'"
/bin/echo 'hello "world"'
/bin/echo "he"ll"o"
/bin/echo ""
/bin/echo "" hello
/bin/echo hello "&&" /bin/echo bye
/bin/echo hello '&& /bin/echo bye'

# invalid/incomplete
'/bin/echo hello &&' /bin/echo bye
/bin/echo "hello

# REDIRECT
/bin/echo hello > test/file1 && cat test/file1
cat < test/file1
/bin/echo "" > test/file1 && ls >> test/file1 && cat test/file1
/bin/echo "" > test/file1 && grep main src/main.c >> test/file1 && cat test/file1
/bin/echo "" > test/file2 && /bin/echo "hello" > test/file1 && cat test/file1 >> test/file2 && cat test/file2
cat test/heredoc_block1.txt
cat test/heredoc_block2.txt
/bin/echo > test/file1 hello && cat test/file1
/bin/echo "" > test/file1 hello
>test/file1 /bin/echo hello && cat test/file1
>test/file1 && cat test/file1
foo > test/file1 2> test/file2 && cat test/file2
grep main < src/main.c
/bin/echo -e "4\n2\n1\n3" > test/file1 && sort < test/file1
wc -l < test/file1
/bin/echo "" > test/file2 && cat < test/file1 >> test/file2 && cat test/file2
> test/file1 && cat test/file1

# invalid/incomplete
ls >
cat <

# VARIABLE EXPANSION
/bin/echo $USER
/bin/echo "$USER"
/bin/echo '$USER'
/bin/echo "hello $USER"
/bin/echo $?
/bin/echo "hello $USER$USER"
/bin/echo "hello $NONE"
/bin/echo hello $NONE
/bin/echo $
/bin/echo "$"
$NONE /bin/echo hello
echo ~
echo ~+
echo ~-
echo ~/hi
echo ~bhb
echo "~"
echo '~'
echo a~
VAR="ls src" && $VAR
VAR="hi" && VAR2=$VAR && echo $VAR2
VAR="" && echo $VAR
VAR='"echo hello"' && $VAR
VAR=~ && echo $VAR
echo hi'$USER'
VAR="echo bye" && echo hi"$VAR"
VAR="ho bye" && ec$VAR
echo hel"l"o$USER
echo $USER'$USER'
echo "$"USER
echo "$US"ER
/bin/echo hello > $NONE
VAR="ho bye" && ec"$VAR"
VAR="this is a beautiful shell" && echo $VAR
"$NONE" /bin/echo hello
echo "$NONE""Makefile"
echo $"hello"
echo $'hello'
echo $1HOME
echo $42HOME
ls *f
ls *f*
ls **f**
ls *
ls .*
/bin/echo *ile *in*
cat < *ile
/bin/echo *on*
echo Make*
echo Make"*"
VAR=*ile && echo $VAR
VAR=*in* && echo $VAR
/bin/echo hello > *in*
VAR="hi *file" && echo $VAR
VAR="hi *file" && echo "$VAR"
VAR=""*ile"" && echo $VAR
VAR="'*ile'" && echo $VAR

# BUILTIN
echo hello && echo -n bye
echo -nnn hello
echo -f hello
echo -nn hello
cd && pwd && cd - && pwd
cd ~/Documents && pwd && cd - && pwd
cd test && pwd && cd .. && pwd
pwd
export TESTVAR=42 && export | grep TESTVAR && unset TESTVAR
env | grep PATH
exit
exit 2
exit -2
exit p
exit a 1

# invalid/incomplete
env hello
env -w
unset -w
export -l
pwd -f
cd -f
cd . include

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
a | b | c | d | e | f | g

# LOGICAL OPERATORS & PARENTHESES
ls && cat Makefile
cat foo || echo "not found"
(/bin/echo hello && /bin/echo world) | wc -l
/bin/echo hello && /bin/echo world | wc -l
(ls)
((ls))
( (ls) )
((ls) )
( (ls))
/bin/echo a && /bin/echo b || /bin/echo c
/bin/echo a || /bin/echo b && /bin/echo c

# invalid/incomplete
()
ls)
(ls) /bin/echo hello
(ls) (ls)
|| ls
&& ls
|| && |
ls ||| wc
/bin/echo "start" && (ls -l | grep minishell) >
(ls
/bin/echo (
ls &&
ls ||

# COMBINATIONS
/bin/echo "start" && (ls -l | grep minishell) && /bin/echo end
/bin/echo "start" && (ls -l | grep minishell) > test/file1 && cat test/file1
/bin/echo "start" && (ls -l | grep minishell) | /bin/echo end
ls -l > test/file1 && (cat < test/file1 || echo fail) | wc -l
echo $SHLVL && ./minishell && echo $SHLVL