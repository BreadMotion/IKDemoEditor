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

//�g������
//�E auto f �ȃ����_���̒��ł̎��g�̊֐��Ƃ��Ĉ���
//�E (10)�ċA���čs�������ϐ�������
//�E �m��������Ȃ��Ă����̃����_���̃N���X������m�����̍�邾���ŉ\
// ��񐬌����Ă邩��ł���͂�
//auto result = FixPoint{ [](auto f, int n) -> int {
//	return n < 2 ? n : (f(n - 1) + f(n - 2));
//  } }(10);