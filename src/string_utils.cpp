#include <humanity/string_utils.hpp>
#include <cstddef>

HUMANITY_NS_BEGIN

/**
 * 文字列が指定された文字列で終了しているかどうかを判定する
 * @param target 判定対象の文字列
 * @param suffix 終端文字列
 * @return 指定した終端文字列で終端している場合はtrue、そうでなければfalseを返す
 */
bool string_utils::ends_with(std::string const &target, std::string const &suffix)
{
	if (suffix.empty()) {
		return false;
	}
	if (target.length() < suffix.length()) {
		return false;
	}

	std::size_t const offset = target.length() - suffix.length();

	return 0 == target.compare(offset, suffix.length(), suffix);
}

/**
 * 文字列が指定された文字列で開始しているかどうかを判定する
 * @param target 判定対象の文字列
 * @param prefix 先頭文字列
 * @return 指定した先頭文字列で開始している場合はtrue、そうでなければfalseを返す
 */
bool string_utils::starts_with(std::string const &target, std::string const &prefix)
{
	if (prefix.empty()) {
		return false;
	}
	if (target.length() < prefix.length()) {
		return false;
	}

	return 0 == target.compare(0, prefix.length(), prefix);
}

HUMANITY_NS_END

