
#include "poly.h"
#include <map>
#include <iostream>
#include <algorithm>

polynomial::polynomial() {
    terms[0] = 0;
}

template polynomial::polynomial(std::vector<std::pair<power, coeff>>::const_iterator, std::vector<std::pair<power, coeff>>::const_iterator);

polynomial::polynomial(const polynomial &other) : terms(other.terms) {}

polynomial &polynomial::operator=(const polynomial &other) {
    if (this != &other) {
        terms = other.terms;
    }
    return *this;
}

void polynomial::print() const {
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        std::cout << it->second << "x^" << it->first << " ";
    }
    std::cout << std::endl;
}

size_t polynomial::find_degree_of() {
    if (terms.empty()) return 0;
    return terms.rbegin()->first;
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> result;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        if (it->second != 0 || (terms.size() == 1 && it->first == 0)) {
            result.emplace_back(it->first, it->second);
        }
    }
    if (result.empty()) result.emplace_back(0, 0);
    return result;
}

const std::map<power, coeff>& polynomial::get_terms() const {
    return terms;
}

std::map<power, coeff>& polynomial::get_terms() {
    return terms;
}

polynomial operator+(const polynomial &a, const polynomial &b) {
    polynomial result = a;
    for (const auto &[p, c] : b.get_terms()) {
        result.get_terms()[p] += c;
    }
    return result;
}

polynomial operator+(const polynomial &a, int b) {
    polynomial result = a;
    result.get_terms()[0] += b;
    return result;
}

polynomial operator+(int a, const polynomial &b) {
    return b + a;
}

polynomial operator*(const polynomial &a, const polynomial &b) {
    polynomial result;
    result.get_terms().clear();
    for (const auto &[pa, ca] : a.get_terms()) {
        for (const auto &[pb, cb] : b.get_terms()) {
            result.get_terms()[pa + pb] += ca * cb;
        }
    }
    if (result.get_terms().empty()) result.get_terms()[0] = 0;
    return result;
}

polynomial operator*(const polynomial &a, int b) {
    polynomial result;
    result.get_terms().clear();
    for (const auto &[p, c] : a.get_terms()) {
        result.get_terms()[p] = c * b;
    }
    if (result.get_terms().empty()) result.get_terms()[0] = 0;
    return result;
}

polynomial operator*(int a, const polynomial &b) {
    return b * a;
}

polynomial operator%(const polynomial &a, const polynomial &b) {
    if (b.get_terms().empty() || (b.get_terms().size() == 1 && b.get_terms().begin()->second == 0)) {
        throw std::invalid_argument("Divide by zero polynomial");
    }

    polynomial remainder = a;
    power deg_b = b.get_terms().rbegin()->first;
    coeff lc_b = b.get_terms().rbegin()->second;

    while (!remainder.get_terms().empty() && remainder.get_terms().rbegin()->first >= deg_b) {
        power deg_diff = remainder.get_terms().rbegin()->first - deg_b;
        coeff lead_coeff_ratio = remainder.get_terms().rbegin()->second / lc_b;

        polynomial subtract_poly;
        for (const auto &[p, c] : b.get_terms()) {
            subtract_poly.get_terms()[p + deg_diff] = c * lead_coeff_ratio;
        }

        for (const auto &[p, c] : subtract_poly.get_terms()) {
            remainder.get_terms()[p] -= c;
            if (remainder.get_terms()[p] == 0) {
                remainder.get_terms().erase(p);
            }
        }
    }

    if (remainder.get_terms().empty()) remainder.get_terms()[0] = 0;
    return remainder;
}