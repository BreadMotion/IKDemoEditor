#include <utility>

namespace Bread
{
	namespace FND
	{
		template <typename F>
		class
#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
			[[nodiscard]]
#endif  // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
		Lambda final : private F
		{
		public:
		  template <typename G>
		  explicit constexpr Lambda(G && g) noexcept
			: F{std::forward<G>(g)} {}

		  template <typename... Args>
		  constexpr decltype(auto)
		  operator()(Args&&... args) const
		#if !defined(__GNUC__) || defined(__clang__) || __GNUC__ >= 9
			noexcept(noexcept(F::operator()(std::declval<Lambda>(), std::declval<Args>()...)))
		#endif  // !defined(__GNUC__) || defined(__clang__) || __GNUC__ >= 9
		  {
			return F::operator()(*this, std::forward<Args>(args)...);
		  }
		}; // class Lambda

#if defined(__cpp_deduction_guides)
		template <typename F>
		Lambda(F&&)
			->Lambda<std::decay_t<F>>;
#endif  // defined(__cpp_deduction_guides)

	}
}

//使い方↓
//・ auto f なラムダ式の中での自身の関数として扱う
//・ (10)再帰して行いたい変数を入れる
//・ 確か代入しなくてもこのラムダ自体クラスだから確か実体作るだけで可能
// 一回成功してるからできるはず
//auto result = FixPoint{ [](auto f, int n) -> int {
//	return n < 2 ? n : (f(n - 1) + f(n - 2));
//  } }(10);