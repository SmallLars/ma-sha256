trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 5; i++))
   do
      { time ./cbmc/cbmc --slice-formula --32 -DCBMC -DBITS=${var} sha256_eval.c >> sha256_eval_${var}_${i}.txt ; } 2>> sha256_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
done
