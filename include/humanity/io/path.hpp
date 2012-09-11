/**
 * パスを扱うためのクラス定義ファイル
 * @file path.hpp
 */

#ifndef HUMANITY_IO_PATH_H
#define HUMANITY_IO_PATH_H

#include <humanity/humanity.hpp>
#include <humanity/memory.hpp>
#include <string>

HUMANITY_NS_BEGIN

/**
 * ファイルのパスを扱うクラス
 */
class path {
private:
	struct impl;

public:
	path();
	path(std::string const &path_str);
	path(char const *path_str);
	path(char const *path_str, std::size_t n);
	path(path const &src);
	~path();

	path &operator = (path const &r);

	bool operator == (path const &r) const;
	bool operator != (path const &r) const;

	path operator + (path const &r) const;
	path &operator += (path const &r);

	bool empty() const;
	bool is_relative() const;
	bool is_absolute() const;
	bool is_parent(path const &child) const;

	path make_relative(path const &child) const;

	std::string file_name() const;
	char const *full_path() const;

	path parent() const;
	path add_file_name_suffix(std::string const &suffix) const;

private:
	auto_ptr<impl> pimpl;
};

HUMANITY_NS_END

#endif // end of HUMANITY_IO_PATH_H

