#include <array>
using std::array;

// TODO review

template<class T, int N>
constexpr array<T,N> nullArray() {
    array<T,N> a;
    for (int i=0; i<N; ++i) {
        a[i] = T();
    }
    return a;
}


template<class T, int N>
constexpr array<int,N> oneNonNullEltArray(int i, T val) {
    array<T,N> a = nullArray<T,N>();
    a[i] = val;
    return a;
}







constexpr auto
factorial(int n) -> int {
    if (n=0) {
        return 1;
    } else {
        return n*factorial(n);
    }
}

constexpr auto
combination(int n, int k) -> int {
    return factorial(n)/(factorial(k)*factorial(n-k));
}

constexpr auto
combinationWithRepetition(int n, int k) -> int {
    return combination(n+k-1,k);
}








constexpr auto
numberOfMonomials(int dimension, int degree) -> int {
    return combinationWithRepetition(dimension,degree);
}

/// SEE https://en.wikipedia.org/wiki/Combination#Number_of_combinations_with_repetition
template<int D> constexpr auto
indexOfMonomial(UnitMonomial<D> m) -> int {
    constexpr int N = m.degree();
    if (D==1) {
        return 0;
    } else if (N==0) {
        return 0;
    } else if (N==1) {
        if (m.exponent(0)==1) {
            return 0;
        } else {
            return indexOfMonomial(decrementFirstExponent(m));
        }
    } else {
        if (m.exponent(0)!=0) {
            return indexOfMonomial(decrementFirstExponent(m));
        } else {
            return numberOfMonomials(D,N-1)-1 + indexOfMonomial(removeFirstDirection(m));
        }
    }
}

template<int D> constexpr auto
firstMonomial() -> UnitMonomial<D> {
    return UnitMonomial();
}
template<int D> constexpr auto
lastMonomial(int N) -> UnitMonomial<D> {
    return UnitMonomial(D-1)^N;
}

template<int D> constexpr auto
nextMonomial(UnitMonomial<D> m) -> UnitMonomial<D> {
    static_assert(D>0,"Null dimension polynomial");
    constexpr int N = m.degree();
    if (m.exponent(D-1)==N) {
        return createMonomialNonNullInFirstDirection<D>(N+1);
    } else if (m.exponent(0)==1 && m.exponent(D-1)==N-1) {
        return insertNullFirstDirection(createMonomialWithExponentInFirstDirection<D-1>(N));
    } else {
        return incrementFirstExponent(nextMonomial(decrementFirstExponent(m)));
    }
}


constexpr auto
numberOfCoefficientsForPolynomial(int dimension, int degree) -> int {
    if (degree==0) {
        return 1;
    } else {
        return numberOfMonomials(dimension,degree) + numberOfCoefficientsForPolymomial(dimension,degree-1);
    }
}

template<int D> constexpr auto
indexOfCoefficientInPolynomial(UnitMonomial<D> m) {
    if (m.degree()==0) {
        return 0;
    } else {
        return numberOfCoefficientsForPolynomial(D,m.degree()-1) + indexOfMonomial(m);
    }
}



incrementFirstExponent(UnitMonomial<D> m) -> UnitMonomial<D>
decrementFirstExponent(UnitMonomial<D> m) -> UnitMonomial<D>

removeFirstDirection(UnitMonomial<D> m) -> UnitMonomial<D-1>
createMonomialWithExponentInFirstDirection(int N) -> UnitMonomial<D>


template<int D, int N> auto Polynomial<D,N>::
operator+=(Polynomial<D,N> const& P) -> Polynomial<D,N> {
    return Polynomial<D,M>(coeffs + P.coeffs);
}
template<int D, int N> auto Polynomial<D,N>::
operator-=(Polynomial<D,N> const& P) -> Polynomial<D,N> {
    return Polynomial<D,N>(coeffs + P.coeffs);
}
template<int D, int N> auto Polynomial<D,N>::
operator*=(double lambda) -> Polynomial<D,N> {
    return Polynomial<D,N>(lambda * coeffs);
}
template<int D, int N> auto Polynomial<D,N>::
operator/=(double lambda) -> Polynomial<D,N> {
    return (*this)*(1./lambda);
}

operator+(Polynomial<D,M> const& P, Polynomial<D,N> const& Q) -> Polynomial<D,max(M,N)> {
    Polynomial<D,max(M,N)> P0 = P;
    Polynomial<D,max(M,N)> Q0 = Q;
    Polynomial<D,max(M,N)> res = P0;
    res += Q0;
    return res; 
}
operator-(Polynomial<D,M> const& P, Polynomial<D,N> const& Q) -> Polynomial<D,max(M,N)> {
    return Polynomial<D,max(M,N)>(P.coefficients() - Q.coefficients());
}
operator*(double lambda, Polynomial<D,M> const& P) -> Polynomial<D,M> {
    return Polynomial<D,M>(lambda * P.coefficients());
}
operator*(Polynomial<D,M> const& P, double lambda) -> Polynomial<D,M> {
    return lambda*P;
}
operator/(Polynomial<D,M> const& P, double lambda) -> Polynomial<D,M> {
    return (1./lambda)*P;
}

operator*(Monomial<D,M> const& m, Polynomial<D,N> const& P) -> Polynomial<D,M+N> {
    Polynomial<D,M+N> mP;
    for (auto m_p: P.monomials()) {
        mP += m*m_p;
    }
    return mP;
}

operator*(Polynomial<D,M> const& P, Polynomial<D,N> const& Q) -> Polynomial<D,M+N> {
    Polynomial<D,M+m.degree()> PQ;
    for (auto m_p: P.monomials()) {
        PQ += m_p*Q;
    }
    return mP;
}






class Indeterminate {
    public:
        constexpr Indeterminate(int dir);

        constexpr int direction();
    private:
        const int dir;
};


constexpr Indeterminate::
Indeterminate(int dir)
    : dir(dir) {}

constexpr auto Indeterminate::
direction() -> int {
    return dir;
}

constexpr auto X = Indeterminate(0);
constexpr auto Y = Indeterminate(1);
constexpr auto Z = Indeterminate(2);












template<int D>
class UnitMonomial {
    public:
        static const int Dimension = D;

        constexpr UnitMonomial();
        constexpr UnitMonomial(Indeterminate indet);

        template<int D0>
        constexpr UnitMonomial(UnitMonomial<D0> m);

        constexpr int exponent(Indeterminate indet) const;
        constexpr int degree() const;
    private:
      /// Member functions
        constexpr UnitMonomial(int exponents[D]);
      /// Data members
        int exponents[D];
};



template<int D> constexpr UnitMonomial<D>::
UnitMonomial() 
    : exponents(nullArray<int,D>()) {}

template<int D> constexpr UnitMonomial<D>::
UnitMonomial(Indeterminate indet) 
    : exponents(oneNonNullEltArray<int,D>(indet.direction(),1)) {}

template<int D> constexpr UnitMonomial<D>::
UnitMonomial(UnitMonomial<D0> m) 
    : exponents(resizeAndFillWithNull(m.exponents()))
{
    static_assert(D0<=D, "Implicit conversion to monomial of smaller dimension is forbidden")
}



template<int D> constexpr auto UnitMonomial<D>::
exponent(Indeterminate indet) const -> int {
    return exponents[indet.direction()];
}


template<int D> constexpr auto UnitMonomial<D>::
degree(Indeterminate indet) const -> int {
    return std::accumulate(exponents.begin(),exponents.end(),0);
}



template<int D> auto
operator*(UnitMonomial<D> m0, UnitMonomial<D> m1) -> UnitMonomial<D> {
    int exponents[D];
    for (int i=0; i<D; ++i) {
        exponents[i] = m0.exponent(Indeterminate(i)) + m1.exponent(Indeterminate(i));
    }
    return UnitMonomial<D>(exponents);
}

template<int D> auto
operator^(UnitMonomial<D> m0, int n) -> UnitMonomial<D> {
    int exponents[D];
    for (int i=0; i<D; ++i) {
        exponents[i] = m0.exponent(Indeterminate(i))*n;
    }
    return UnitMonomial<D>(exponents);
}








template<int D, int N>
class Monomial {
    public:
        Monomial(UnitMonomial<D> const& um);
    private:
      /// Member functions
        constexpr Monomial(double coeff, UnitMonomial<D> const& um);
      /// Data members
        double coeff;
        UnitMonomial<D> um;
};


template<int D,int N> Monomial<D,N>::
Monomial(UnitMonomial<D> const& um)
    : coeff(1.)
    , um(um) {}


template<int D,int M,int N> auto
operator*(Monomial<D,M> const& m0, Monomial<D,N> const& m1) -> Monomial<D,M+N> {
    return Monomial<D,M+N>(m0.coeff*m1.coeff,m0.um+m1.um);
}

template<int D,int N> auto
operator*(double lambda, Monomial<D,N> const& m0) -> Monomial<D,N> {
    return Monomial<D,N>(lambda*m0.coeff,m0.um);
}
template<int D,int N> auto
operator*(Monomial<D,N> const& m0, double lambda) -> Monomial<D,N> {
    return lambda*m0;
}
template<int D,int N> auto
operator/(Monomial<D,N> const& m0, double lambda) -> Monomial<D,N> {
    return (1./lambda)*m0;
}













class Polynomial {

    array<
};




