#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <map>          // std::map
#include <vector>       // std::vector
#include <numeric>      // std::iota
#include <iterator>     // std::inserter
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

auto findPrimes(int m)
{
    std::vector<int> primes(m-1);
    std::iota(begin(primes), end(primes), 2);
    auto iterEnd = end(primes);
    std::remove_if(begin(primes), iterEnd,
                   [& iterEnd, & primes](int aPrime){
                        iterEnd = std::remove_if(begin(primes), iterEnd,
                                                 [& aPrime](int number){
                                                        return ((number>aPrime) && (number%aPrime == 0));
                                                 });
                        return 0;
                    });
   primes.resize(std::distance(begin(primes), iterEnd));
   return primes;
}

auto findDivisors(std::vector<int> primes, std::vector<int> numbers)
{
    std::map<int,std::vector<int>> map;
    std::vector<int> helperVec;
    std::transform(begin(primes), end(primes),
                   std::inserter(map, begin(map)),
                   [& numbers, & helperVec](int primeNumber) {
                      helperVec.clear();
                      std::copy_if(begin(numbers), end(numbers),
                                   std::back_inserter(helperVec),
                                   [& primeNumber](int numb) {
                                        return (numb % primeNumber == 0 && numb >= primeNumber); 
                                    });
                      return std::make_pair(primeNumber, helperVec);
                   });
    return map;
}

auto getRandomNumbers(int m, int n)
{
    std::vector<int> numbers(m+1);
    std::iota(begin(numbers), end(numbers), 0);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(begin(numbers), end(numbers), std::default_random_engine(seed));
    numbers.resize(n);
    std::sort(begin(numbers), end(numbers));
    return numbers;
}

int main()
{
    int n, m;
    std::cin >> n >> m;
    auto randomNumbers = getRandomNumbers(m, n);
    auto primes = findPrimes(m);
    auto divisors = findDivisors(primes, randomNumbers);
    std::copy(begin(randomNumbers), end(randomNumbers), std::ostream_iterator<int>(std::cout, "  "));
    std::cout << std::endl;
    for (auto mapDivisors : divisors)
    {
        std::cout << "\n" << mapDivisors.first << "  -> ";
        std::copy(begin(mapDivisors.second), end(mapDivisors.second), std::ostream_iterator<int>(std::cout, "  "));
    }
    std::cout << std::endl;
    return 0;
}
