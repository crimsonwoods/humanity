/**
 * @file array.hpp
 */

#ifndef HUMANITY_ARRAY_H
#define HUMANITY_ARRAY_H

#include <humanity/humanity.hpp>
#include <cstdlib>
#include <cstddef>

#ifdef _EXCEPTION
#include <stdexcept>
#endif

HUMANITY_NS_BEGIN

/**
 * 配列を扱うためのテンプレートクラス。<br/>
 * 基本的にはC++11の std::array の模倣。
 */
template <typename T_, std::size_t Size_> class array {
private:
    T_ array_[Size_];

public:
    /** 格納している要素の型 */
    typedef T_ value_type;
    /** サイズの型 */
    typedef std::size_t size_type;
    /** 参照型 */
    typedef value_type& reference;
    /** 変更不可能な参照型 */
    typedef value_type const& const_reference;
    /** ポインタ型 */
    typedef T_* pointer;
    /** 変更不可能なポインタ型 */
    typedef T_ const* const_pointer;

    array() : array_() {}
    /** コピーコンストラクタ */
    array(array const &src) : array_(src.array_) {}
    ~array() {}

    /** 引数で指定した位置の要素を取得する */
    reference at(size_type pos) {
        THROW_IF(pos >= Size_, std::out_of_range, "index out of range");
        return array_[pos];
    }
    /** 引数で指定した位置の要素を取得する */
    const_reference at(size_type pos) const {
        THROW_IF(pos >= Size_, std::out_of_range, "index out of range");
        return array_[pos];
    }

    /** []演算子 */
    reference operator [] (size_type pos) {
        return at(pos);
    }
    /** []演算子 */
    const_reference operator [] (size_type pos) const {
        return at(pos);
    }

    /** 配列の先頭要素を取得する */
    reference front() {
        return array_[0];
    }
    /** 配列の先頭要素を取得する */
    const_reference front() const {
        return array_[0];
    }

    /** 配列の末尾の要素を取得する */
    reference back() {
        return array_[Size_ - 1];
    }
    /** 配列の末尾の要素を取得する */
    const_reference back() const {
        return array_[Size_ - 1];
    }

    /** 配列の先頭要素へのポインタを返す */
    pointer data() {
        return array_;
    }
    /** 配列の先頭要素へのポインタを返す */
    const_pointer data() const {
        return array_;
    }

    /** 空配列かどうかを判定する */
    bool empty() const {
        return 0 == Size_ ? true : false;
    }

    /** 配列の要素数を取得する */
    size_type size() const {
        return Size_;
    }

    /** 配列の要素数の最大値を取得する */
    size_type max_size() const {
        return Size_;
    }
};

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator == (array<T_, N_> const &l, array<T_, N_> const &r) {
    return 0 == std::memcmp(l.data(), r.data(), N_);
}

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator != (array<T_, N_> const &l, array<T_, N_> const &r) {
    return !(l == r);
}

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator < (array<T_, N_> const &l, array<T_, N_> const &r) {
    return std::memcmp(l.data(), r.data(), N_) < 0;
}

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator > (array<T_, N_> const &l, array<T_, N_> const &r) {
    return std::memcmp(l.data(), r.data(), N_) > 0;
}

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator <= (array<T_, N_> const &l, array<T_, N_> const &r) {
    return std::memcmp(l.data(), r.data(), N_) <= 0;
}

/** arrayに対する等値比較演算子 */
template <typename T_, std::size_t N_> bool operator >= (array<T_, N_> const &l, array<T_, N_> const &r) {
    return std::memcmp(l.data(), r.data(), N_) >= 0;
}

HUMANITY_NS_END

#endif // end of HUMANITY_ARRAY_H

