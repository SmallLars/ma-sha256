trap 'exit' 2

for ((var = 9; var <= 18; var++))
do
   for ((i = 0; i < 5; i++))
   do
       ./bitcoin_eval.bin $var >> bitcoin_eval.txt
   done
done
