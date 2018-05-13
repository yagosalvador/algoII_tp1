#!/usr/bin/env sh

PATH="./ejemplos/"
declare -i errors=0
for transformado in "$PATH""T"*; do
    original=${transformado#$PATH}
    original=$PATH${original:1}

    # Check if the DFT works
    if ./tp0 -i "$original" | ./diff_complex -i2 "$transformado"; then
        :
    else
        errors=$errors+1
        echo "Error in ""$original"
    fi

    # Check if the IDFT works
    if ./tp0 -i "$transformado" -m IDFT | ./diff_complex -i2 "$original"; then
        :
    else
        errors=$errors+1
        echo "Error in ""$transformado"
    fi

done
echo "A total of ""$errors"" errors were found."
