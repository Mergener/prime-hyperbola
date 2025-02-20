
#include <iostream>
#include <chrono>
#include <functional>
#include <cstdint>

/**
 * Our integer type, int64 by default.
 * We could replace this with an integer type from a BigInt library, for instance.
 */
using Integer = std::int64_t;

/**
 * Returns the integer square root of a given integer x.
 */
static Integer isqrt(Integer x) {
    if (x <= 1) {
        return x;
    }

    Integer x0 = x / 2;
    Integer x1 = (x0 + x / x0) / 2;

    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + x / x0) / 2;
    }

    return x0;
}

/**
 * Returns floor(x / d) if floor(x / d) is odd.
 * Otherwise, returns floor(x / d) - 1.
 */
static Integer floor_star(Integer x,
                          Integer d) {
    Integer res = x / d;
    if (res % 2 == 0) {
        res -= 1;
    }
    return res;
}

/**
 * Returns x / d if x / d is an odd integer.
 * Otherwise, returns the nearest odd integer greater than x / d.
 */
static Integer ceil_star(Integer x,
                         Integer d) {
    if (x % d > 0) {
        Integer next = x / d + 1;
        if (next % 2 == 0) {
            next++;
        }
        return next;
    }
    return x / d;
}

/**
 * Computes the set B.
 */
static void compute_B(Integer m,
                      Integer n,
                      std::vector<bool>& out) {
    Integer last_i = isqrt(m);

    auto term = [=](Integer i, Integer j) {
        return i * (ceil_star(m, i) + j);
    };

    for (Integer i = 3; i <= last_i; i += 2) {
        Integer last_j = floor_star(n, i) - ceil_star(m, i);

        for (Integer j = 0; j <= last_j; j += 2) {
            Integer x = term(i, j);

            out[(x - m) / 2] = true;
        }
    }
}

/**
 * Computes the set C.
 */
static void compute_C(Integer m,
                      Integer n,
                      std::vector<bool>& out) {
    Integer last_i = isqrt(n) - isqrt(m);
    Integer sfsm   = isqrt(m);

    auto term = [=](Integer i, Integer j) {
        return (sfsm + i) * (sfsm + i + j);
    };

    for (Integer i = 2; i <= last_i; i += 2) {
        Integer last_j = floor_star(n, sfsm + i) - sfsm - i;

        for (Integer j = 0; j <= last_j; j += 2) {
            Integer x = term(i, j);

            out[(x - m) / 2] = true;
        }
    }
}

/**
 * Runs our prime finding algorithm.
 * Each found prime number is passed to the handler() callback.
 */
void find_primes(Integer m,
                 Integer n,
                 std::function<void(Integer)> handler) {
    if (n < 2) {
        // No prime numbers to be found if upper bound is less than two.
        return;
    }
    if (m <= 2) {
        // Explicitly handle 2, as our algorithm only covers 3 and beyond.
        handler(2);
        m = 3;
    }
    if (m % 2 == 0) {
        // If m is even, it is not a prime. Count
        // from the next integer.
        m++;
    }
    if (n % 2 == 0) {
        // If n is even, it is not a prime. Count
        // up to the previous integer.
        n--;
    }
    if (m > n) {
        // Invalid bounds.
        return;
    }

    // We need to compute set S. We'll define it as a vector of booleans, in which
    // the presence of an element X exists if S[X] == true.
    // Also, (n - m) / 2 + 1 is the maximum number of elements for S.
    std::vector<bool> S((n - m) / 2 + 1);

    // Compute both sets B and C. Since S is the union of the two, we can use
    // the same data structure to hold both.
    compute_B(m, n, S);
    compute_C(m, n, S);

    // Our primes will consist of all odd numbers between m and n (set A) that do
    // not exist in set S, thus consisting of the set A - S.
    for (Integer i = m; i <= n; i += 2) {
        if (!S[(i - m) / 2]) {
            handler(i);
        }
    }
}

int main() {
    // When not using stdio, the following line might make
    // outputting prime numbers slightly faster.
    std::ios_base::sync_with_stdio(false);

    std::vector<Integer> primes;
    while (true) {
        Integer m, n;

        // Read user inputs.
        std::cout << "Enter m:\n";
        std::cin >> m;
        std::cout << "Enter n:\n";
        std::cin >> n;

        // Prepare our prime numbers vector.
        // We are reserving a comfortable upper bound size
        // for our primes vector, but smaller values can
        // certainly be used here.
        primes.clear();
        primes.reserve((n - m) / 2 + 1);

        // Run the algorithm for the given inputs and record the execution time.
        auto before = std::chrono::high_resolution_clock::now();
        find_primes(m, n, [&](Integer i) {
            primes.push_back(i);
        });
        auto after = std::chrono::high_resolution_clock::now();
        double diff = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count());

        // Compute some stats and show our results.
        Integer last_prime = 0;
        Integer largest_distance = 0;
        Integer largest_distance_prime_a = 0;
        Integer largest_distance_prime_b = 0;
        Integer prime_distance_sum = 0;

        std::cout << "Primes between [" << m << ", " << n << "]: ";
        for (size_t i = 0; i < primes.size(); ++i) {
            Integer prime = primes[i];
            if (i > 0) {
                std::cout << ", " << prime;

                // Compute the distance between this prime and the last prime.
                Integer distance = prime - last_prime;

                // Add to the sum. To be used later when calculating the average.
                prime_distance_sum += distance;

                if (distance > largest_distance) {
                    // We found the largest distance between two primes within the interval.
                    // Save it.
                    largest_distance_prime_a = last_prime;
                    largest_distance_prime_b = prime;
                    largest_distance = distance;
                }
            }
            else {
                std::cout << prime;
            }

            last_prime = prime;
        }
        std::cout << "\n\n";
        std::cout << "Prime numbers found:             " << primes.size() << '\n';
        std::cout << "Algorithm run time:              " << diff / 1000000.0 << "ms\n";
        std::cout << "Average distance between primes: " << double(prime_distance_sum) / double(primes.size()) << '\n';
        std::cout << "Largest distance between primes"
                  << " (between " << largest_distance_prime_a << " and " << largest_distance_prime_b << "): "
                  << largest_distance << '\n';
        std::cout << std::endl;
    }
}
