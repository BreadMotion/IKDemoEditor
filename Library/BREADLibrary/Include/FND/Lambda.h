#include <utility>

namespace Bread
{
	namespace FND
	{
		template <class F>
		class [[nodiscard]] Lambda final : private F
		{
		public:
		  template <class G>
		  explicit constexpr Lambda(G && g) noexcept : F{std::forward<G>(g)} {}

		  template <class... Args>
		  constexpr decltype(auto) operator()(Args&&... args) const
			noexcept(noexcept(F::operator()(std::declval<Lambda>(), std::declval<Args>()...)))
		  {
			return F::operator()(*this, std::forward<Args>(args)...);
		  }
		};

		template <class F>
		Lambda(F&&)->Lambda<std::decay_t<F>>;
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