#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include <chrono>

int divisao_impar(int x, int d) {
    int res = x / d;
    if (res % 2 == 0) {
        res -= 1;
    }
    return res;
}

std::vector<int>& gera_impares1(std::vector<int>& v, int n) {
    v.reserve(n * n);

    for (int i = 3; i <= n; i += 2) {
        for (int j = i; j <= n; j += 2) {
            v.push_back(i * j);
        }
    }

    return v;
}

std::vector<int>& gera_impares2(std::vector<int>& v, int n) {
    v.reserve(n * n);

    if (n % 2 == 0) {
        n--;
    }

    int p = divisao_impar(n*n, 3);

    for (int j = 2; j <= p - n; j += 2) {
        for (int i = 3; i <= divisao_impar(n * n, n + j); i += 2) {
            v.push_back(i * (n + j));
        }
    }

    return v;
}

void gera_primos(int n) {
    std::vector<int> v;

    auto before = std::chrono::high_resolution_clock::now();
    gera_impares1(v, n);
    gera_impares2(v, n);
    auto after = std::chrono::high_resolution_clock::now();

    std::set<int> s(v.begin(), v.end());
    size_t primos_gerados = 1;
    std::cout << 2 << '\n';

    int ultimo = 1;
    for (int impar: s) {
        for (int i = ultimo + 2; i < impar; i += 2) {
            std::cout << i << '\n';
            primos_gerados++;
        }
        ultimo = impar;
    }
    std::cout.flush();

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    std::cout << "Tempo total corrido: " << delta.count() << "ms" << std::endl;
    std::cout << "Total de primos encontrados: " << primos_gerados << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    while (true) {
        int n;
        std::cout << "Digite um N: ";
        std::cin >> n;

        gera_primos(std::ceil(std::sqrt(double(n))));
    }

    return 0;
}
