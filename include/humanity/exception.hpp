/**
 * 例外クラスの定義ファイル
 * @file exception.hpp
 */

#ifndef HUMANITY_EXCEPTION_H
#define HUMANITY_EXCEPTION_H

#include <humanity/humanity.hpp>

#if defined(HUMANITY_ENABLE_EXCEPTIONS)

#  include <stdexcept>
#  include <cstring>
#  include <cerrno>

HUMANITY_NS_BEGIN

/**
 * システムコールで発生したエラーを通知するための例外クラス
 */
class system_call_error : public std::runtime_error {
private:
	int err_;

public:
	/**
	 * コンストラクタ
	 * @param err errnoに設定されたエラーコード
	 */
	system_call_error(int err = errno)
		: std::runtime_error(std::strerror(err)), err_(err)
	{
	}

	/**
	 * コンストラクタ
	 * @param msg メッセージ
	 * @param err errnoに設定されたエラーコード
	 */
	system_call_error(std::string const &msg, int err)
		: std::runtime_error(msg + " (" + std::strerror(err) + ")"), err_(err)
	{
	}

	/**
	 * エラーコードを取得する
	 * @return コンストラクタで指定されたエラーコードを返す
	 */
	int error_code() const {
		return err_;
	}
};

HUMANITY_NS_END

#  define THROW(type, ...)              throw type( __VA_ARGS__ )
#  define THROW_IF(cond, type, ...)     if ((cond))  { THROW(type, ##__VA_ARGS__ ); }
#  define THROW_IF_NOT(cond, type, ...) if (!(cond)) { THROW(type, ##__VA_ARGS__ ); }

#elif defined(HUMANITY_USE_ASSERT_INSTEAD_OF_EXCEPTIONS)

#  include <cassert>
#  define THROW(type, ...)              assert(false)
#  define THROW_IF(cond, type, ...)     assert(!(cond))
#  define THROW_IF_NOT(cond, type, ...) assert((cond))

#else

#  define THROW(type, ...)              
#  define THROW_IF(cond, type, ...)     
#  define THROW_IF_NOT(cond, type, ...) 

#endif // end of HUMANITY_ENABLE_EXCEPTIONS

#endif // end of HUMANITY_EXCEPTION_H

