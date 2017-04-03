trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   cbmc/cbmc --slice-formula --32 -DCBMC -DBITS=${var} --dimacs --outfile fixed_${var}.dimacs sha256_eval.c >> /dev/null 2>> /dev/null
   echo "Dimacs für $var Bits generiert."
   for ((i = 0; i < 5; i++))
   do
      { time ../knf_gen/cryptominisat/cryptominisat4 --input ./fixed_${var}.dimacs --verb 0 >> sha256_eval_${var}_${i}.txt ; } 2>> sha256_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
   rm fixed_${var}.dimacs
done
