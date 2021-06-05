# !/bin/bash

for name in hprd human yeast
    do
        for typ in n s
            do
                for num in 1 3 5 8
                    do
                        rm -f result_${name}_${typ}${num}.txt
                        ./main/program ../data/lcc_${name}.igraph ../query/lcc_${name}_${typ}${num}.igraph ../candidate_set/lcc_${name}_${typ}${num}.cs >> result_${name}_${typ}${num}.txt
                        echo lcc_${name}_${typ}${num}_test
                        ../check/a.out ../data/lcc_${name}.igraph ../query/lcc_${name}_${typ}${num}.igraph result_${name}_${typ}${num}.txt
                    done
            done
     done
