template<int N>
struct fixed_value_type {
  static constexpr auto
  value() -> int {
    return N;
  }
};

class runtime_value_t {
  private:
    int n;
  public:
    constexpr
    runtime_value_t(int n)
      : n(n)
    {}
    constexpr auto
    value() const -> int {
      return n;
    }
};


template<class value_param_type>
class my_class : private value_param_type { // private because implementation inheritance
  public:
    using value_param_type::value; // the trick is here:
    // if value_param_type==fixed_value_type,
    //     then value() is static
    // if value_param_type==runtime_value_t,
    //     then value() is a member function
};

int main() {
  my_class<runtime_value_t> x;
  std::cout << x.value(); // works

  my_class<fixed_value_type<2>> y;
  std::cout << y.value(); // also works: even if value() is static,
             // the member function syntax is possible
  constexpr int val = decltype(y)::value(); // also works
}
