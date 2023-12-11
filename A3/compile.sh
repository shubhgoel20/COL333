mkdir -p exe/
g++ -O3 cnf_parallel.cpp part1_cnfbuild.cpp -o exe/cnf_build
g++ -O3 cnf_parallel_2.cpp part2_cnfbuild.cpp -o exe/cnf_build_2
g++ -O3 part1_resultparse.cpp -o exe/result_parse
g++ -O3 cnf_parallel_2.cpp part2_resultparse.cpp -o exe/result_parse_2