#include <iostream>
#include "Rustic_SAT_Solver/data.hpp"
#include "Rustic_SAT_Solver/solver.hpp"
#include <random>
#include <algorithm>
#include <vector>
#include <cassert>
#include <fstream>

using real = long double;

std::random_device seed_gen;
std::mt19937 engine;
std::uniform_int_distribution<> dist(0, 1);

struct Initializer{ Initializer(){
    engine = std::mt19937{seed_gen()};
}} initializer__;

std::vector<std::string> makeNames(int N) {
    std::vector<std::string> res;
    for(int i=0; i<N; ++i)
        res.push_back("L" + std::to_string(i));
    return res;
};

std::vector<Literal> makeLiterals(std::vector<std::string> names){
    std::vector<Literal> res;
    for(auto&& name : names)
        res.push_back({name, static_cast<bool>(dist(engine))});
    return res;
}

ClauseSet func_d(int k, int N, int M){
    // std::assert(k <= N);
    std::vector<Clause> res;
    auto&& names = makeNames(N);
    while(M --> 0){
        std::vector<std::string> usingNames;
        std::sample(names.begin(), names.end(), std::back_inserter(usingNames), k, engine);
        std::set<Literal> clause;
        for(auto&& literal : makeLiterals(usingNames))
            clause.insert(literal);
        res.push_back({clause});
    }
    return ClauseSet{res};
}

void showClauseSet(const ClauseSet& clauses){
    for(auto&& clause : clauses.clauses){
        for(auto&& literal : clause.literals){
            std::cout << (literal.invert ? "-" : "+") << literal.prop.name << ", ";
        }
        std::cout << std::endl;
    }
}

int main(){

    std::printf("  N | k |  M |  M/N  | rate of SAT \n");
    for(int N : {5, 10, 20}){
        for(int k : {2, 3}){
            std::ofstream data_out("data_" + std::to_string(N) + "_" + std::to_string(k) + ".txt");
            std::printf("--------------------------------------\n");
            for(int M=1; M<25*N; M += N/5){
                int satCnt = 0, unsatCnt = 0;
                for(int i=0; i<10000; ++i){
                    auto clauses = func_d(k, N, M);
                    auto [isSAT, conds] = solve(clauses);
                    if(isSAT)
                        ++satCnt;
                    else
                        ++unsatCnt;
                }
                std::printf(" %2d | %d | %2d | %2.2lf | %1.4lf \n",
                    N, k, M, static_cast<double>(M) / N, static_cast<double>(satCnt) / (satCnt + unsatCnt));
                data_out
                    << static_cast<double>(M) / N
                    << "  "
                    << static_cast<double>(satCnt) / (satCnt + unsatCnt)
                    << std::endl;
                std::fflush(stdout);
            }
        }
    }
}
