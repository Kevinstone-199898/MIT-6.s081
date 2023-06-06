mkdir a
mkdir c
echo hello > c/b
echo hello > b
echo hello world > a/b
find . b | xargs grep hello
