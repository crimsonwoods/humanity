/**
 * ディレクトリを扱うためのクラス定義ファイル
 * @file directory.hpp
 */

#ifndef HUMANITY_IO_DIRECTORY_H
#define HUMANITY_IO_DIRECTORY_H

#include <humanity/humanity.hpp>
#include <humanity/memory.hpp>
#include <cstring>
#include <string>
#include <vector>

HUMANITY_NS_BEGIN

class path;

/**
 * ディレクトリに格納されているエントリの情報を保持するクラス
 */
class directory_entry {
	friend class directory;
private:
	struct impl;

	directory_entry();
public:
	directory_entry(directory_entry const &entry);
	~directory_entry();

	directory_entry &operator = (directory_entry const &r);

	char const *name() const;
	bool is_directory() const;
	bool is_link() const;
	bool is_regular() const;

private:
	auto_ptr<impl> pimpl;
};

/**
 * ディレクトリをスキャンした結果を格納するためのコンテナクラス
 */
class contained_file_names : public std::vector<std::string> {
public:
	contained_file_names() : std::vector<std::string>() {}
	~contained_file_names() {}
};

/**
 * ディレクトリを扱うためのクラス
 */
class directory {
private:
	struct impl;

public:
	directory(path const &path);
	~directory();

	bool next();
	directory_entry &entry();
	directory_entry const &entry() const;

	static bool scan_all(path const &dir_path, contained_file_names &container);

	static bool is_exist(path const &path);
	static bool rename(path const &src, path const &dst);
	static bool rmdir(path const &path);
	static bool mkdir(path const &path);

private:
	static bool scan(path const &root_dir_path, path const &dir_path, contained_file_names &container);

	auto_ptr<impl> pimpl;
};

HUMANITY_NS_END

#endif // end of APPVERIFIER_DIRECTORY_H

