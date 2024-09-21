#!/bin/bash
IFS=$'\n'

readuntil() {
    while read line; do
        echo "$line"
        [[ "$line" == "}" ]] && break
    done
}

last=
for i in *.cpp; do
    n=0
    while read line; do
        echo "$line"
        [[ "$line" == *"namespace"* ]] && continue
        if [[ "$line" == "{" ]]; then
            last="$(grep -Poh '[a-zA-Z:0-9_-]*\(' <<< "$last")"
            echo 'meowln("'"$i:$n ${last//(}"'");'
            readuntil
        fi
        if [[ "$line" != " "* ]]; then
            last="$line"
        fi
        (( n++ ))
    done < $i
done
