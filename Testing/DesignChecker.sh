#!  /bin/bash

testerDirectory="/home/ssdavis/40/p5/Testing"

if [ $# -ne 1 ]; then
  >&2 echo "usage DesignChecker.sh: $testerDirectory/DesignChecker.sh CPU.out_directory"
  exit
fi

if [[ $1 =~ Testing ]]; then # Use from this directory is not allowed
  >&2 echo "You may not use Testing as your own source directory!"
  exit
fi

cd $1

rm -f designResults.txt *.o
designError=0
dos2unix *.cpp *.h &> /dev/null

grep "/\*" *.cpp *.h > temp
if [ -s temp ]; then
  cat temp >> designResults.txt
  echo 'Fatal design error: "/*" found.' >> designResults.txt
  (( ++designError ))
fi

openBracketCount=$( grep -v "//" *.h | grep "{" | wc -l )

if [ $openBracketCount -ne 4 ]; then
  echo 'Fatal design error: Implementation code found in header file.' >> designResults.txt
  (( ++designError ))
fi

if egrep "(struct |stdio|string\.h|stdlib\.h|malloc|free|fopen)" *.h *.cpp >& /dev/null ; then
  echo 'Fatal design error: struct, stdio, string.h, stdlib.h, malloc, free,' >> designResults.txt
  echo 'or fopen found in file(s).' >> designResults.txt
  (( +designError ))
fi

grep -v "^ *//" *.cpp| awk '/void/ , /\)/' | grep ";" > temp

if [ -s temp ]; then
  echo 'Fatal design error: Prototype in *.cpp file.' >> designResults.txt
  (( ++designError ))
fi


if [[ ` grep -v "//" directory.h | grep ";" | wc -l ` -lt 17 ]]; then
  echo 'Fatal design error: Less than 17 ";" not found in directory.h'  >> designResults.txt
  (( ++ designError ))
fi 

if [[ ` grep -v "//" list.h | grep ";" | wc -l ` -lt 13 ]]; then
  echo 'Fatal design error: Less than 13 ";" found in list.h'  >> designResults.txt
  (( ++ designError ))
fi

if [[ ` awk '/protected/, /public/' directory.h | grep -v "//" | grep "char *\* *name" | wc -l ` -ne 1 ]]; then
  echo 'Fatal design error: name is not protected in class File in directory.h'  >> designResults.txt
  (( ++ designError ))
fi

if [[ ` awk '/~ListNode/, /^}/' list.cpp | grep -v "//" | grep "delete" | wc -l ` -ne 1 ]]; then
  echo 'Fatal design error: ListNode destructor does not delete its data in list.cpp'  >> designResults.txt
  (( ++ designError ))
fi

if [[ ` awk '/~List *\(/, /^}/' list.cpp | grep -v "//" | grep "delete " | wc -l ` -ne 1 ]]; then
  echo 'Fatal design error: List destructor does not delete the ListNodes in list.cpp'  >> designResults.txt
  (( ++ designError ))
fi

if [[  ` grep -v "//" list.cpp | grep "template *< *typename *T *>" | wc -l ` -lt 7 ]]; then
  echo 'Fatal design error: Less than seven "template <typename T>" found in list.cpp'  >> designResults.txt
  (( ++ designError ))
fi

if grep "include *<" *.cpp *.h | egrep -v "(iostream|cstring|fstream)" &> /dev/null ; then
  echo 'Standard library other than iostream, cstring, and fstream found.'  >> designResults.txt
  (( ++ designError ))
fi
# non-fatal design errors from here on


if ! grep "List *()" list.h  &> /dev/null  &> /dev/null ; then
  echo "List constructor not found in list.h" >> designResults.txt
fi

if ! grep "bool *find *( *const" list.h  &> /dev/null  &> /dev/null ; then
  echo "bool find(const not found in list.h" >> designResults.txt
fi

if ! grep "void *insert *(" list.h  &> /dev/null  &> /dev/null ; then
  echo "void insert( not found in list.h" >> designResults.txt
fi

if ! grep "void *print *(" list.h  &> /dev/null  &> /dev/null ; then
  echo "void print( not found in list.h" >> designResults.txt
fi

if ! grep -v "//" list.cpp | grep "prev *="  &> /dev/null  ; then
  echo "prev = not found in list.cpp" >> designResults.txt
fi

if awk '/::find/,/^}/' list.cpp | grep -v "//" | grep "ListNode" &> /dev/null ; then
  echo "ListNode found in find()"  >> designResults.txt
  echo "so you won't receive 3 points" >> designResults.txt
  echo "extra credit." >> designResults.txt
fi

if awk '/::insert/,/^}/' list.cpp | grep -v "//" | grep "ListNode.*\*" &> /dev/null ; then
  echo "ListNode pointer found in insert()"  >> designResults.txt
  echo "so you won't receive 2 points extra credit." >> designResults.txt
fi

if [[  ` grep -v "//" list.cpp | egrep "(curr->prev|curr->next)" | wc -l ` -lt 10 ]]; then
  echo "curr was not used" >> designResults.txt
  echo "at least 10 times" >> designResults.txt
  echo "so you " >> designResults.txt
  echo "lose" >> designResults.txt
  echo "5 points." >> designResults.txt
fi

if ! grep -v "//" directory.cpp | grep "File *:: *File *( *const"  &> /dev/null  ; then
  echo "File(const constructor not found in directory.cpp" >> designResults.txt
fi

if ! grep -v "//" directory.cpp | grep "const.*File *:: *getName.*const"  &> /dev/null  ; then
  echo "const char* File::getName() const not found in directory.cpp" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *File *([^;]|$)/,/};/' | grep "virtual *~File"  &> /dev/null  ; then
  echo "virtual File destructor not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *File *([^;]|$)/,/};/' | grep "virtual *bool *find"  &> /dev/null  ; then
  echo "virtual File find not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *File *([^;]|$)/,/};/' | grep "virtual *void *insert"  &> /dev/null  ; then
  echo "virtual File insert not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *File *([^;]|$)/,/};/' | grep "virtual *bool *operator *== *( *const.*const"  &> /dev/null  ; then
  echo "File operator==(const...) const not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *File *([^;]|$)/,/};/' | grep "virtual *bool *operator *< *( *const.*const"  &> /dev/null  ; then
  echo "File operator<(const ...) const not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *File *([^;]|$)/,/};/' |grep "virtual *bool *operator *> *( *const.*const"  &> /dev/null  ; then
  echo "File operator>(const ...) const not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *Directory *([^;]|$)/,/};/' | grep " *Directory *( *const"  &> /dev/null  ; then
  echo " Directory(const constructor not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *Directory *([^;]|$)/,/};/' | grep " *~Directory"  &> /dev/null  ; then
  echo " Directory destructor not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *Directory *([^;]|$)/,/};/' | grep " *bool *find"  &> /dev/null  ; then
  echo " Directory find not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *Directory *([^;]|$)/,/};/' | grep " *void *insert"  &> /dev/null  ; then
  echo " Directory insert not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *Directory *([^;]|$)/,/};/' | grep " *bool *operator *== *( *const.*const"  &> /dev/null  ; then
  echo "Directory operator==(const...)const not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h | awk '/class *Directory *([^;]|$)/,/};/' | grep " *bool *operator *< *( *const.*const"  &> /dev/null  ; then
  echo "Directory operator<(const...)const not found in directory.h" >> designResults.txt
fi

if ! grep -v "//" directory.h |  awk '/class *Directory *([^;]|$)/,/};/' |grep " *bool *operator *> *( *const.*const"  &> /dev/null  ; then
  echo "Directory operator>(const...)const not found in directory.h" >> designResults.txt
fi



echo $designError  # Only standard out

