trap 'exit' 2

for ((var = 9; var <= 22; var++))
do
   for ((i = 0; i < 5; i++))
   do
       ./sha256_eval $var >> sha256_eval.txt
   done
done
