trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 5; i++))
   do
      { time ../knf_gen/cryptominisat/cryptominisat4 --input ./1_sha256.dimacs --input ./fixed_${var}.dimacs --verb 0 >> sha256_eval_${var}_${i}.txt ; } 2>> sha256_eval_${var}_${i}_t.txt
      echo "Lösung mit $var Bits gefunden. Durchlauf $(($i+1))/5."
   done
done

