/*
- nodeflow -
BSD 3-Clause License

Copyright (c) 2022, Ruwen Kohm
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*
Implementation of Expected.hpp based on:
	expected - An implementation of std::expected with extensions
	Written in 2017 by Sy Brand (tartanllama@gmail.com, @TartanLlama)

	Documentation available at http://tl.tartanllama.xyz/

	To the extent possible under law, the author(s) have dedicated all
	copyright and related and neighboring rights to this software to the
	public domain worldwide. This software is distributed without any warranty.

	You should have received a copy of the CC0 Public Domain Dedication
	along with this software. If not, see
	<http://creativecommons.org/publicdomain/zero/1.0/>.

	Creative Commons Legal Code

		CC0 1.0 Universal

			CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
			LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
			ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
			INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
			REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
			PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
			THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
			HEREUNDER.

		Statement of Purpose

		The laws of most jurisdictions throughout the world automatically confer
		exclusive Copyright and Related Rights (defined below) upon the creator
		and subsequent owner(s) (each and all, an "owner") of an original work of
		authorship and/or a database (each, a "Work").

		Certain owners wish to permanently relinquish those rights to a Work for
		the purpose of contributing to a commons of creative, cultural and
		scientific works ("Commons") that the public can reliably and without fear
		of later claims of infringement build upon, modify, incorporate in other
		works, reuse and redistribute as freely as possible in any form whatsoever
		and for any purposes, including without limitation commercial purposes.
		These owners may contribute to the Commons to promote the ideal of a free
		culture and the further production of creative, cultural and scientific
		works, or to gain reputation or greater distribution for their Work in
		part through the use and efforts of others.

		For these and/or other purposes and motivations, and without any
		expectation of additional consideration or compensation, the person
		associating CC0 with a Work (the "Affirmer"), to the extent that he or she
		is an owner of Copyright and Related Rights in the Work, voluntarily
		elects to apply CC0 to the Work and publicly distribute the Work under its
		terms, with knowledge of his or her Copyright and Related Rights in the
		Work and the meaning and intended legal effect of CC0 on those rights.

		1. Copyright and Related Rights. A Work made available under CC0 may be
		protected by copyright and related or neighboring rights ("Copyright and
		Related Rights"). Copyright and Related Rights include, but are not
		limited to, the following:

		  i. the right to reproduce, adapt, distribute, perform, display,
			 communicate, and translate a Work;
		 ii. moral rights retained by the original author(s) and/or performer(s);
		iii. publicity and privacy rights pertaining to a person's image or
			 likeness depicted in a Work;
		 iv. rights protecting against unfair competition in regards to a Work,
			 subject to the limitations in paragraph 4(a), below;
		  v. rights protecting the extraction, dissemination, use and reuse of data
			 in a Work;
		 vi. database rights (such as those arising under Directive 96/9/EC of the
			 European Parliament and of the Council of 11 March 1996 on the legal
			 protection of databases, and under any national implementation
			 thereof, including any amended or successor version of such
			 directive); and
		vii. other similar, equivalent or corresponding rights throughout the
			 world based on applicable law or treaty, and any national
			 implementations thereof.

		2. Waiver. To the greatest extent permitted by, but not in contravention
		of, applicable law, Affirmer hereby overtly, fully, permanently,
		irrevocably and unconditionally waives, abandons, and surrenders all of
		Affirmer's Copyright and Related Rights and associated claims and causes
		of action, whether now known or unknown (including existing as well as
		future claims and causes of action), in the Work (i) in all territories
		worldwide, (ii) for the maximum duration provided by applicable law or
		treaty (including future time extensions), (iii) in any current or future
		medium and for any number of copies, and (iv) for any purpose whatsoever,
		including without limitation commercial, advertising or promotional
		purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
		member of the public at large and to the detriment of Affirmer's heirs and
		successors, fully intending that such Waiver shall not be subject to
		revocation, rescission, cancellation, termination, or any other legal or
		equitable action to disrupt the quiet enjoyment of the Work by the public
		as contemplated by Affirmer's express Statement of Purpose.

		3. Public License Fallback. Should any part of the Waiver for any reason
		be judged legally invalid or ineffective under applicable law, then the
		Waiver shall be preserved to the maximum extent permitted taking into
		account Affirmer's express Statement of Purpose. In addition, to the
		extent the Waiver is so judged Affirmer hereby grants to each affected
		person a royalty-free, non transferable, non sublicensable, non exclusive,
		irrevocable and unconditional license to exercise Affirmer's Copyright and
		Related Rights in the Work (i) in all territories worldwide, (ii) for the
		maximum duration provided by applicable law or treaty (including future
		time extensions), (iii) in any current or future medium and for any number
		of copies, and (iv) for any purpose whatsoever, including without
		limitation commercial, advertising or promotional purposes (the
		"License"). The License shall be deemed effective as of the date CC0 was
		applied by Affirmer to the Work. Should any part of the License for any
		reason be judged legally invalid or ineffective under applicable law, such
		partial invalidity or ineffectiveness shall not invalidate the remainder
		of the License, and in such case Affirmer hereby affirms that he or she
		will not (i) exercise any of his or her remaining Copyright and Related
		Rights in the Work or (ii) assert any associated claims and causes of
		action with respect to the Work, in either case contrary to Affirmer's
		express Statement of Purpose.

		4. Limitations and Disclaimers.

		 a. No trademark or patent rights held by Affirmer are waived, abandoned,
			surrendered, licensed or otherwise affected by this document.
		 b. Affirmer offers the Work as-is and makes no representations or
			warranties of any kind concerning the Work, express, implied,
			statutory or otherwise, including without limitation warranties of
			title, merchantability, fitness for a particular purpose, non
			infringement, or the absence of latent or other defects, accuracy, or
			the present or absence of errors, whether or not discoverable, all to
			the greatest extent permissible under applicable law.
		 c. Affirmer disclaims responsibility for clearing rights of other persons
			that may apply to the Work or any use thereof, including without
			limitation any person's Copyright and Related Rights in the Work.
			Further, Affirmer disclaims responsibility for obtaining any necessary
			consents, permissions or other rights required for any use of the
			Work.
		 d. Affirmer understands and acknowledges that Creative Commons is not a
			party to this document and has no duty or obligation with respect to
			this CC0 or use of the Work.
*/

/* Modifications to above include changes to namespace and names */

#pragma once

#ifndef TL_EXPECTED_HPP
#define TL_EXPECTED_HPP

#define TL_EXPECTED_VERSION_MAJOR 1
#define TL_EXPECTED_VERSION_MINOR 0
#define TL_EXPECTED_VERSION_PATCH 1

#include <exception>
#include <functional>
#include <type_traits>
#include <utility>
#include <format>

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define TL_EXPECTED_EXCEPTIONS_ENABLED
#endif

#if (defined(_MSC_VER) && _MSC_VER == 1900)
#define TL_EXPECTED_MSVC2015
#define TL_EXPECTED_MSVC2015_CONSTEXPR
#else
#define TL_EXPECTED_MSVC2015_CONSTEXPR constexpr
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC49
#endif

#if (defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ <= 4 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC54
#endif

#if (defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ <= 5 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC55
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9 &&              \
     !defined(__clang__))
// GCC < 5 doesn't support overloading on const&& for member functions

#define TL_EXPECTED_NO_CONSTRR
// GCC < 5 doesn't support some standard C++11 type traits
#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                         \
  std::has_trivial_copy_constructor<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                            \
  std::has_trivial_copy_assign<T>

// This one will be different for GCC 5.7 if it's ever supported
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)                               \
  std::is_trivially_destructible<T>

// GCC 5 < v < 8 has a bug in is_trivially_copy_constructible which breaks
// std::vector for non-copyable types
#elif (defined(__GNUC__) && __GNUC__ < 8 && !defined(__clang__))
#ifndef TL_GCC_LESS_8_TRIVIALLY_COPY_CONSTRUCTIBLE_MUTEX
#define TL_GCC_LESS_8_TRIVIALLY_COPY_CONSTRUCTIBLE_MUTEX
namespace nf {
	namespace detail {
		template <class T>
		struct is_trivially_copy_constructible
			: std::is_trivially_copy_constructible<T> {};
#ifdef _GLIBCXX_VECTOR
		template <class T, class A>
		struct is_trivially_copy_constructible<std::vector<T, A>> : std::false_type {};
#endif
	} // namespace detail
} // namespace tl
#endif

#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                         \
  tl::detail::is_trivially_copy_constructible<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                            \
  std::is_trivially_copy_assignable<T>
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)                               \
  std::is_trivially_destructible<T>
#else
#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                         \
  std::is_trivially_copy_constructible<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                            \
  std::is_trivially_copy_assignable<T>
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)                               \
  std::is_trivially_destructible<T>
#endif

#if __cplusplus > 201103L
#define TL_EXPECTED_CXX14
#endif

#ifdef TL_EXPECTED_GCC49
#define TL_EXPECTED_GCC49_CONSTEXPR
#else
#define TL_EXPECTED_GCC49_CONSTEXPR constexpr
#endif

#if (__cplusplus == 201103L || defined(TL_EXPECTED_MSVC2015) ||                \
     defined(TL_EXPECTED_GCC49))
#define TL_EXPECTED_11_CONSTEXPR
#else
#define TL_EXPECTED_11_CONSTEXPR constexpr
#endif

namespace nf {
	template <class T, class E> class Expected;

#ifndef TL_MONOSTATE_INPLACE_MUTEX
#define TL_MONOSTATE_INPLACE_MUTEX
	class monostate {};

	struct in_place_t {
		explicit in_place_t() = default;
	};
	static constexpr in_place_t in_place{};
#endif

	template <class E> class Unexpected {
	public:
		static_assert(!std::is_same<E, void>::value, "E must not be void");

		Unexpected() = delete;
		constexpr explicit Unexpected(const E& e) : m_val(e) {}

		constexpr explicit Unexpected(E&& e) : m_val(std::move(e)) {}

		template <class... Args, typename std::enable_if<std::is_constructible<
			E, Args &&...>::value>::type* = nullptr>
		constexpr explicit Unexpected(Args &&...args)
			: m_val(std::forward<Args>(args)...) {}
		template <
			class U, class... Args,
			typename std::enable_if<std::is_constructible<
			E, std::initializer_list<U>&, Args &&...>::value>::type* = nullptr>
		constexpr explicit Unexpected(std::initializer_list<U> l, Args &&...args)
			: m_val(l, std::forward<Args>(args)...) {}

		constexpr const E& value() const& { return m_val; }
		TL_EXPECTED_11_CONSTEXPR E& value()& { return m_val; }
		TL_EXPECTED_11_CONSTEXPR E&& value()&& { return std::move(m_val); }
		constexpr const E&& value() const&& { return std::move(m_val); }

	private:
		E m_val;
	};

#ifdef __cpp_deduction_guides
	template <class E> Unexpected(E) -> Unexpected<E>;
#endif

	template <class E>
	constexpr bool operator==(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() == rhs.value();
	}
	template <class E>
	constexpr bool operator!=(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() != rhs.value();
	}
	template <class E>
	constexpr bool operator<(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() < rhs.value();
	}
	template <class E>
	constexpr bool operator<=(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() <= rhs.value();
	}
	template <class E>
	constexpr bool operator>(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() > rhs.value();
	}
	template <class E>
	constexpr bool operator>=(const Unexpected<E>& lhs, const Unexpected<E>& rhs) {
		return lhs.value() >= rhs.value();
	}

	template <class E>
	Unexpected<typename std::decay<E>::type> make_unexpected(E&& e) {
		return Unexpected<typename std::decay<E>::type>(std::forward<E>(e));
	}

	struct unexpect_t {
		unexpect_t() = default;
	};
	static constexpr unexpect_t unexpect{};

	namespace detail {
		template <typename E>
		[[noreturn]] TL_EXPECTED_11_CONSTEXPR void throw_exception(E&& e) {
#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
			throw std::forward<E>(e);
#else
#ifdef _MSC_VER
			__assume(0);
#else
			__builtin_unreachable();
#endif
#endif
		}

#ifndef TL_TRAITS_MUTEX
#define TL_TRAITS_MUTEX
		// C++14-style aliases for brevity
		template <class T> using remove_const_t = typename std::remove_const<T>::type;
		template <class T>
		using remove_reference_t = typename std::remove_reference<T>::type;
		template <class T> using decay_t = typename std::decay<T>::type;
		template <bool E, class T = void>
		using enable_if_t = typename std::enable_if<E, T>::type;
		template <bool B, class T, class F>
		using conditional_t = typename std::conditional<B, T, F>::type;

		// std::conjunction from C++17
		template <class...> struct conjunction : std::true_type {};
		template <class B> struct conjunction<B> : B {};
		template <class B, class... Bs>
		struct conjunction<B, Bs...>
			: std::conditional<bool(B::value), conjunction<Bs...>, B>::type {};

#if defined(_LIBCPP_VERSION) && __cplusplus == 201103L
#define TL_TRAITS_LIBCXX_MEM_FN_WORKAROUND
#endif

		// In C++11 mode, there's an issue in libc++'s std::mem_fn
		// which results in a hard-error when using it in a noexcept expression
		// in some cases. This is a check to workaround the common failing case.
#ifdef TL_TRAITS_LIBCXX_MEM_FN_WORKAROUND
		template <class T>
		struct is_pointer_to_non_const_member_func : std::false_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...)>
			: std::true_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...)&>
			: std::true_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...)&&>
			: std::true_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...) volatile>
			: std::true_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...) volatile&>
			: std::true_type {};
		template <class T, class Ret, class... Args>
		struct is_pointer_to_non_const_member_func<Ret(T::*)(Args...) volatile&&>
			: std::true_type {};

		template <class T> struct is_const_or_const_ref : std::false_type {};
		template <class T> struct is_const_or_const_ref<T const&> : std::true_type {};
		template <class T> struct is_const_or_const_ref<T const> : std::true_type {};
#endif

		// std::invoke from C++17
		// https://stackoverflow.com/questions/38288042/c11-14-invoke-workaround
		template <
			typename Fn, typename... Args,
#ifdef TL_TRAITS_LIBCXX_MEM_FN_WORKAROUND
			typename = enable_if_t<!(is_pointer_to_non_const_member_func<Fn>::value&&
				is_const_or_const_ref<Args...>::value)>,
#endif
			typename = enable_if_t<std::is_member_pointer<decay_t<Fn>>::value>, int = 0>
		constexpr auto invoke(Fn&& f, Args &&...args) noexcept(
			noexcept(std::mem_fn(f)(std::forward<Args>(args)...)))
			-> decltype(std::mem_fn(f)(std::forward<Args>(args)...)) {
			return std::mem_fn(f)(std::forward<Args>(args)...);
		}

		template <typename Fn, typename... Args,
			typename = enable_if_t<!std::is_member_pointer<decay_t<Fn>>::value>>
			constexpr auto invoke(Fn&& f, Args &&...args) noexcept(
				noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
			-> decltype(std::forward<Fn>(f)(std::forward<Args>(args)...)) {
			return std::forward<Fn>(f)(std::forward<Args>(args)...);
		}

		// std::invoke_result from C++17
		template <class F, class, class... Us> struct invoke_result_impl;

		template <class F, class... Us>
		struct invoke_result_impl<
			F,
			decltype(detail::invoke(std::declval<F>(), std::declval<Us>()...), void()),
			Us...> {
			using type =
				decltype(detail::invoke(std::declval<F>(), std::declval<Us>()...));
		};

		template <class F, class... Us>
		using invoke_result = invoke_result_impl<F, void, Us...>;

		template <class F, class... Us>
		using invoke_result_t = typename invoke_result<F, Us...>::type;

#if defined(_MSC_VER) && _MSC_VER <= 1900
		// TODO make a version which works with MSVC 2015
		template <class T, class U = T> struct is_swappable : std::true_type {};

		template <class T, class U = T> struct is_nothrow_swappable : std::true_type {};
#else
		// https://stackoverflow.com/questions/26744589/what-is-a-proper-way-to-implement-is-swappable-to-test-for-the-swappable-concept
		namespace swap_adl_tests {
			// if swap ADL finds this then it would call std::swap otherwise (same
			// signature)
			struct tag {};

			template <class T> tag swap(T&, T&);
			template <class T, std::size_t N> tag swap(T(&a)[N], T(&b)[N]);

			// helper functions to test if an unqualified swap is possible, and if it
			// becomes std::swap
			template <class, class> std::false_type can_swap(...) noexcept(false);
			template <class T, class U,
				class = decltype(swap(std::declval<T&>(), std::declval<U&>()))>
			std::true_type can_swap(int) noexcept(noexcept(swap(std::declval<T&>(),
				std::declval<U&>())));

			template <class, class> std::false_type uses_std(...);
			template <class T, class U>
			std::is_same<decltype(swap(std::declval<T&>(), std::declval<U&>())), tag>
				uses_std(int);

			template <class T>
			struct is_std_swap_noexcept
				: std::integral_constant<bool,
				std::is_nothrow_move_constructible<T>::value&&
				std::is_nothrow_move_assignable<T>::value> {};

			template <class T, std::size_t N>
			struct is_std_swap_noexcept<T[N]> : is_std_swap_noexcept<T> {};

			template <class T, class U>
			struct is_adl_swap_noexcept
				: std::integral_constant<bool, noexcept(can_swap<T, U>(0))> {};
		} // namespace swap_adl_tests

		template <class T, class U = T>
		struct is_swappable
			: std::integral_constant<
			bool,
			decltype(detail::swap_adl_tests::can_swap<T, U>(0))::value &&
			(!decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value ||
				(std::is_move_assignable<T>::value &&
					std::is_move_constructible<T>::value))> {};

		template <class T, std::size_t N>
		struct is_swappable<T[N], T[N]>
			: std::integral_constant<
			bool,
			decltype(detail::swap_adl_tests::can_swap<T[N], T[N]>(0))::value &&
			(!decltype(detail::swap_adl_tests::uses_std<T[N], T[N]>(
				0))::value ||
				is_swappable<T, T>::value)> {};

		template <class T, class U = T>
		struct is_nothrow_swappable
			: std::integral_constant<
			bool,
			is_swappable<T, U>::value &&
			((decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value&&
				detail::swap_adl_tests::is_std_swap_noexcept<T>::value) ||
				(!decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value &&
					detail::swap_adl_tests::is_adl_swap_noexcept<T, U>::value))> {};
#endif
#endif

		// Trait for checking if a type is a tl::expected
		template <class T> struct is_expected_impl : std::false_type {};
		template <class T, class E>
		struct is_expected_impl<Expected<T, E>> : std::true_type {};
		template <class T> using is_expected = is_expected_impl<decay_t<T>>;

		template <class T, class E, class U>
		using expected_enable_forward_value = detail::enable_if_t<
			std::is_constructible<T, U&&>::value &&
			!std::is_same<detail::decay_t<U>, in_place_t>::value &&
			!std::is_same<Expected<T, E>, detail::decay_t<U>>::value &&
			!std::is_same<Unexpected<E>, detail::decay_t<U>>::value>;

		template <class T, class E, class U, class G, class UR, class GR>
		using expected_enable_from_other = detail::enable_if_t<
			std::is_constructible<T, UR>::value&&
			std::is_constructible<E, GR>::value &&
			!std::is_constructible<T, Expected<U, G>&>::value &&
			!std::is_constructible<T, Expected<U, G>&&>::value &&
			!std::is_constructible<T, const Expected<U, G>&>::value &&
			!std::is_constructible<T, const Expected<U, G>&&>::value &&
			!std::is_convertible<Expected<U, G>&, T>::value &&
			!std::is_convertible<Expected<U, G>&&, T>::value &&
			!std::is_convertible<const Expected<U, G>&, T>::value &&
			!std::is_convertible<const Expected<U, G>&&, T>::value>;

		template <class T, class U>
		using is_void_or = conditional_t<std::is_void<T>::value, std::true_type, U>;

		template <class T>
		using is_copy_constructible_or_void =
			is_void_or<T, std::is_copy_constructible<T>>;

		template <class T>
		using is_move_constructible_or_void =
			is_void_or<T, std::is_move_constructible<T>>;

		template <class T>
		using is_copy_assignable_or_void = is_void_or<T, std::is_copy_assignable<T>>;

		template <class T>
		using is_move_assignable_or_void = is_void_or<T, std::is_move_assignable<T>>;

	} // namespace detail

	namespace detail {
		struct no_init_t {};
		static constexpr no_init_t no_init{};

		// Implements the storage of the values, and ensures that the destructor is
		// trivial if it can be.
		//
		// This specialization is for where neither `T` or `E` is trivially
		// destructible, so the destructors must be called on destruction of the
		// `expected`
		template <class T, class E, bool = std::is_trivially_destructible<T>::value,
			bool = std::is_trivially_destructible<E>::value>
		struct expected_storage_base {
			constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
			constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
				nullptr>
			constexpr expected_storage_base(in_place_t, Args &&...args)
				: m_val(std::forward<Args>(args)...), m_has_val(true) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
				Args &&...args)
				: m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() {
				if (m_has_val) {
					m_val.~T();
				}
				else {
					m_unexpect.~Unexpected<E>();
				}
			}
			union {
				T m_val;
				Unexpected<E> m_unexpect;
				char m_no_init;
			};
			bool m_has_val;
		};

		// This specialization is for when both `T` and `E` are trivially-destructible,
		// so the destructor of the `expected` can be trivial.
		template <class T, class E> struct expected_storage_base<T, E, true, true> {
			constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
			constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
				nullptr>
			constexpr expected_storage_base(in_place_t, Args &&...args)
				: m_val(std::forward<Args>(args)...), m_has_val(true) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
				Args &&...args)
				: m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() = default;
			union {
				T m_val;
				Unexpected<E> m_unexpect;
				char m_no_init;
			};
			bool m_has_val;
		};

		// T is trivial, E is not.
		template <class T, class E> struct expected_storage_base<T, E, true, false> {
			constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
			TL_EXPECTED_MSVC2015_CONSTEXPR expected_storage_base(no_init_t)
				: m_no_init(), m_has_val(false) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
				nullptr>
			constexpr expected_storage_base(in_place_t, Args &&...args)
				: m_val(std::forward<Args>(args)...), m_has_val(true) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
				Args &&...args)
				: m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() {
				if (!m_has_val) {
					m_unexpect.~Unexpected<E>();
				}
			}

			union {
				T m_val;
				Unexpected<E> m_unexpect;
				char m_no_init;
			};
			bool m_has_val;
		};

		// E is trivial, T is not.
		template <class T, class E> struct expected_storage_base<T, E, false, true> {
			constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
			constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
				nullptr>
			constexpr expected_storage_base(in_place_t, Args &&...args)
				: m_val(std::forward<Args>(args)...), m_has_val(true) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
				Args &&...args)
				: m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() {
				if (m_has_val) {
					m_val.~T();
				}
			}
			union {
				T m_val;
				Unexpected<E> m_unexpect;
				char m_no_init;
			};
			bool m_has_val;
		};

		// `T` is `void`, `E` is trivially-destructible
		template <class E> struct expected_storage_base<void, E, false, true> {
#if __GNUC__ <= 5
			//no constexpr for GCC 4/5 bug
#else
			TL_EXPECTED_MSVC2015_CONSTEXPR
#endif 
				expected_storage_base() : m_has_val(true) {}

			constexpr expected_storage_base(no_init_t) : m_val(), m_has_val(false) {}

			constexpr expected_storage_base(in_place_t) : m_has_val(true) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() = default;
			struct dummy {};
			union {
				Unexpected<E> m_unexpect;
				dummy m_val;
			};
			bool m_has_val;
		};

		// `T` is `void`, `E` is not trivially-destructible
		template <class E> struct expected_storage_base<void, E, false, false> {
			constexpr expected_storage_base() : m_dummy(), m_has_val(true) {}
			constexpr expected_storage_base(no_init_t) : m_dummy(), m_has_val(false) {}

			constexpr expected_storage_base(in_place_t) : m_dummy(), m_has_val(true) {}

			template <class... Args,
				detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
				nullptr>
			constexpr explicit expected_storage_base(unexpect_t, Args &&...args)
				: m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

			template <class U, class... Args,
				detail::enable_if_t<std::is_constructible<
				E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
			constexpr explicit expected_storage_base(unexpect_t,
				std::initializer_list<U> il,
				Args &&...args)
				: m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

			~expected_storage_base() {
				if (!m_has_val) {
					m_unexpect.~Unexpected<E>();
				}
			}

			union {
				Unexpected<E> m_unexpect;
				char m_dummy;
			};
			bool m_has_val;
		};

		// This base class provides some handy member functions which can be used in
		// further derived classes
		template <class T, class E>
		struct expected_operations_base : expected_storage_base<T, E> {
			using expected_storage_base<T, E>::expected_storage_base;

			template <class... Args> void construct(Args &&...args) noexcept {
				new (std::addressof(this->m_val)) T(std::forward<Args>(args)...);
				this->m_has_val = true;
			}

			template <class Rhs> void construct_with(Rhs&& rhs) noexcept {
				new (std::addressof(this->m_val)) T(std::forward<Rhs>(rhs).get());
				this->m_has_val = true;
			}

			template <class... Args> void construct_error(Args &&...args) noexcept {
				new (std::addressof(this->m_unexpect))
					Unexpected<E>(std::forward<Args>(args)...);
				this->m_has_val = false;
			}

#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED

			// These assign overloads ensure that the most efficient assignment
			// implementation is used while maintaining the strong exception guarantee.
			// The problematic case is where rhs has a value, but *this does not.
			//
			// This overload handles the case where we can just copy-construct `T`
			// directly into place without throwing.
			template <class U = T,
				detail::enable_if_t<std::is_nothrow_copy_constructible<U>::value>
				* = nullptr>
			void assign(const expected_operations_base& rhs) noexcept {
				if (!this->m_has_val && rhs.m_has_val) {
					geterr().~Unexpected<E>();
					construct(rhs.get());
				}
				else {
					assign_common(rhs);
				}
			}

			// This overload handles the case where we can attempt to create a copy of
			// `T`, then no-throw move it into place if the copy was successful.
			template <class U = T,
				detail::enable_if_t<!std::is_nothrow_copy_constructible<U>::value&&
				std::is_nothrow_move_constructible<U>::value>
				* = nullptr>
			void assign(const expected_operations_base& rhs) noexcept {
				if (!this->m_has_val && rhs.m_has_val) {
					T tmp = rhs.get();
					geterr().~Unexpected<E>();
					construct(std::move(tmp));
				}
				else {
					assign_common(rhs);
				}
			}

			// This overload is the worst-case, where we have to move-construct the
			// unexpected value into temporary storage, then try to copy the T into place.
			// If the construction succeeds, then everything is fine, but if it throws,
			// then we move the old unexpected value back into place before rethrowing the
			// exception.
			template <class U = T,
				detail::enable_if_t<!std::is_nothrow_copy_constructible<U>::value &&
				!std::is_nothrow_move_constructible<U>::value>
				* = nullptr>
			void assign(const expected_operations_base& rhs) {
				if (!this->m_has_val && rhs.m_has_val) {
					auto tmp = std::move(geterr());
					geterr().~Unexpected<E>();

#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
					try {
						construct(rhs.get());
					}
					catch (...) {
						geterr() = std::move(tmp);
						throw;
					}
#else
					construct(rhs.get());
#endif
				}
				else {
					assign_common(rhs);
				}
			}

			// These overloads do the same as above, but for rvalues
			template <class U = T,
				detail::enable_if_t<std::is_nothrow_move_constructible<U>::value>
				* = nullptr>
			void assign(expected_operations_base&& rhs) noexcept {
				if (!this->m_has_val && rhs.m_has_val) {
					geterr().~Unexpected<E>();
					construct(std::move(rhs).get());
				}
				else {
					assign_common(std::move(rhs));
				}
			}

			template <class U = T,
				detail::enable_if_t<!std::is_nothrow_move_constructible<U>::value>
				* = nullptr>
			void assign(expected_operations_base&& rhs) {
				if (!this->m_has_val && rhs.m_has_val) {
					auto tmp = std::move(geterr());
					geterr().~Unexpected<E>();
#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
					try {
						construct(std::move(rhs).get());
					}
					catch (...) {
						geterr() = std::move(tmp);
						throw;
					}
#else
					construct(std::move(rhs).get());
#endif
				}
				else {
					assign_common(std::move(rhs));
				}
			}

#else

			// If exceptions are disabled then we can just copy-construct
			void assign(const expected_operations_base& rhs) noexcept {
				if (!this->m_has_val && rhs.m_has_val) {
					geterr().~Unexpected<E>();
					construct(rhs.get());
				}
				else {
					assign_common(rhs);
				}
			}

			void assign(expected_operations_base&& rhs) noexcept {
				if (!this->m_has_val && rhs.m_has_val) {
					geterr().~Unexpected<E>();
					construct(std::move(rhs).get());
				}
				else {
					assign_common(rhs);
				}
			}

#endif

			// The common part of move/copy assigning
			template <class Rhs> void assign_common(Rhs&& rhs) {
				if (this->m_has_val) {
					if (rhs.m_has_val) {
						get() = std::forward<Rhs>(rhs).get();
					}
					else {
						destroy_val();
						construct_error(std::forward<Rhs>(rhs).geterr());
					}
				}
				else {
					if (!rhs.m_has_val) {
						geterr() = std::forward<Rhs>(rhs).geterr();
					}
				}
			}

			bool has_value() const { return this->m_has_val; }

			TL_EXPECTED_11_CONSTEXPR T& get()& { return this->m_val; }
			constexpr const T& get() const& { return this->m_val; }
			TL_EXPECTED_11_CONSTEXPR T&& get()&& { return std::move(this->m_val); }
#ifndef TL_EXPECTED_NO_CONSTRR
			constexpr const T&& get() const&& { return std::move(this->m_val); }
#endif

			TL_EXPECTED_11_CONSTEXPR Unexpected<E>& geterr()& {
				return this->m_unexpect;
			}
			constexpr const Unexpected<E>& geterr() const& { return this->m_unexpect; }
			TL_EXPECTED_11_CONSTEXPR Unexpected<E>&& geterr()&& {
				return std::move(this->m_unexpect);
			}
#ifndef TL_EXPECTED_NO_CONSTRR
			constexpr const Unexpected<E>&& geterr() const&& {
				return std::move(this->m_unexpect);
			}
#endif

			TL_EXPECTED_11_CONSTEXPR void destroy_val() { get().~T(); }
		};

		// This base class provides some handy member functions which can be used in
		// further derived classes
		template <class E>
		struct expected_operations_base<void, E> : expected_storage_base<void, E> {
			using expected_storage_base<void, E>::expected_storage_base;

			template <class... Args> void construct() noexcept { this->m_has_val = true; }

			// This function doesn't use its argument, but needs it so that code in
			// levels above this can work independently of whether T is void
			template <class Rhs> void construct_with(Rhs&&) noexcept {
				this->m_has_val = true;
			}

			template <class... Args> void construct_error(Args &&...args) noexcept {
				new (std::addressof(this->m_unexpect))
					Unexpected<E>(std::forward<Args>(args)...);
				this->m_has_val = false;
			}

			template <class Rhs> void assign(Rhs&& rhs) noexcept {
				if (!this->m_has_val) {
					if (rhs.m_has_val) {
						geterr().~Unexpected<E>();
						construct();
					}
					else {
						geterr() = std::forward<Rhs>(rhs).geterr();
					}
				}
				else {
					if (!rhs.m_has_val) {
						construct_error(std::forward<Rhs>(rhs).geterr());
					}
				}
			}

			bool has_value() const { return this->m_has_val; }

			TL_EXPECTED_11_CONSTEXPR Unexpected<E>& geterr()& {
				return this->m_unexpect;
			}
			constexpr const Unexpected<E>& geterr() const& { return this->m_unexpect; }
			TL_EXPECTED_11_CONSTEXPR Unexpected<E>&& geterr()&& {
				return std::move(this->m_unexpect);
			}
#ifndef TL_EXPECTED_NO_CONSTRR
			constexpr const Unexpected<E>&& geterr() const&& {
				return std::move(this->m_unexpect);
			}
#endif

			TL_EXPECTED_11_CONSTEXPR void destroy_val() {
				// no-op
			}
		};

		// This class manages conditionally having a trivial copy constructor
		// This specialization is for when T and E are trivially copy constructible
		template <class T, class E,
			bool = is_void_or<T, TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)>::
			value&& TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(E)::value>
		struct expected_copy_base : expected_operations_base<T, E> {
			using expected_operations_base<T, E>::expected_operations_base;
		};

		// This specialization is for when T or E are not trivially copy constructible
		template <class T, class E>
		struct expected_copy_base<T, E, false> : expected_operations_base<T, E> {
			using expected_operations_base<T, E>::expected_operations_base;

			expected_copy_base() = default;
			expected_copy_base(const expected_copy_base& rhs)
				: expected_operations_base<T, E>(no_init) {
				if (rhs.has_value()) {
					this->construct_with(rhs);
				}
				else {
					this->construct_error(rhs.geterr());
				}
			}

			expected_copy_base(expected_copy_base&& rhs) = default;
			expected_copy_base& operator=(const expected_copy_base& rhs) = default;
			expected_copy_base& operator=(expected_copy_base&& rhs) = default;
		};

		// This class manages conditionally having a trivial move constructor
		// Unfortunately there's no way to achieve this in GCC < 5 AFAIK, since it
		// doesn't implement an analogue to std::is_trivially_move_constructible. We
		// have to make do with a non-trivial move constructor even if T is trivially
		// move constructible
#ifndef TL_EXPECTED_GCC49
		template <class T, class E,
			bool = is_void_or<T, std::is_trivially_move_constructible<T>>::value
			&& std::is_trivially_move_constructible<E>::value>
		struct expected_move_base : expected_copy_base<T, E> {
			using expected_copy_base<T, E>::expected_copy_base;
		};
#else
		template <class T, class E, bool = false> struct expected_move_base;
#endif
		template <class T, class E>
		struct expected_move_base<T, E, false> : expected_copy_base<T, E> {
			using expected_copy_base<T, E>::expected_copy_base;

			expected_move_base() = default;
			expected_move_base(const expected_move_base& rhs) = default;

			expected_move_base(expected_move_base&& rhs) noexcept(
				std::is_nothrow_move_constructible<T>::value)
				: expected_copy_base<T, E>(no_init) {
				if (rhs.has_value()) {
					this->construct_with(std::move(rhs));
				}
				else {
					this->construct_error(std::move(rhs.geterr()));
				}
			}
			expected_move_base& operator=(const expected_move_base& rhs) = default;
			expected_move_base& operator=(expected_move_base&& rhs) = default;
		};

		// This class manages conditionally having a trivial copy assignment operator
		template <class T, class E,
			bool = is_void_or<
			T, conjunction<TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T),
			TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T),
			TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)>>::value
			&& TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(E)::value
			&& TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(E)::value
			&& TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(E)::value>
			struct expected_copy_assign_base : expected_move_base<T, E> {
			using expected_move_base<T, E>::expected_move_base;
		};

		template <class T, class E>
		struct expected_copy_assign_base<T, E, false> : expected_move_base<T, E> {
			using expected_move_base<T, E>::expected_move_base;

			expected_copy_assign_base() = default;
			expected_copy_assign_base(const expected_copy_assign_base& rhs) = default;

			expected_copy_assign_base(expected_copy_assign_base&& rhs) = default;
			expected_copy_assign_base& operator=(const expected_copy_assign_base& rhs) {
				this->assign(rhs);
				return *this;
			}
			expected_copy_assign_base&
				operator=(expected_copy_assign_base&& rhs) = default;
		};

		// This class manages conditionally having a trivial move assignment operator
		// Unfortunately there's no way to achieve this in GCC < 5 AFAIK, since it
		// doesn't implement an analogue to std::is_trivially_move_assignable. We have
		// to make do with a non-trivial move assignment operator even if T is trivially
		// move assignable
#ifndef TL_EXPECTED_GCC49
		template <class T, class E,
			bool =
			is_void_or<T, conjunction<std::is_trivially_destructible<T>,
			std::is_trivially_move_constructible<T>,
			std::is_trivially_move_assignable<T>>>::
			value&& std::is_trivially_destructible<E>::value
			&& std::is_trivially_move_constructible<E>::value
			&& std::is_trivially_move_assignable<E>::value>
			struct expected_move_assign_base : expected_copy_assign_base<T, E> {
			using expected_copy_assign_base<T, E>::expected_copy_assign_base;
		};
#else
		template <class T, class E, bool = false> struct expected_move_assign_base;
#endif

		template <class T, class E>
		struct expected_move_assign_base<T, E, false>
			: expected_copy_assign_base<T, E> {
			using expected_copy_assign_base<T, E>::expected_copy_assign_base;

			expected_move_assign_base() = default;
			expected_move_assign_base(const expected_move_assign_base& rhs) = default;

			expected_move_assign_base(expected_move_assign_base&& rhs) = default;

			expected_move_assign_base&
				operator=(const expected_move_assign_base& rhs) = default;

			expected_move_assign_base&
				operator=(expected_move_assign_base&& rhs) noexcept(
					std::is_nothrow_move_constructible<T>::value
					&& std::is_nothrow_move_assignable<T>::value) {
				this->assign(std::move(rhs));
				return *this;
			}
		};

		// expected_delete_ctor_base will conditionally delete copy and move
		// constructors depending on whether T is copy/move constructible
		template <class T, class E,
			bool EnableCopy = (is_copy_constructible_or_void<T>::value&&
				std::is_copy_constructible<E>::value),
			bool EnableMove = (is_move_constructible_or_void<T>::value&&
				std::is_move_constructible<E>::value)>
		struct expected_delete_ctor_base {
			expected_delete_ctor_base() = default;
			expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
			expected_delete_ctor_base&
				operator=(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base&
				operator=(expected_delete_ctor_base&&) noexcept = default;
		};

		template <class T, class E>
		struct expected_delete_ctor_base<T, E, true, false> {
			expected_delete_ctor_base() = default;
			expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
			expected_delete_ctor_base&
				operator=(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base&
				operator=(expected_delete_ctor_base&&) noexcept = default;
		};

		template <class T, class E>
		struct expected_delete_ctor_base<T, E, false, true> {
			expected_delete_ctor_base() = default;
			expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
			expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
			expected_delete_ctor_base&
				operator=(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base&
				operator=(expected_delete_ctor_base&&) noexcept = default;
		};

		template <class T, class E>
		struct expected_delete_ctor_base<T, E, false, false> {
			expected_delete_ctor_base() = default;
			expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
			expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
			expected_delete_ctor_base&
				operator=(const expected_delete_ctor_base&) = default;
			expected_delete_ctor_base&
				operator=(expected_delete_ctor_base&&) noexcept = default;
		};

		// expected_delete_assign_base will conditionally delete copy and move
		// constructors depending on whether T and E are copy/move constructible +
		// assignable
		template <class T, class E,
			bool EnableCopy = (is_copy_constructible_or_void<T>::value&&
				std::is_copy_constructible<E>::value&&
				is_copy_assignable_or_void<T>::value&&
				std::is_copy_assignable<E>::value),
			bool EnableMove = (is_move_constructible_or_void<T>::value&&
				std::is_move_constructible<E>::value&&
				is_move_assignable_or_void<T>::value&&
				std::is_move_assignable<E>::value)>
		struct expected_delete_assign_base {
			expected_delete_assign_base() = default;
			expected_delete_assign_base(const expected_delete_assign_base&) = default;
			expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
				default;
			expected_delete_assign_base&
				operator=(const expected_delete_assign_base&) = default;
			expected_delete_assign_base&
				operator=(expected_delete_assign_base&&) noexcept = default;
		};

		template <class T, class E>
		struct expected_delete_assign_base<T, E, true, false> {
			expected_delete_assign_base() = default;
			expected_delete_assign_base(const expected_delete_assign_base&) = default;
			expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
				default;
			expected_delete_assign_base&
				operator=(const expected_delete_assign_base&) = default;
			expected_delete_assign_base&
				operator=(expected_delete_assign_base&&) noexcept = delete;
		};

		template <class T, class E>
		struct expected_delete_assign_base<T, E, false, true> {
			expected_delete_assign_base() = default;
			expected_delete_assign_base(const expected_delete_assign_base&) = default;
			expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
				default;
			expected_delete_assign_base&
				operator=(const expected_delete_assign_base&) = delete;
			expected_delete_assign_base&
				operator=(expected_delete_assign_base&&) noexcept = default;
		};

		template <class T, class E>
		struct expected_delete_assign_base<T, E, false, false> {
			expected_delete_assign_base() = default;
			expected_delete_assign_base(const expected_delete_assign_base&) = default;
			expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
				default;
			expected_delete_assign_base&
				operator=(const expected_delete_assign_base&) = delete;
			expected_delete_assign_base&
				operator=(expected_delete_assign_base&&) noexcept = delete;
		};

		// This is needed to be able to construct the expected_default_ctor_base which
		// follows, while still conditionally deleting the default constructor.
		struct default_constructor_tag {
			explicit constexpr default_constructor_tag() = default;
		};

		// expected_default_ctor_base will ensure that expected has a deleted default
		// consturctor if T is not default constructible.
		// This specialization is for when T is default constructible
		template <class T, class E,
			bool Enable =
			std::is_default_constructible<T>::value || std::is_void<T>::value>
		struct expected_default_ctor_base {
			constexpr expected_default_ctor_base() noexcept = default;
			constexpr expected_default_ctor_base(
				expected_default_ctor_base const&) noexcept = default;
			constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept =
				default;
			expected_default_ctor_base&
				operator=(expected_default_ctor_base const&) noexcept = default;
			expected_default_ctor_base&
				operator=(expected_default_ctor_base&&) noexcept = default;

			constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
		};

		// This specialization is for when T is not default constructible
		template <class T, class E> struct expected_default_ctor_base<T, E, false> {
			constexpr expected_default_ctor_base() noexcept = delete;
			constexpr expected_default_ctor_base(
				expected_default_ctor_base const&) noexcept = default;
			constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept =
				default;
			expected_default_ctor_base&
				operator=(expected_default_ctor_base const&) noexcept = default;
			expected_default_ctor_base&
				operator=(expected_default_ctor_base&&) noexcept = default;

			constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
		};
	} // namespace detail

	template <class E> class bad_expected_access : public std::exception {
	public:
		explicit bad_expected_access(E e) : m_val(std::move(e)) {}

		virtual const char* what() const noexcept override {
			return "Bad expected access";
		}

		const E& error() const& { return m_val; }
		E& error()& { return m_val; }
		const E&& error() const&& { return std::move(m_val); }
		E&& error()&& { return std::move(m_val); }

	private:
		E m_val;
	};

	/// An `expected<T, E>` object is an object that contains the storage for
	/// another object and manages the lifetime of this contained object `T`.
	/// Alternatively it could contain the storage for another unexpected object
	/// `E`. The contained object may not be initialized after the expected object
	/// has been initialized, and may not be destroyed before the expected object
	/// has been destroyed. The initialization state of the contained object is
	/// tracked by the expected object.
	template <class T, class E>
	class Expected : private detail::expected_move_assign_base<T, E>,
		private detail::expected_delete_ctor_base<T, E>,
		private detail::expected_delete_assign_base<T, E>,
		private detail::expected_default_ctor_base<T, E> {
		static_assert(!std::is_reference<T>::value, "T must not be a reference");
		static_assert(!std::is_same<T, std::remove_cv<in_place_t>::type>::value,
			"T must not be in_place_t");
		static_assert(!std::is_same<T, std::remove_cv<unexpect_t>::type>::value,
			"T must not be unexpect_t");
		static_assert(
			!std::is_same<T, typename std::remove_cv<Unexpected<E>>::type>::value,
			"T must not be unexpected<E>");
		static_assert(!std::is_reference<E>::value, "E must not be a reference");

		T* valptr() { return std::addressof(this->m_val); }
		const T* valptr() const { return std::addressof(this->m_val); }
		Unexpected<E>* errptr() { return std::addressof(this->m_unexpect); }
		const Unexpected<E>* errptr() const {
			return std::addressof(this->m_unexpect);
		}

		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR U& val() {
			return this->m_val;
		}
		TL_EXPECTED_11_CONSTEXPR Unexpected<E>& err() { return this->m_unexpect; }

		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		constexpr const U& val() const {
			return this->m_val;
		}
		constexpr const Unexpected<E>& err() const { return this->m_unexpect; }

		using impl_base = detail::expected_move_assign_base<T, E>;
		using ctor_base = detail::expected_default_ctor_base<T, E>;

	public:
		typedef T value_type;
		typedef E error_type;
		typedef Unexpected<E> unexpected_type;

#if defined(TL_EXPECTED_CXX14) && !defined(TL_EXPECTED_GCC49) &&               \
    !defined(TL_EXPECTED_GCC54) && !defined(TL_EXPECTED_GCC55)
		template <class F> TL_EXPECTED_11_CONSTEXPR auto and_then(F&& f)& {
			return and_then_impl(*this, std::forward<F>(f));
		}
		template <class F> TL_EXPECTED_11_CONSTEXPR auto and_then(F&& f)&& {
			return and_then_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F> constexpr auto and_then(F&& f) const& {
			return and_then_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F> constexpr auto and_then(F&& f) const&& {
			return and_then_impl(std::move(*this), std::forward<F>(f));
		}
#endif

#else
		template <class F>
		TL_EXPECTED_11_CONSTEXPR auto
			and_then(F&& f) & -> decltype(and_then_impl(std::declval<Expected&>(),
				std::forward<F>(f))) {
			return and_then_impl(*this, std::forward<F>(f));
		}
		template <class F>
		TL_EXPECTED_11_CONSTEXPR auto
			and_then(F&& f) && -> decltype(and_then_impl(std::declval<Expected&&>(),
				std::forward<F>(f))) {
			return and_then_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F>
		constexpr auto and_then(F&& f) const& -> decltype(and_then_impl(
			std::declval<Expected const&>(), std::forward<F>(f))) {
			return and_then_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F>
		constexpr auto and_then(F&& f) const&& -> decltype(and_then_impl(
			std::declval<Expected const&&>(), std::forward<F>(f))) {
			return and_then_impl(std::move(*this), std::forward<F>(f));
		}
#endif
#endif

#if defined(TL_EXPECTED_CXX14) && !defined(TL_EXPECTED_GCC49) &&               \
    !defined(TL_EXPECTED_GCC54) && !defined(TL_EXPECTED_GCC55)
		template <class F> TL_EXPECTED_11_CONSTEXPR auto map(F&& f)& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F> TL_EXPECTED_11_CONSTEXPR auto map(F&& f)&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F> constexpr auto map(F&& f) const& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F> constexpr auto map(F&& f) const&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
#else
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(expected_map_impl(
			std::declval<Expected&>(), std::declval<F&&>()))
			map(F&& f)& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(expected_map_impl(std::declval<Expected>(),
			std::declval<F&&>()))
			map(F&& f)&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F>
		constexpr decltype(expected_map_impl(std::declval<const Expected&>(),
			std::declval<F&&>()))
			map(F&& f) const& {
			return expected_map_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F>
		constexpr decltype(expected_map_impl(std::declval<const Expected&&>(),
			std::declval<F&&>()))
			map(F&& f) const&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
#endif
#endif

#if defined(TL_EXPECTED_CXX14) && !defined(TL_EXPECTED_GCC49) &&               \
    !defined(TL_EXPECTED_GCC54) && !defined(TL_EXPECTED_GCC55)
		template <class F> TL_EXPECTED_11_CONSTEXPR auto transform(F&& f)& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F> TL_EXPECTED_11_CONSTEXPR auto transform(F&& f)&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F> constexpr auto transform(F&& f) const& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F> constexpr auto transform(F&& f) const&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
#else
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(expected_map_impl(
			std::declval<Expected&>(), std::declval<F&&>()))
			transform(F&& f)& {
			return expected_map_impl(*this, std::forward<F>(f));
		}
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(expected_map_impl(std::declval<Expected>(),
			std::declval<F&&>()))
			transform(F&& f)&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F>
		constexpr decltype(expected_map_impl(std::declval<const Expected&>(),
			std::declval<F&&>()))
			transform(F&& f) const& {
			return expected_map_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F>
		constexpr decltype(expected_map_impl(std::declval<const Expected&&>(),
			std::declval<F&&>()))
			transform(F&& f) const&& {
			return expected_map_impl(std::move(*this), std::forward<F>(f));
		}
#endif
#endif

#if defined(TL_EXPECTED_CXX14) && !defined(TL_EXPECTED_GCC49) &&               \
    !defined(TL_EXPECTED_GCC54) && !defined(TL_EXPECTED_GCC55)
		template <class F> TL_EXPECTED_11_CONSTEXPR auto map_error(F&& f)& {
			return map_error_impl(*this, std::forward<F>(f));
		}
		template <class F> TL_EXPECTED_11_CONSTEXPR auto map_error(F&& f)&& {
			return map_error_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F> constexpr auto map_error(F&& f) const& {
			return map_error_impl(*this, std::forward<F>(f));
		}
		template <class F> constexpr auto map_error(F&& f) const&& {
			return map_error_impl(std::move(*this), std::forward<F>(f));
		}
#else
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(map_error_impl(std::declval<Expected&>(),
			std::declval<F&&>()))
			map_error(F&& f)& {
			return map_error_impl(*this, std::forward<F>(f));
		}
		template <class F>
		TL_EXPECTED_11_CONSTEXPR decltype(map_error_impl(std::declval<Expected&&>(),
			std::declval<F&&>()))
			map_error(F&& f)&& {
			return map_error_impl(std::move(*this), std::forward<F>(f));
		}
		template <class F>
		constexpr decltype(map_error_impl(std::declval<const Expected&>(),
			std::declval<F&&>()))
			map_error(F&& f) const& {
			return map_error_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F>
		constexpr decltype(map_error_impl(std::declval<const Expected&&>(),
			std::declval<F&&>()))
			map_error(F&& f) const&& {
			return map_error_impl(std::move(*this), std::forward<F>(f));
		}
#endif
#endif
		template <class F> Expected TL_EXPECTED_11_CONSTEXPR or_else(F&& f)& {
			return or_else_impl(*this, std::forward<F>(f));
		}

		template <class F> Expected TL_EXPECTED_11_CONSTEXPR or_else(F&& f)&& {
			return or_else_impl(std::move(*this), std::forward<F>(f));
		}

		template <class F> Expected constexpr or_else(F&& f) const& {
			return or_else_impl(*this, std::forward<F>(f));
		}

#ifndef TL_EXPECTED_NO_CONSTRR
		template <class F> Expected constexpr or_else(F&& f) const&& {
			return or_else_impl(std::move(*this), std::forward<F>(f));
		}
#endif
		constexpr Expected() = default;
		constexpr Expected(const Expected& rhs) = default;
		constexpr Expected(Expected&& rhs) = default;
		Expected& operator=(const Expected& rhs) = default;
		Expected& operator=(Expected&& rhs) = default;

		template <class... Args,
			detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
			nullptr>
		constexpr Expected(in_place_t, Args &&...args)
			: impl_base(in_place, std::forward<Args>(args)...),
			ctor_base(detail::default_constructor_tag{}) {}

		template <class U, class... Args,
			detail::enable_if_t<std::is_constructible<
			T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
		constexpr Expected(in_place_t, std::initializer_list<U> il, Args &&...args)
			: impl_base(in_place, il, std::forward<Args>(args)...),
			ctor_base(detail::default_constructor_tag{}) {}

		template <class G = E,
			detail::enable_if_t<std::is_constructible<E, const G&>::value>* =
			nullptr,
			detail::enable_if_t<!std::is_convertible<const G&, E>::value>* =
			nullptr>
		explicit constexpr Expected(const Unexpected<G>& e)
			: impl_base(unexpect, e.value()),
			ctor_base(detail::default_constructor_tag{}) {}

		template <
			class G = E,
			detail::enable_if_t<std::is_constructible<E, const G&>::value>* =
			nullptr,
			detail::enable_if_t<std::is_convertible<const G&, E>::value>* = nullptr>
		constexpr Expected(Unexpected<G> const& e)
			: impl_base(unexpect, e.value()),
			ctor_base(detail::default_constructor_tag{}) {}

		template <
			class G = E,
			detail::enable_if_t<std::is_constructible<E, G&&>::value>* = nullptr,
			detail::enable_if_t<!std::is_convertible<G&&, E>::value>* = nullptr>
		explicit constexpr Expected(Unexpected<G>&& e) noexcept(
			std::is_nothrow_constructible<E, G&&>::value)
			: impl_base(unexpect, std::move(e.value())),
			ctor_base(detail::default_constructor_tag{}) {}

		template <
			class G = E,
			detail::enable_if_t<std::is_constructible<E, G&&>::value>* = nullptr,
			detail::enable_if_t<std::is_convertible<G&&, E>::value>* = nullptr>
		constexpr Expected(Unexpected<G>&& e) noexcept(
			std::is_nothrow_constructible<E, G&&>::value)
			: impl_base(unexpect, std::move(e.value())),
			ctor_base(detail::default_constructor_tag{}) {}

		template <class... Args,
			detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
			nullptr>
		constexpr explicit Expected(unexpect_t, Args &&...args)
			: impl_base(unexpect, std::forward<Args>(args)...),
			ctor_base(detail::default_constructor_tag{}) {}

		template <class U, class... Args,
			detail::enable_if_t<std::is_constructible<
			E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
		constexpr explicit Expected(unexpect_t, std::initializer_list<U> il,
			Args &&...args)
			: impl_base(unexpect, il, std::forward<Args>(args)...),
			ctor_base(detail::default_constructor_tag{}) {}

		template <class U, class G,
			detail::enable_if_t<!(std::is_convertible<U const&, T>::value&&
				std::is_convertible<G const&, E>::value)>* =
			nullptr,
			detail::expected_enable_from_other<T, E, U, G, const U&, const G&>
			* = nullptr>
		explicit TL_EXPECTED_11_CONSTEXPR Expected(const Expected<U, G>& rhs)
			: ctor_base(detail::default_constructor_tag{}) {
			if (rhs.has_value()) {
				this->construct(*rhs);
			}
			else {
				this->construct_error(rhs.error());
			}
		}

		template <class U, class G,
			detail::enable_if_t<(std::is_convertible<U const&, T>::value&&
				std::is_convertible<G const&, E>::value)>* =
			nullptr,
			detail::expected_enable_from_other<T, E, U, G, const U&, const G&>
			* = nullptr>
		TL_EXPECTED_11_CONSTEXPR Expected(const Expected<U, G>& rhs)
			: ctor_base(detail::default_constructor_tag{}) {
			if (rhs.has_value()) {
				this->construct(*rhs);
			}
			else {
				this->construct_error(rhs.error());
			}
		}

		template <
			class U, class G,
			detail::enable_if_t<!(std::is_convertible<U&&, T>::value&&
				std::is_convertible<G&&, E>::value)>* = nullptr,
			detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
		explicit TL_EXPECTED_11_CONSTEXPR Expected(Expected<U, G>&& rhs)
			: ctor_base(detail::default_constructor_tag{}) {
			if (rhs.has_value()) {
				this->construct(std::move(*rhs));
			}
			else {
				this->construct_error(std::move(rhs.error()));
			}
		}

		template <
			class U, class G,
			detail::enable_if_t<(std::is_convertible<U&&, T>::value&&
				std::is_convertible<G&&, E>::value)>* = nullptr,
			detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR Expected(Expected<U, G>&& rhs)
			: ctor_base(detail::default_constructor_tag{}) {
			if (rhs.has_value()) {
				this->construct(std::move(*rhs));
			}
			else {
				this->construct_error(std::move(rhs.error()));
			}
		}

		template <
			class U = T,
			detail::enable_if_t<!std::is_convertible<U&&, T>::value>* = nullptr,
			detail::expected_enable_forward_value<T, E, U>* = nullptr>
		explicit TL_EXPECTED_MSVC2015_CONSTEXPR Expected(U&& v)
			: Expected(in_place, std::forward<U>(v)) {}

		template <
			class U = T,
			detail::enable_if_t<std::is_convertible<U&&, T>::value>* = nullptr,
			detail::expected_enable_forward_value<T, E, U>* = nullptr>
		TL_EXPECTED_MSVC2015_CONSTEXPR Expected(U&& v)
			: Expected(in_place, std::forward<U>(v)) {}

		template <
			class U = T, class G = T,
			detail::enable_if_t<std::is_nothrow_constructible<T, U&&>::value>* =
			nullptr,
			detail::enable_if_t<!std::is_void<G>::value>* = nullptr,
			detail::enable_if_t<
			(!std::is_same<Expected<T, E>, detail::decay_t<U>>::value &&
				!detail::conjunction<std::is_scalar<T>,
				std::is_same<T, detail::decay_t<U>>>::value&&
				std::is_constructible<T, U>::value&&
				std::is_assignable<G&, U>::value&&
				std::is_nothrow_move_constructible<E>::value)>* = nullptr>
		Expected& operator=(U&& v) {
			if (has_value()) {
				val() = std::forward<U>(v);
			}
			else {
				err().~Unexpected<E>();
				::new (valptr()) T(std::forward<U>(v));
				this->m_has_val = true;
			}

			return *this;
		}

		template <
			class U = T, class G = T,
			detail::enable_if_t<!std::is_nothrow_constructible<T, U&&>::value>* =
			nullptr,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr,
			detail::enable_if_t<
			(!std::is_same<Expected<T, E>, detail::decay_t<U>>::value &&
				!detail::conjunction<std::is_scalar<T>,
				std::is_same<T, detail::decay_t<U>>>::value&&
				std::is_constructible<T, U>::value&&
				std::is_assignable<G&, U>::value&&
				std::is_nothrow_move_constructible<E>::value)>* = nullptr>
		Expected& operator=(U&& v) {
			if (has_value()) {
				val() = std::forward<U>(v);
			}
			else {
				auto tmp = std::move(err());
				err().~Unexpected<E>();

#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
				try {
					::new (valptr()) T(std::forward<U>(v));
					this->m_has_val = true;
				}
				catch (...) {
					err() = std::move(tmp);
					throw;
				}
#else
				::new (valptr()) T(std::forward<U>(v));
				this->m_has_val = true;
#endif
			}

			return *this;
		}

		template <class G = E,
			detail::enable_if_t<std::is_nothrow_copy_constructible<G>::value&&
			std::is_assignable<G&, G>::value>* = nullptr>
		Expected& operator=(const Unexpected<G>& rhs) {
			if (!has_value()) {
				err() = rhs;
			}
			else {
				this->destroy_val();
				::new (errptr()) Unexpected<E>(rhs);
				this->m_has_val = false;
			}

			return *this;
		}

		template <class G = E,
			detail::enable_if_t<std::is_nothrow_move_constructible<G>::value&&
			std::is_move_assignable<G>::value>* = nullptr>
		Expected& operator=(Unexpected<G>&& rhs) noexcept {
			if (!has_value()) {
				err() = std::move(rhs);
			}
			else {
				this->destroy_val();
				::new (errptr()) Unexpected<E>(std::move(rhs));
				this->m_has_val = false;
			}

			return *this;
		}

		template <class... Args, detail::enable_if_t<std::is_nothrow_constructible<
			T, Args &&...>::value>* = nullptr>
		void emplace(Args &&...args) {
			if (has_value()) {
				val().~T();
			}
			else {
				err().~Unexpected<E>();
				this->m_has_val = true;
			}
			::new (valptr()) T(std::forward<Args>(args)...);
		}

		template <class... Args, detail::enable_if_t<!std::is_nothrow_constructible<
			T, Args &&...>::value>* = nullptr>
		void emplace(Args &&...args) {
			if (has_value()) {
				val().~T();
				::new (valptr()) T(std::forward<Args>(args)...);
			}
			else {
				auto tmp = std::move(err());
				err().~Unexpected<E>();

#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
				try {
					::new (valptr()) T(std::forward<Args>(args)...);
					this->m_has_val = true;
				}
				catch (...) {
					err() = std::move(tmp);
					throw;
				}
#else
				::new (valptr()) T(std::forward<Args>(args)...);
				this->m_has_val = true;
#endif
			}
		}

		template <class U, class... Args,
			detail::enable_if_t<std::is_nothrow_constructible<
			T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
		void emplace(std::initializer_list<U> il, Args &&...args) {
			if (has_value()) {
				T t(il, std::forward<Args>(args)...);
				val() = std::move(t);
			}
			else {
				err().~Unexpected<E>();
				::new (valptr()) T(il, std::forward<Args>(args)...);
				this->m_has_val = true;
			}
		}

		template <class U, class... Args,
			detail::enable_if_t<!std::is_nothrow_constructible<
			T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
		void emplace(std::initializer_list<U> il, Args &&...args) {
			if (has_value()) {
				T t(il, std::forward<Args>(args)...);
				val() = std::move(t);
			}
			else {
				auto tmp = std::move(err());
				err().~Unexpected<E>();

#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
				try {
					::new (valptr()) T(il, std::forward<Args>(args)...);
					this->m_has_val = true;
				}
				catch (...) {
					err() = std::move(tmp);
					throw;
				}
#else
				::new (valptr()) T(il, std::forward<Args>(args)...);
				this->m_has_val = true;
#endif
			}
		}

	private:
		using t_is_void = std::true_type;
		using t_is_not_void = std::false_type;
		using t_is_nothrow_move_constructible = std::true_type;
		using move_constructing_t_can_throw = std::false_type;
		using e_is_nothrow_move_constructible = std::true_type;
		using move_constructing_e_can_throw = std::false_type;

		void swap_where_both_have_value(Expected& /*rhs*/, t_is_void) noexcept {
			// swapping void is a no-op
		}

		void swap_where_both_have_value(Expected& rhs, t_is_not_void) {
			using std::swap;
			swap(val(), rhs.val());
		}

		void swap_where_only_one_has_value(Expected& rhs, t_is_void) noexcept(
			std::is_nothrow_move_constructible<E>::value) {
			::new (errptr()) unexpected_type(std::move(rhs.err()));
			rhs.err().~unexpected_type();
			std::swap(this->m_has_val, rhs.m_has_val);
		}

		void swap_where_only_one_has_value(Expected& rhs, t_is_not_void) {
			swap_where_only_one_has_value_and_t_is_not_void(
				rhs, typename std::is_nothrow_move_constructible<T>::type{},
				typename std::is_nothrow_move_constructible<E>::type{});
		}

		void swap_where_only_one_has_value_and_t_is_not_void(
			Expected& rhs, t_is_nothrow_move_constructible,
			e_is_nothrow_move_constructible) noexcept {
			auto temp = std::move(val());
			val().~T();
			::new (errptr()) unexpected_type(std::move(rhs.err()));
			rhs.err().~unexpected_type();
			::new (rhs.valptr()) T(std::move(temp));
			std::swap(this->m_has_val, rhs.m_has_val);
		}

		void swap_where_only_one_has_value_and_t_is_not_void(
			Expected& rhs, t_is_nothrow_move_constructible,
			move_constructing_e_can_throw) {
			auto temp = std::move(val());
			val().~T();
#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
			try {
				::new (errptr()) unexpected_type(std::move(rhs.err()));
				rhs.err().~unexpected_type();
				::new (rhs.valptr()) T(std::move(temp));
				std::swap(this->m_has_val, rhs.m_has_val);
			}
			catch (...) {
				val() = std::move(temp);
				throw;
			}
#else
			::new (errptr()) unexpected_type(std::move(rhs.err()));
			rhs.err().~unexpected_type();
			::new (rhs.valptr()) T(std::move(temp));
			std::swap(this->m_has_val, rhs.m_has_val);
#endif
		}

		void swap_where_only_one_has_value_and_t_is_not_void(
			Expected& rhs, move_constructing_t_can_throw,
			e_is_nothrow_move_constructible) {
			auto temp = std::move(rhs.err());
			rhs.err().~unexpected_type();
#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
			try {
				::new (rhs.valptr()) T(std::move(val()));
				val().~T();
				::new (errptr()) unexpected_type(std::move(temp));
				std::swap(this->m_has_val, rhs.m_has_val);
			}
			catch (...) {
				rhs.err() = std::move(temp);
				throw;
			}
#else
			::new (rhs.valptr()) T(std::move(val()));
			val().~T();
			::new (errptr()) unexpected_type(std::move(temp));
			std::swap(this->m_has_val, rhs.m_has_val);
#endif
		}

	public:
		template <class OT = T, class OE = E>
		detail::enable_if_t<detail::is_swappable<OT>::value&&
			detail::is_swappable<OE>::value &&
			(std::is_nothrow_move_constructible<OT>::value ||
				std::is_nothrow_move_constructible<OE>::value)>
			swap(Expected& rhs) noexcept(
				std::is_nothrow_move_constructible<T>::value
				&& detail::is_nothrow_swappable<T>::value
				&& std::is_nothrow_move_constructible<E>::value
				&& detail::is_nothrow_swappable<E>::value) {
			if (has_value() && rhs.has_value()) {
				swap_where_both_have_value(rhs, typename std::is_void<T>::type{});
			}
			else if (!has_value() && rhs.has_value()) {
				rhs.swap(*this);
			}
			else if (has_value()) {
				swap_where_only_one_has_value(rhs, typename std::is_void<T>::type{});
			}
			else {
				using std::swap;
				swap(err(), rhs.err());
			}
		}

		constexpr const T* operator->() const { return valptr(); }
		TL_EXPECTED_11_CONSTEXPR T* operator->() { return valptr(); }

		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		constexpr const U& operator*() const& {
			return val();
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR U& operator*()& {
			return val();
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		constexpr const U&& operator*() const&& {
			return std::move(val());
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR U&& operator*()&& {
			return std::move(val());
		}

		constexpr bool has_value() const noexcept { return this->m_has_val; }
		constexpr explicit operator bool() const noexcept { return this->m_has_val; }

		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR const U& value() const& {
			if (!has_value())
				detail::throw_exception(bad_expected_access<E>(err().value()));
			return val();
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR U& value()& {
			if (!has_value())
				detail::throw_exception(bad_expected_access<E>(err().value()));
			return val();
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR const U&& value() const&& {
			if (!has_value())
				detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
			return std::move(val());
		}
		template <class U = T,
			detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
		TL_EXPECTED_11_CONSTEXPR U&& value()&& {
			if (!has_value())
				detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
			return std::move(val());
		}

		constexpr const E& error() const& { return err().value(); }
		TL_EXPECTED_11_CONSTEXPR E& error()& { return err().value(); }
		constexpr const E&& error() const&& { return std::move(err().value()); }
		TL_EXPECTED_11_CONSTEXPR E&& error()&& { return std::move(err().value()); }

		template <class U> constexpr T value_or(U&& v) const& {
			static_assert(std::is_copy_constructible<T>::value &&
				std::is_convertible<U&&, T>::value,
				"T must be copy-constructible and convertible to from U&&");
			return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
		}
		template <class U> TL_EXPECTED_11_CONSTEXPR T value_or(U&& v)&& {
			static_assert(std::is_move_constructible<T>::value &&
				std::is_convertible<U&&, T>::value,
				"T must be move-constructible and convertible to from U&&");
			return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
		}
	};

	template <class T, class E>
	using ExpectedRef = nf::Expected<std::reference_wrapper<T>, E>;

	namespace detail {
		template <class Exp> using exp_t = typename detail::decay_t<Exp>::value_type;
		template <class Exp> using err_t = typename detail::decay_t<Exp>::error_type;
		template <class Exp, class Ret> using ret_t = Expected<Ret, err_t<Exp>>;

#ifdef TL_EXPECTED_CXX14
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>()))>
		constexpr auto and_then_impl(Exp&& exp, F&& f) {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");

			return exp.has_value()
				? detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
				: Ret(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>()))>
		constexpr auto and_then_impl(Exp&& exp, F&& f) {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");

			return exp.has_value() ? detail::invoke(std::forward<F>(f))
				: Ret(unexpect, std::forward<Exp>(exp).error());
		}
#else
		template <class> struct TC;
		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>())),
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr>
		auto and_then_impl(Exp&& exp, F&& f) -> Ret {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");

			return exp.has_value()
				? detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
				: Ret(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>())),
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr>
		constexpr auto and_then_impl(Exp&& exp, F&& f) -> Ret {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");

			return exp.has_value() ? detail::invoke(std::forward<F>(f))
				: Ret(unexpect, std::forward<Exp>(exp).error());
		}
#endif

#ifdef TL_EXPECTED_CXX14
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto expected_map_impl(Exp&& exp, F&& f) {
			using result = ret_t<Exp, detail::decay_t<Ret>>;
			return exp.has_value() ? result(detail::invoke(std::forward<F>(f),
				*std::forward<Exp>(exp)))
				: result(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto expected_map_impl(Exp&& exp, F&& f) {
			using result = Expected<void, err_t<Exp>>;
			if (exp.has_value()) {
				detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
				return result();
			}

			return result(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto expected_map_impl(Exp&& exp, F&& f) {
			using result = ret_t<Exp, detail::decay_t<Ret>>;
			return exp.has_value() ? result(detail::invoke(std::forward<F>(f)))
				: result(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto expected_map_impl(Exp&& exp, F&& f) {
			using result = Expected<void, err_t<Exp>>;
			if (exp.has_value()) {
				detail::invoke(std::forward<F>(f));
				return result();
			}

			return result(unexpect, std::forward<Exp>(exp).error());
		}
#else
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>

		constexpr auto expected_map_impl(Exp&& exp, F&& f)
			-> ret_t<Exp, detail::decay_t<Ret>> {
			using result = ret_t<Exp, detail::decay_t<Ret>>;

			return exp.has_value() ? result(detail::invoke(std::forward<F>(f),
				*std::forward<Exp>(exp)))
				: result(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				*std::declval<Exp>())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>

		auto expected_map_impl(Exp&& exp, F&& f) -> Expected<void, err_t<Exp>> {
			if (exp.has_value()) {
				detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
				return {};
			}

			return Unexpected<err_t<Exp>>(std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>

		constexpr auto expected_map_impl(Exp&& exp, F&& f)
			-> ret_t<Exp, detail::decay_t<Ret>> {
			using result = ret_t<Exp, detail::decay_t<Ret>>;

			return exp.has_value() ? result(detail::invoke(std::forward<F>(f)))
				: result(unexpect, std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>

		auto expected_map_impl(Exp&& exp, F&& f) -> Expected<void, err_t<Exp>> {
			if (exp.has_value()) {
				detail::invoke(std::forward<F>(f));
				return {};
			}

			return Unexpected<err_t<Exp>>(std::forward<Exp>(exp).error());
		}
#endif

#if defined(TL_EXPECTED_CXX14) && !defined(TL_EXPECTED_GCC49) &&               \
    !defined(TL_EXPECTED_GCC54) && !defined(TL_EXPECTED_GCC55)
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto map_error_impl(Exp&& exp, F&& f) {
			using result = Expected<exp_t<Exp>, detail::decay_t<Ret>>;
			return exp.has_value()
				? result(*std::forward<Exp>(exp))
				: result(unexpect, detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()));
		}
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto map_error_impl(Exp&& exp, F&& f) {
			using result = Expected<exp_t<Exp>, monostate>;
			if (exp.has_value()) {
				return result(*std::forward<Exp>(exp));
			}

			detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
			return result(unexpect, monostate{});
		}
		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto map_error_impl(Exp&& exp, F&& f) {
			using result = Expected<exp_t<Exp>, detail::decay_t<Ret>>;
			return exp.has_value()
				? result()
				: result(unexpect, detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()));
		}
		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto map_error_impl(Exp&& exp, F&& f) {
			using result = Expected<exp_t<Exp>, monostate>;
			if (exp.has_value()) {
				return result();
			}

			detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
			return result(unexpect, monostate{});
		}
#else
		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto map_error_impl(Exp&& exp, F&& f)
			-> Expected<exp_t<Exp>, detail::decay_t<Ret>> {
			using result = Expected<exp_t<Exp>, detail::decay_t<Ret>>;

			return exp.has_value()
				? result(*std::forward<Exp>(exp))
				: result(unexpect, detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()));
		}

		template <class Exp, class F,
			detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto map_error_impl(Exp&& exp, F&& f) -> Expected<exp_t<Exp>, monostate> {
			using result = Expected<exp_t<Exp>, monostate>;
			if (exp.has_value()) {
				return result(*std::forward<Exp>(exp));
			}

			detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
			return result(unexpect, monostate{});
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto map_error_impl(Exp&& exp, F&& f)
			-> Expected<exp_t<Exp>, detail::decay_t<Ret>> {
			using result = Expected<exp_t<Exp>, detail::decay_t<Ret>>;

			return exp.has_value()
				? result()
				: result(unexpect, detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()));
		}

		template <class Exp, class F,
			detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		auto map_error_impl(Exp&& exp, F&& f) -> Expected<exp_t<Exp>, monostate> {
			using result = Expected<exp_t<Exp>, monostate>;
			if (exp.has_value()) {
				return result();
			}

			detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
			return result(unexpect, monostate{});
		}
#endif

#ifdef TL_EXPECTED_CXX14
		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		constexpr auto or_else_impl(Exp&& exp, F&& f) {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");
			return exp.has_value() ? std::forward<Exp>(exp)
				: detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		detail::decay_t<Exp> or_else_impl(Exp&& exp, F&& f) {
			return exp.has_value() ? std::forward<Exp>(exp)
				: (detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()),
					std::forward<Exp>(exp));
		}
#else
		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
		auto or_else_impl(Exp&& exp, F&& f) -> Ret {
			static_assert(detail::is_expected<Ret>::value, "F must return an expected");
			return exp.has_value() ? std::forward<Exp>(exp)
				: detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error());
		}

		template <class Exp, class F,
			class Ret = decltype(detail::invoke(std::declval<F>(),
				std::declval<Exp>().error())),
			detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
		detail::decay_t<Exp> or_else_impl(Exp&& exp, F&& f) {
			return exp.has_value() ? std::forward<Exp>(exp)
				: (detail::invoke(std::forward<F>(f),
					std::forward<Exp>(exp).error()),
					std::forward<Exp>(exp));
		}
#endif
	} // namespace detail

	template <class T, class E, class U, class F>
	constexpr bool operator==(const Expected<T, E>& lhs,
		const Expected<U, F>& rhs) {
		return (lhs.has_value() != rhs.has_value())
			? false
			: (!lhs.has_value() ? lhs.error() == rhs.error() : *lhs == *rhs);
	}
	template <class T, class E, class U, class F>
	constexpr bool operator!=(const Expected<T, E>& lhs,
		const Expected<U, F>& rhs) {
		return (lhs.has_value() != rhs.has_value())
			? true
			: (!lhs.has_value() ? lhs.error() != rhs.error() : *lhs != *rhs);
	}
	template <class E, class F>
	constexpr bool operator==(const Expected<void, E>& lhs,
		const Expected<void, F>& rhs) {
		return (lhs.has_value() != rhs.has_value())
			? false
			: (!lhs.has_value() ? lhs.error() == rhs.error() : true);
	}
	template <class E, class F>
	constexpr bool operator!=(const Expected<void, E>& lhs,
		const Expected<void, F>& rhs) {
		return (lhs.has_value() != rhs.has_value())
			? true
			: (!lhs.has_value() ? lhs.error() == rhs.error() : false);
	}

	template <class T, class E, class U>
	constexpr bool operator==(const Expected<T, E>& x, const U& v) {
		return x.has_value() ? *x == v : false;
	}
	template <class T, class E, class U>
	constexpr bool operator==(const U& v, const Expected<T, E>& x) {
		return x.has_value() ? *x == v : false;
	}
	template <class T, class E, class U>
	constexpr bool operator!=(const Expected<T, E>& x, const U& v) {
		return x.has_value() ? *x != v : true;
	}
	template <class T, class E, class U>
	constexpr bool operator!=(const U& v, const Expected<T, E>& x) {
		return x.has_value() ? *x != v : true;
	}

	template <class T, class E>
	constexpr bool operator==(const Expected<T, E>& x, const Unexpected<E>& e) {
		return x.has_value() ? false : x.error() == e.value();
	}
	template <class T, class E>
	constexpr bool operator==(const Unexpected<E>& e, const Expected<T, E>& x) {
		return x.has_value() ? false : x.error() == e.value();
	}
	template <class T, class E>
	constexpr bool operator!=(const Expected<T, E>& x, const Unexpected<E>& e) {
		return x.has_value() ? true : x.error() != e.value();
	}
	template <class T, class E>
	constexpr bool operator!=(const Unexpected<E>& e, const Expected<T, E>& x) {
		return x.has_value() ? true : x.error() != e.value();
	}

	template <class T, class E,
		detail::enable_if_t<(std::is_void<T>::value ||
			std::is_move_constructible<T>::value) &&
		detail::is_swappable<T>::value&&
		std::is_move_constructible<E>::value&&
		detail::is_swappable<E>::value>* = nullptr>
	void swap(Expected<T, E>& lhs,
		Expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}
} // namespace tl


template<typename T, typename E, class CharT>
struct std::formatter<nf::Expected<T, E>, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const nf::Expected<T, E>& exp, FormatContext& fc) const
	{
		if (exp)
		{
			if constexpr (std::is_void_v<T>)
				return std::format_to(fc.out(), "{}", "void");
			else
				return std::format_to(fc.out(), "{}", exp.value());
		}
		return std::format_to(fc.out(), "{}", exp.error());
	}
};

#endif
