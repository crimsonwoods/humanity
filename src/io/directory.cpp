#include <humanity/io/directory.hpp>
#include <humanity/io/file.hpp>
#include <humanity/io/path.hpp>
#include <humanity/exception.hpp>
#include <humanity/log.hpp>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <stack>

HUMANITY_IO_NS_BEGIN

/**
 * ディレクトリエントリの実装用内部データ構造
 */
struct directory_entry::impl {
	/** struct dirent型のインスタンス */
	::dirent entry_;

	impl() : entry_() {
		std::memset(&entry_, 0, sizeof(entry_));
	}
	/** コピーコンストラクタ */
	impl(impl const &src) : entry_(src.entry_) {
	}
};

directory_entry::directory_entry()
	: pimpl(new impl())
{
}

/**
 * コピーコンストラクタ
 */
directory_entry::directory_entry(directory_entry const &entry)
	: pimpl(new impl(*entry.pimpl.get()))
{
}

directory_entry::~directory_entry()
{
}

/**
 * 代入演算子
 */
directory_entry &directory_entry::operator = (directory_entry const &r)
{
	pimpl->entry_ = r.pimpl->entry_;
	return *this;
}

/**
 * エントリの名前を取得する
 * @return エントリの名前を返す
 */
char const *directory_entry::name() const
{
	return pimpl->entry_.d_name;
}

/**
 * エントリがディレクトリかどうか判定する
 * @return エントリがディレクトリであればtrue、そうでなければfalseを返す
 */
bool directory_entry::is_directory() const
{
	return DT_DIR == pimpl->entry_.d_type;
}

/**
 * エントリがシンボリックリンクかどうか判定する
 * @return エントリがシンボリックリンクであればtrue、そうでなければfalseを返す
 */
bool directory_entry::is_link() const
{
	return DT_LNK == pimpl->entry_.d_type;
}

/**
 * エントリがレギュラーファイルかどうか判定する
 * @return エントリがレギュラーファイルであればtrue、そうでなければfalseを返す
 */
bool directory_entry::is_regular() const
{
	return DT_REG == pimpl->entry_.d_type;
}

//////////////////////////////////////////////////////////////////////////////

/**
 * ディレクトリの内部実装用データ構造
 */
struct directory::impl {
	/** DIR型のインスタンス */
	unique_ptr<DIR, int(*)(DIR*)> dir_;
	/** 現在のエントリを保持するインスタンス */
	directory_entry entry_;

	/** opendirで開いたDIRを受けて構築するコンストラクタ */
	impl(DIR *dir) : dir_(dir, closedir), entry_() {
	}
	~impl() {}
};

/**
 * パスを指定してディレクトリを開く
 * @param path 開くディレクトリのパス
 */
directory::directory(path const &path)
	: pimpl()
{
	if (!path.empty()) {
		DIR *dir = ::opendir(path.full_path());
		THROW_IF(NULL == dir, system_call_error, "failed to open directory", errno);
		pimpl.reset(new impl(dir));
	}
}

directory::~directory() 
{
}

/**
 * ディレクトリ内部のエントリを一つ次に進める
 * @return 次のエントリが存在する場合はtrue、そうでなければfalseを返す
 */
bool directory::next()
{
	dirent *result = NULL;
	int const err = readdir_r(pimpl->dir_.get(), &pimpl->entry_.pimpl->entry_, &result);
	THROW_IF(0 != err, system_call_error, "failed to read directory", err);
	if (NULL == result) {
		return false;
	}
	return true;
}

/**
 * ディレクトリエントリ内部の現在のエントリを取得する
 * @return 現在のエントリを返す。対応するエントリが存在しない場合の動作は未定義。
 */
directory_entry &directory::entry()
{
	return pimpl->entry_;
}

/**
 * ディレクトリエントリ内部の現在のエントリを取得する
 * @return 現在のエントリを返す。対応するエントリが存在しない場合の動作は未定義。
 */
directory_entry const &directory::entry() const
{
	return pimpl->entry_;
}

/**
 * ディレクトリ中の全てのエントリを再帰的に探索して、エントリへのパスをコンテナに格納する
 * @param dir_path 探索対象のディレクトリのパス
 * @param container 各エントリへのパスを格納するためのコンテナ
 * @return 正常に探索が完了した場合はtrue、そうでなければfalse
 */
bool directory::scan_all(path const &dir_path, contained_file_names &container)
{
	return scan(dir_path, dir_path, container);
}

bool directory::scan(path const &root_dir_path, path const &dir_path, contained_file_names &container)
{
	directory dir(dir_path);
	while (dir.next()) {
		directory_entry const &entry = dir.entry();
		if ((0 == std::strncmp(entry.name(), ".", 2)) || (0 == std::strncmp(entry.name(), "..", 3))) {
			continue;
		}
		if (entry.is_directory()) {
			if (!directory::scan(root_dir_path, dir_path + entry.name(), container)) {
				return false;
			}
			continue;
		}
		if (entry.is_regular()) {
			path const &rel = root_dir_path.make_relative(dir_path + entry.name());
			container.push_back(rel.full_path());
			continue;
		}
	}
	return true;
}

/**
 * ディレクトリが存在するかどうか判定する
 * @param path 判定対象のディレクトリのパス
 * @return ディレクトリまたはファイルが存在した場合はtrue、そうでなければfalseを返す
 */
bool directory::is_exist(path const &path)
{
	return file::is_exist(path);
}

/**
 * ディレクトリをリネームする
 * @param src リネーム対象のディレクトリのパス
 * @param dst リネーム後のディレクトリのパス
 * @return 正常にリネームできた場合はtrue、そうでなければfalseを返す
 */
bool directory::rename(path const &src, path const &dst)
{
	return file::rename(src, dst);
}

/**
 * ディレクトリおよび内部のエントリを再帰的に削除する
 * @param dir_path 削除対象のディレクトリのパス
 * @return 正常に削除に成功した場合はtrue、そうでなければfalseを返す
 */
bool directory::rmdir(path const &dir_path)
{
	if (dir_path.empty()) {
		return false;
	}

	try {
		if (!directory::is_exist(dir_path)) {
			return true;
		}

		directory dir(dir_path);
		while (dir.next()) {
			directory_entry const &entry = dir.entry();
			if ((0 == std::strncmp(entry.name(), ".", 2)) || (0 == std::strncmp(entry.name(), "..", 3))) {
				continue;
			}

			path new_path = dir_path + entry.name();
			if (entry.is_directory()) {
				if (!directory::rmdir(new_path)) {
					return false;
				}
			} else if (entry.is_link() || entry.is_regular()) {
				if (0 != ::unlink(new_path.full_path())) {
					if (ENOENT != errno) {
						return false;
					}
				}
			} else {
				return false;
			}
		}
	} catch (system_call_error &ex) {
		LOGE("%s", ex.what());
		return false;
	}
	if (0 != ::rmdir(dir_path.full_path())) {
		if (ENOENT != errno) {
			return false;
		}
	}
	return true;
}

/**
 * 指定したディレクトリを作成する
 * @param dir_path 作成するディレクトリのパス
 * @return 正常にディレクトリが作成できた場合はtrue、そうでなければfalseを返す
 */
bool directory::mkdir(path const &dir_path)
{
	if (dir_path.empty()) {
		return false;
	}

	if (0 == ::mkdir(dir_path.full_path(), S_IRWXU)) {
		return true;
	}
	int const e = errno;
	if (e == EEXIST) {
		return true;
	}
	if (e != ENOENT) {
		return false;
	}

	std::stack<path> pstack;
	path current_dir(dir_path);
	pstack.push(current_dir);
	for (;;) {
		path parent_dir = current_dir.parent();
		if (parent_dir.empty()) {
			return false;
		}
		try {
			if (directory::is_exist(parent_dir)) {
				break;
			}
		} catch (system_call_error &ex) {
			LOGE("%s", ex.what());
			return false;
		}
		pstack.push(parent_dir);
		current_dir = parent_dir;
	}

	while (!pstack.empty()) {
		path const &dir = pstack.top();

		if (0 != ::mkdir(dir.full_path(), S_IRWXU)) {
			if (EEXIST != errno) {
				return false;
			}
		}

		pstack.pop();
	}
	return true;
}

HUMANITY_IO_NS_END

