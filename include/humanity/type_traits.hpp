/**
 * @file type_traits.hpp
 */

#ifndef HUMANITY_TYPE_TRAITS_H
#define HUMANITY_TYPE_TRAITS_H

#include <humanity/humanity.hpp>

HUMANITY_NS_BEGIN

template <typename T, T v_> struct integral_constant {
	typedef T value_type;
	typedef integral_constant<T, v> type;
	static T const value = v_;
	operator value_type() const { return value; }
};

typedef std::integral_constant<bool, true>  true_type;
typedef std::integral_constant<bool, false> false_type;

template <typename T> struct is_void : public false_type {};
template <> struct is_void<void> : public true_type {};

template <typename T> struct is_array : public false_type {};
template <typename T> struct is_array<T[]> : public true_type {};
template <typename T, std::size_t N> struct is_array<T[N]> : public true_type {};

template <typename T> struct is_pointer : public false_type {};
template <typename T> struct is_pointer<T*> : public true_type {};
template <typename T> struct is_pointer<T* const> : public true_type {};
template <typename T> struct is_pointer<T* volatile> : public true_type {};
template <typename T> struct is_pointer<T* const volatile> : public true_type {};

template <typename T> struct is_const : public false_type {};
template <typename T> struct is_const<T const> : public true_type {};

template <typename T> struct is_volatile : public false_type {};
template <typename T> struct is_volatile<T volatile> : public true_type {};

/**
 * C++11のtype_traitsにある std::remove_pointer の模倣
 */
template <typename T> struct remove_pointer {
    /** テンプレートに渡された型 */
    typedef T type;
};

/**
 * remove_pointerの特殊化
 */
template <typename T> struct remove_pointer<T*> {
    /** テンプレートに渡されたポインタ型の元になった型 */
    typedef T type;
};

/**
 * remove_pointerの特殊化
 */
template <typename T> struct remove_pointer<T* const> {
    /** テンプレートに渡されたポインタ型の元になった型 */
	typedef T type;
};

/**
 * remove_pointerの特殊化
 */
template <typename T> struct remove_pointer<T* volatile>  {
    /** テンプレートに渡されたポインタ型の元になった型 */
	typedef T type;
};

/**
 * remove_pointerの特殊化
 */
template <typename T> struct remove_pointer<T* const volatile> {
    /** テンプレートに渡されたポインタ型の元になった型 */
	typedef T type;
};

template <bool B, typename T = void> struct enable_if {};
template <typename T> struct enable_if<true, T> { typedef T type; };

HUMANITY_NS_END

#endif // end of HUMANITY_TYPE_TRAITS_H

