#include <humanity/io/file.hpp>
#include <humanity/io/path.hpp>
#include <humanity/exception.hpp>
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>

HUMANITY_IO_NS_BEGIN

/**
 * ファイルがシンボリックリンクかどうか判定する
 * @param path 判定対象のファイルのパス
 * @return シンボリックリンクだった場合はtrue、そうでなければfalseを返す
 */
bool file::is_link(path const &path)
{
	if (path.empty()) {
		return false;
	}

	struct stat s = { 0, };
	if (0 == ::lstat(path.full_path(), &s)) {
		if (S_ISLNK(s.st_mode)) {
			return true;
		}
		return false;
	}

	THROW(system_call_error, "cannot get file status", errno);
}

/**
 * ファイルが存在するかどうか判定する
 * @param path 判定対象のファイルのパス
 * @return ファイルが存在する場合はtrue、そうでなければfalseを返す
 */
bool file::is_exist(path const &path)
{
	if (path.empty()) {
		return false;
	}

	struct stat s = { 0, };
	if (0 == ::lstat(path.full_path(), &s)) {
		return true;
	}
	int const e = errno;
	if ((ENOENT == e) || (ENOTDIR == e)) {
		return false;
	}
	THROW(system_call_error, "cannot check file existense", e);
}

/**
 * ファイルのモードを変更する
 * @param path モードを変更する対象のファイルのパス
 * @param mode 変更後のモード（複数のモード定数をビット和でまとめて指定する）
 * @return モード変更に成功した場合はtrue、そうでなければfalseを返す
 */
bool file::chmod(path const &path, uint16_t mode)
{
	if (path.empty()) {
		return false;
	}

	if (0 == ::chmod(path.full_path(), static_cast<mode_t>(mode))) {
		return true;
	}
	return false;
}

/**
 * 指定したファイルを削除する。<br/>
 * 実際にはstd::removeを呼び出す。
 * @param path 削除対象のファイルのパス
 * @return 削除に成功した場合はtrue、そうでなければfalseを返す
 */
bool file::remove(path const &path)
{
	if (path.empty()) {
		return false;
	}

	if (0 == std::remove(path.full_path())) {
		return true;
	}
	return false;
}

/**
 * ファイルの名前を変更する。<br/>
 * 実際にはstd::renameを呼び出す。
 * @param src 変更前のファイルのパス
 * @param dst 変更後のファイルのパス
 * @return 名前の変更に成功した場合はtrue、そうでなければfalseを返す
 */
bool file::rename(path const &src, path const &dst)
{
	if (src.empty() || dst.empty()) {
		return false;
	}

	if (0 == std::rename(src.full_path(), dst.full_path())) {
		return true;
	}
	return false;
}

HUMANITY_IO_NS_END

