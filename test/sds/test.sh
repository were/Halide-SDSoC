DIR="$(dirname "${BASH_SOURCE[0]}")"
cd $DIR
for i in `ls`
do
    if [ -d $i ] && [ $i != "support" ]; then
        cd $i
        make "test" &> /dev/null
        ./test.exe &> /dev/null
        if [ $? -ne 0 ]; then
            echo "Fail @" $i
            exit
        fi
        make clean &> /dev/null
        cd ..
        echo $i test passed...
    fi
done
echo "SDS target CSIM feature test done!"
