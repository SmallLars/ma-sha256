trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 5; i++))
   do
      { time ../cnf_gen/cryptominisat/cryptominisat4 --input ./sha256_espresso.dimacs --input ./fixed_${var}.dimacs --verb 0 >> espresso/sha256_eval_${var}_${i}.txt ; } 2>> espresso/sha256_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
done

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 5; i++))
   do
      { time ../cnf_gen/cryptominisat/cryptominisat4 --input ./sha256_tseitin.dimacs --input ./fixed_${var}.dimacs --verb 0 >> tseitin/sha256_eval_${var}_${i}.txt ; } 2>> tseitin/sha256_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
done

