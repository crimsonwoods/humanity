/**
 * @file memory.hpp
 */

#ifndef HUMANITY_MEMORY_H
#define HUMANITY_MEMORY_H

#include <humanity/humanity.hpp>
#include <humanity/utils.hpp>
#include <new>

HUMANITY_NS_BEGIN

/**
 * 不完全型のチェック機能付きdelete
 */
template <typename T_> struct checked_delete {
	void operator()(T_ *ptr) {
    	typedef char type_must_be_complete[ sizeof(T_)? 1: -1 ];
    	(void) sizeof(type_must_be_complete);
    	delete ptr;
	}
};

template <typename T_> struct checked_delete<T_[]> {
	void operator()(T_ *ptr) {
		typedef char type_must_be_complete[ sizeof(T_)? 1: -1 ];
    	(void) sizeof(type_must_be_complete);
		delete[] ptr;
	}
};

/**
 * デフォルトのメモリ解放用関数オブジェクト
 */
template <typename T_> struct default_delete {
	void operator () (T_ *ptr) const {
		if (ptr) checked_delete<T_>()(ptr);
	}
};

/**
 * ポインタを安全に扱うためのテンプレートクラス。<br/>
 * 基本的にはstd::auto_ptrと一緒だが、non_copyableになっている。
 */
template <typename T_> class auto_ptr : private non_copyable< auto_ptr<T_> > {
private:
    mutable T_ *ptr_;
public:
    auto_ptr() : ptr_(NULL) {
    }
    /**
     * ポインタを指定して構築するコンストラクタ
     * @param ptr 管理対象となるポインタ
     */
    explicit auto_ptr(T_ *ptr) : ptr_(ptr) {
    }
    ~auto_ptr() {
        checked_delete<T_>(ptr_);
    }
    /** 等値比較演算子の実装 */
    friend bool operator == (T_ const * const l, auto_ptr<T_> const &r) {
        return l == r.ptr_;
    }
    /** 等値比較演算子の実装 */
    friend bool operator != (T_ const * const l, auto_ptr<T_> const &r) {
        return l != r.ptr_;
    }
    /** 等値比較演算子の実装 */
    bool operator == (T_ const * const r) const {
        return ptr_ == r;
    }
    /** 等値比較演算子の実装 */
    bool operator != (T_ const * const r) const {
        return ptr_ != r;
    }
    /** 等値比較演算子の実装 */
    bool operator == (auto_ptr<T_> const &r) const {
        return ptr_ == r.ptr_;
    }
    /** 等値比較演算子の実装 */
    bool operator != (auto_ptr<T_> const &r) const {
        return ptr_ != r.ptr_;
    }
    /** NOT演算子の実装 */
    bool operator !() const {
        return !ptr_;
    }
    /** アロー演算子の実装 */
    T_ const *operator ->() const {
        return ptr_;
    }
    /** アロー演算子の実装 */
    T_ *operator ->() {
        return ptr_;
    }
    /** *演算子の実装 */
    T_ const &operator *() const {
        return *ptr_;
    }
    /** *演算子の実装 */
    T_ &operator *() {
        return *ptr_;
    }
    /** 所持しているポインタを取得する。 */
    T_ *get() {
        return ptr_;
    }
    /** 所持しているポインタを取得する。 */
    T_ const *get() const {
        return ptr_;
    }
    /** 所持しているポインタを放棄する。 */
    T_ *release() const {
        T_ *ret = ptr_;
        ptr_ = NULL;
        return ret;
    }
    /** 新しいポインタを設定する。<br/>古いポインタは自動的に解放される。 */
    void reset(T_ *ptr = NULL) {
        if (ptr_) {
            delete ptr_;
        }
        ptr_ = ptr;
    }
};

/**
 * C++11の std::unique_ptr の模倣
 */
template <typename T_, typename Del_ = default_delete<T_> > class unique_ptr : private non_copyable< unique_ptr<T_> >{
public:
	typedef T_* pointer;
	typedef T_ const* const_pointer;
	typedef T_& reference;
	typedef T_ const& const_reference;
	typedef Del_ deleter_type;

	unique_ptr() : ptr_(NULL), deleter_() {}
	explicit unique_ptr(pointer ptr) : ptr_(ptr), deleter_() {}
	/**
	 * 削除のための任意のオブジェクトを指定して構築するコンストラクタ
	 * @param deleter リソースを削除するためのオブジェクト・関数ポインタなど
	 */
	unique_ptr(pointer ptr, Del_ deleter) : ptr_(ptr), deleter_(deleter) {}
	~unique_ptr() {}
    /** !演算子の実装 */
    bool operator !() const {
        return !ptr_;
    }
    /** アロー演算子の実装 */
    pointer operator->() {
        return ptr_;
    }
    /** アロー演算子の実装 */
    const_pointer operator->() const {
        return ptr_;
    }
	/** *単項演算子の実装 */
	reference operator*() {
		return *ptr_;
	}
	/** *単項演算子の実装 */
	const_reference operator*() const {
		return *ptr_;
	}
    /** 所持しているリソースを取得する。 */
    pointer get() {
        return ptr_;
    }
    /** 所持しているリソースを取得する。 */
    const_pointer get() const {
        return ptr_;
    }
    /** 所持しているリソースを放棄する。 */
    pointer release() const {
        return ptr_;
    }
    /** 新しいリソースを設定する。<br/>古いリソースは自動的に解放される。 */
    void reset(pointer ptr = pointer()) {
		deleter_(ptr_);
        ptr_ = ptr;
    }
	/** 削除子を取得する。 */
	deleter_type &get_deleter() {
		return deleter_;
	}
	/** 削除子を取得する。 */
	deleter_type const &get_deleter() const {
		return deleter_;
	}
private:
	mutable T_ *ptr_;
	Del_ deleter_;
};

/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator == (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() == rhs.get();
}
/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator != (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() != rhs.get();
}
/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator < (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() < rhs.get();
}
/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator <= (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() <= rhs.get();
}
/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator > (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() > rhs.get();
}
/** 等値比較演算子の実装 */
template <typename T_, typename D1_, typename D2_> bool operator >= (unique_ptr<T_, D1_> const &lhs, unique_ptr<T_, D2_> const &rhs) {
	return lhs.get() >= rhs.get();
}


HUMANITY_NS_END

#endif // end of HUMANITY_MEMORY_H

