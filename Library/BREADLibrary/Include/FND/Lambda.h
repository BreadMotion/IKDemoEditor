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

//�g������
//�E auto f �ȃ����_���̒��ł̎��g�̊֐��Ƃ��Ĉ���
//�E (10)�ċA���čs�������ϐ�������
//�E �m��������Ȃ��Ă����̃����_���̃N���X������m�����̍�邾���ŉ\
// ��񐬌����Ă邩��ł���͂�
//auto result = FixPoint{ [](auto f, int n) -> int {
//	return n < 2 ? n : (f(n - 1) + f(n - 2));
//  } }(10);