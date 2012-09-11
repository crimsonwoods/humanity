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
template <typename T_> void checked_delete(T_ *ptr) {
    typedef char type_must_be_complete[ sizeof(T_)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete ptr;
}

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
 * 任意のリソースを安全に使うためのテンプレートクラス。<br/>
 * 任意の型のdeleterをconstructorで指定できるようになっている。
 */
template <typename T_> class auto_res : private non_copyable< auto_res<T_> >{
private:
    class holder_base_ {
    protected:
        mutable T_ *ptr_;

    public:
        holder_base_(T_ *ptr) : ptr_(ptr) {}
        virtual ~holder_base_() {}

        T_ *get() {
            return ptr_;
        }
        T_ const *get() const {
            return ptr_;
        }
        virtual void reset(T_ *ptr) {}
        T_ *release() const {
            T_ *ret = ptr_;
            ptr_ = NULL;
            return ret;
        }
    };
    template <typename Del_> class holder_ : public holder_base_ {
    private:
        Del_ deleter_;
    public:
        holder_(T_ *ptr, Del_ deleter) : holder_base_(ptr), deleter_(deleter) {}
        ~holder_() {
            if (holder_base_::ptr_) {
                deleter_(holder_base_::ptr_);
            }
        }
        void reset(T_ *ptr) {
            if (holder_base_::ptr_) {
                deleter_(holder_base_::ptr_);
            }
            holder_base_::ptr_ = ptr;
        }
    };
    auto_ptr<holder_base_> holder;

public:
    auto_res() : holder(new holder_<void (*)(T_*)>(NULL, checked_delete<T_>)) {
    }
    /**
     * 削除のための任意のオブジェクトを指定して構築するコンストラクタ
     * @param deleter リソースを削除するためのオブジェクト・関数ポインタなど
     */
    template <typename Del_> explicit auto_res(Del_ deleter) : holder(new holder_<Del_>(NULL, deleter)) {
    }
    /**
     * リソースオブジェクトとその削除のための任意のオブジェクトを指定して構築するコンストラクタ
     * @param ptr リソース
     * @param deleter リソースを削除するためのオブジェクト・関数ポインタなど
     */
    template <typename Del_> auto_res(T_* ptr, Del_ deleter) : holder(new holder_<Del_>(ptr, deleter)) {
    }
    ~auto_res() {
    }
    /** 等値比較演算子の実装 */
    friend bool operator == (T_ const * const l, auto_res<T_> const &r) {
        return l == r.holder->get();
    }
    /** 等値比較演算子の実装 */
    friend bool operator != (T_ const * const l, auto_res<T_> const &r) {
        return l != r.holder->get();
    }
    /** 等値比較演算子の実装 */
    bool operator != (T_ const * const r) const {
        return holder->get() != r;
    }
    /** 等値比較演算子の実装 */
    bool operator == (T_ const * const r) const {
        return holder->get() == r;
    }
    /** 等値比較演算子の実装 */
    bool operator != (auto_res<T_> const &r) const {
        return holder->get() != r.holder->get();
    }
    /** 等値比較演算子の実装 */
    bool operator == (auto_res<T_> const &r) const {
        return holder->get() == r.holder->get();
    }
    /** !演算子の実装 */
    bool operator !() const {
        return !holder->get();
    }
    /** アロー演算子の実装 */
    T_ *operator->() {
        return holder->get();
    }
    /** アロー演算子の実装 */
    T_ const *operator->() const {
        return holder->get();
    }
    /** 所持しているリソースを取得する。 */
    T_ *get() {
        return holder->get();
    }
    /** 所持しているリソースを取得する。 */
    T_ const *get() const {
        return holder->get();
    }
    /** 所持しているリソースを放棄する。 */
    T_ *release() const {
        return holder->release();
    }
    /** 新しいリソースを設定する。<br/>古いリソースは自動的に解放される。 */
    void reset(T_ *ptr = NULL) {
        holder->reset(ptr);
    }
};

HUMANITY_NS_END

#endif // end of HUMANITY_MEMORY_H

