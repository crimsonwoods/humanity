/**
 * 汎用的なユーティリティクラス群をまとめたヘッダファイル
 * @file utils.hpp
 */

#ifndef HUMANITY_UTILS_H
#define HUMANITY_UTILS_H

#include <humanity/humanity.hpp>

HUMANITY_NS_BEGIN

/**
 * コピー禁止を明示するためのテンプレートクラス
 */
template <typename T> class non_copyable {
protected:
	non_copyable() {}
	~non_copyable() {}
private:
	non_copyable(non_copyable const&);
	T &operator = (T const&);
};

/**
 * 任意回数のリトライを行うためのテンプレートクラス
 */
template <uint32_t n> class retry {
public:
	/** 引数に渡された処理を行い、次の処理を呼び出すoperator() */
	template <typename Pred_> void operator () (Pred_ pred) {
		if (!pred()) {
			retry<n - 1>()(pred);
		}
	}
};

/**
 * retryの特殊化
 */
template <> class retry<0> {
public:
	/** 何もしないoperator() */
	template <typename Pred_> void operator () (Pred_ pred) {
	}
};

HUMANITY_NS_END

#endif // end of HUMANITY_UTILS_H

