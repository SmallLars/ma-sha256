trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 1; i++))
   do
      { time ../cnf_gen/cryptominisat/cryptominisat4 --input ./miter.dimacs --input ./equal_${var}.dimacs --verb 0 >> miter_eval_${var}_${i}.txt ; } 2>> miter_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
done

