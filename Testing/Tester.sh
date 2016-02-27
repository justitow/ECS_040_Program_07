#! /bin/bash

testerDirectory="/home/ssdavis/40/p7/Testing"

if [ $# -ne 1 ]; then
  echo "usage Tester.sh: $testerDirectory/Tester.sh decipher.out_directory" 1>&2
  exit
fi

if [[ $1 =~ Testing ]]; then # Use from this directory is not allowed
  echo "You may not use Testing as your own source directory!"  1>&2
  exit
fi

if [[ ! -e $1/decipher.out ]]; then # decipher.out not found
  echo "decipher.out not found in $1!"  1>&2
  exit
fi

cd $1
cp $testerDirectory/words $testerDirectory/frequencies .
fileNum=1
opScore=0
echo Operation testing:  1>&2

while [[ $fileNum -lt 9 ]]; do
  echo $fileNum  1>&2
  echo "File#: $fileNum Extra Credit" >> operationsResults.txt
  rm -f out$fileNumXC.txt  
  (decipher.out $testerDirectory/File$fileNum.txt.enc | head -100 > out${fileNum}XC.txt )&
  sleep 2
  pkill decipher.out &> /dev/null
  rm core &> /dev/null
  if [[ -s out${fileNum}XC.txt ]]; then
    diff out${fileNum}XC.txt $testerDirectory/out${fileNum}XC.txt > temp

    if [ -s temp ] ; then
      echo "Not OK" >> operationsResults.txt
      echo "Yours:" >> operationsResults.txt
      cat out${fileNum}XC.txt >> operationsResults.txt
      echo -e "\n\nSean's:" >> operationsResults.txt
      cat  $testerDirectory/out${fileNum}XC.txt >> operationsResults.txt
      echo "Errors in extra credit, so using standard grading."  >> operationsResults.tx
      break
    else
      echo "OK" >> operationsResults.txt
      (( opScore += 2 ))
    fi
  else
    echo "No output created so zero for operation." >> operationsResults.txt
  fi

  fileNum=$((fileNum + 1))
done  # while less than 9 

if [ $opScore -eq 16 ]; then
  echo "Full extra credit score: 29"  >> operationsResults.txt 
  opScore=29  # full extra credit
else
  fileNum=1
  opScore=0
  while [[ $fileNum -lt 7 ]]; do
    echo $fileNum  1>&2
    echo "File#: $fileNum" >> operationsResults.txt
    rm -f out$fileNum.txt
    (decipher.out $testerDirectory/File$fileNum.txt.enc | head -100 > out$fileNum.txt )&
    sleep 2
    pkill decipher.out &> /dev/null
    rm core &> /dev/null

    if [[ -s out$fileNum.txt ]]; then
      diff out$fileNum.txt $testerDirectory/out$fileNum.txt > temp

      if [ -s temp ] ; then
        echo "Not OK" >> operationsResults.txt
        echo "Yours:" >> operationsResults.txt
        cat out$fileNum.txt >> operationsResults.txt
        echo -e "\n\nSean's:" >> operationsResults.txt
        cat  $testerDirectory/out$fileNum.txt >> operationsResults.txt
      else
        echo "OK" >> operationsResults.txt
        (( opScore += 4 ))
      fi
    else
      echo "No output created so zero for operation." >> operationsResults.txt
    fi

  fileNum=$((fileNum + 1))
  done  # while less than 7 
fi # else not XC

echo $opScore   # this is the only line that goes to stdout

