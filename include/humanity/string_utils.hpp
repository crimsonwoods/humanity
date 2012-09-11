/**
 * 文字列処理用ユーティリティクラスの定義ファイル
 * @file string_utils.hpp
 */

#ifndef HUMANITY_STRING_UTILS_H
#define HUMANITY_STRING_UTILS_H

#include <humanity/humanity.hpp>
#include <string>

HUMANITY_NS_BEGIN

/**
 * 文字列に対する操作をまとめたヘルパークラス
 */
class string_utils {
public:
	static bool ends_with(std::string const &target, std::string const &suffix);
	static bool starts_with(std::string const &target, std::string const &prefix);
};

HUMANITY_NS_END

#endif // end of HUMANITY_STRING_UTILS_H

