function calculate(){
    num1=$1
    operand=$2
    num2=$3

    case "$operand" in
        "/")
            result=$(($num1/$num2))
        ;;

        "+") 
            result=$(($num1+$num2))
        ;;

        "-")
            result=$(($num1-$num2))
        ;;

        "x")
            result=$((num1*num2))
        ;;
    esac

    return $result

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

if [ $# -eq 3 ]; then
    check_input $1 $3
    if [ $? -eq 0 ]; then
        calculate $@
        echo Result: $?
    fi
else
    echo not enough args
fi
