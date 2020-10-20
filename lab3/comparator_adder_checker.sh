function sum(){
    sum_result=0
    for var in $@; do
        sum_result=$(($sum_result+$var))
    done
    return $sum_result
}

function find_greater(){
    greater=$1
    if [ $1 -lt $2 ]; then
        greater=$2
    fi

    return $greater
}

function check_input(){
    exit_code=0
    for var in $@; do
        if ! [[ "$var" =~ ^[0-9]+$ ]]
        then
            echo "[Error]>> Enter numbers only: "$var
            exit_code=1
        fi
    done

    return $exit_code
}

if [ $# -eq 2 ]
then
    check_input $@
    if [ $? -eq 0 ];then

        sum $@
        echo "sum is:" $?
    
        find_greater $@
        echo "greater number is:" $?
    fi
fi
