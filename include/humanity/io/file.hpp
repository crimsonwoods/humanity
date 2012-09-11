/**
 * ファイルを扱うためのクラス定義ファイル
 * @file file.hpp
 */

#ifndef HUMANITY_IO_FILE_H
#define HUMANITY_IO_FILE_H

#include <humanity/io/io.hpp>

HUMANITY_IO_NS_BEGIN

class path;

/**
 * ファイルを扱うためのクラス
 */
class file {
public:
	/**
	 * ファイルのモードを表す定数値群
	 */
	enum {
		FMODE_USER_READ   = 0400,
		FMODE_USER_WRITE  = 0200,
		FMODE_USER_EXEC   = 0100,
		FMODE_USER_RW     = FMODE_USER_READ  | FMODE_USER_WRITE,
		FMODE_USER_RX     = FMODE_USER_READ  | FMODE_USER_EXEC,
		FMODE_USER_WX     = FMODE_USER_WRITE | FMODE_USER_EXEC,
		FMODE_USER_RWX    = FMODE_USER_READ  | FMODE_USER_WRITE | FMODE_USER_EXEC,
		FMODE_GROUP_READ  = 0040,
		FMODE_GROUP_WRITE = 0020,
		FMODE_GROUP_EXEC  = 0010,
		FMODE_GROUP_RW    = FMODE_GROUP_READ  | FMODE_GROUP_WRITE,
		FMODE_GROUP_RX    = FMODE_GROUP_READ  | FMODE_GROUP_EXEC,
		FMODE_GROUP_WX    = FMODE_GROUP_WRITE | FMODE_GROUP_EXEC,
		FMODE_GROUP_RWX   = FMODE_GROUP_READ  | FMODE_GROUP_WRITE | FMODE_GROUP_EXEC,
		FMODE_OTHER_READ  = 0004,
		FMODE_OTHER_WRITE = 0002,
		FMODE_OTHER_EXEC  = 0001,
		FMODE_OTHER_RW    = FMODE_OTHER_READ  | FMODE_OTHER_WRITE,
		FMODE_OTHER_RX    = FMODE_OTHER_READ  | FMODE_OTHER_EXEC,
		FMODE_OTHER_WX    = FMODE_OTHER_WRITE | FMODE_OTHER_EXEC,
		FMODE_OTHER_RWX   = FMODE_OTHER_READ  | FMODE_OTHER_WRITE | FMODE_OTHER_EXEC,
	};

	static bool is_link(path const &path);
	static bool is_exist(path const &path);
	static bool chmod(path const &path, uint16_t mode);
	static bool remove(path const &path);
	static bool rename(path const &src, path const &dst);
};

HUMANITY_IO_NS_END

#endif // end of HUMANITY_IO_FILE_H

