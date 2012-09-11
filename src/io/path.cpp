#include <humanity/io/path.hpp>
#include <humanity/exception.hpp>
#include <humanity/string_utils.hpp>
#include <string>
#include <cstddef>
#include <stack>

#define C_FILE_SEPARATOR '/'
#define S_FILE_SEPARATOR "/"

HUMANITY_IO_NS_BEGIN

typedef std::stack<std::string> path_element_stack;
static void split(std::string const &str, path_element_stack &elements);
static std::string build(path_element_stack &elements);
static std::string to_canonical(std::string const &str);


/**
 * パスの内部実装用データ構造
 */
struct path::impl {
	/** パスを保持するインスタンス */
	std::string path_;

	impl() : path_() {
	}
	/** パス文字列を指定して構築するコンストラクタ */
	impl(char const *path_str) : path_(path_str) {
	}
	/** パス文字列と文字列の長さを指定して構築するコンストラクタ */
	impl(char const *path_str, std::size_t n) : path_(path_str, n) {
	}
	/** パス文字列を指定して構築するコンストラクタ */
	impl(std::string const &path_str) : path_(path_str) {
	}
	~impl() {
	}
};

path::path()
	: pimpl(new impl)
{
}

/** パス文字列を指定して構築するコンストラクタ */
path::path(std::string const &path_str)
	: pimpl(new impl(path_str))
{
}

/** NULL終端されたパス文字列を指定して構築するコンストラクタ */
path::path(char const *path_str)
	: pimpl(new impl(path_str))
{
}

/** パス文字列と文字列の長さを指定して構築するコンストラクタ */
path::path(char const *path_str, std::size_t n)
	: pimpl(new impl(path_str, n))
{
}

/** コピーコンストラクタ */
path::path(path const &src)
	: pimpl(new impl(src.pimpl->path_))
{
}

path::~path()
{
}

/** 代入演算子 */
path &path::operator = (path const &r)
{
	pimpl->path_ = r.pimpl->path_;
	return *this;
}

/** 等値比較演算子 */
bool path::operator == (path const &r) const
{
	return pimpl->path_ == r.pimpl->path_;
}

/** 等値比較演算子 */
bool path::operator != (path const &r) const
{
	return !(*this == r);
}

/**
 * パス文字列の結合を行う。<br/>
 * 引数で渡されたパス文字列はセパレータで区切られて結合される（ディレクトリとして結合される）。
 * @param r 結合するパス文字列
 * @return 結合結果のパス文字列
 */
path path::operator + (path const &r) const
{
	path ret(pimpl->path_.c_str());
	ret += r;
	return ret;
}

/**
 * パス文字列の結合を行う演算子
 * @param r 結合対象のパス
 * @return 結合後のパス
 */
path &path::operator += (path const &r)
{
	path_element_stack stack;

	split(pimpl->path_, stack);
	split(r.pimpl->path_, stack);
	pimpl->path_ = build(stack);

	return *this;
}

/**
 * パス文字列が空かどうか判定する
 * @return パス文字列が空の場合はtrue、そうでなければfalseを返す
 */
bool path::empty() const
{
	return pimpl->path_.empty();
}

/**
 * パスが相対パスかどうかを判定する
 * @return 相対パスの場合はtrue、そうでなければfalseを返す
 */
bool path::is_relative() const
{
	if (pimpl->path_.empty()) {
		return false;
	}
	char const c = *pimpl->path_.begin();
	return c == C_FILE_SEPARATOR ? false : true;
}

/**
 * パスが絶対パスかどうかを判定する
 * @return 絶対パスの場合はtrue、そうでなければfalseを返す
 */
bool path::is_absolute() const
{
	if (pimpl->path_.empty()) {
		return false;
	}
	return !is_relative();
}

/**
 * パスが引数に指定したパスの親のパスかどうかを判定する。<br/>
 * 引数に指定したパスとの間に複数のディレクトリが存在しても親と判定する。
 * @param child 子要素へのパス
 * @return 親のパスの場合はtrue、そうでなければfalseを返す
 */
bool path::is_parent(path const &child) const
{
	std::string const p = to_canonical(pimpl->path_);
	std::string const q = to_canonical(child.pimpl->path_);
	if (p.empty() || q.empty()) {
		return false;
	}
	if (p.length() >= q.length()) {
		return false;
	}
	return 0 == child.pimpl->path_.compare(0, pimpl->path_.length(), pimpl->path_);
}

/**
 * 引数に指定したパスとの相対パスを生成する
 * @param child 子要素へのパス
 * @return 正常にパスが生成された場合はそのパス、そうでなければ空のパスを返す
 */
path path::make_relative(path const &child) const
{
	if (!is_parent(child)) {
		return path();
	}
	std::string const p = to_canonical(pimpl->path_);
	std::string q = to_canonical(child.pimpl->path_);
	return q.erase(0, p.length() + 1);
}

/**
 * パスからファイルまたはディレクトリの名前を取得する
 * @return ファイルまたはディレクトリの名前
 */
std::string path::file_name() const
{
	path_element_stack stack;
	split(pimpl->path_, stack);
	for(; !stack.empty(); stack.pop()) {
		path_element_stack::value_type const &top = stack.top();
		if (top.empty()) {
			continue;
		}
		if (top == std::string(".")) {
			continue;
		}
		if (top == std::string("..")) {
			if (stack.empty()) {
				throw std::runtime_error("cannot over the top level directory");
			}
			stack.pop();
			continue;
		}
		return top;
	}
	return std::string();
}

/**
 * パス文字列を取得する
 * @return パス文字列の先頭へのポインタを返す
 */
char const *path::full_path() const
{
	return pimpl->path_.c_str();
}

/**
 * 親ディレクトリのパスを生成する
 * @return 生成された親ディレクトリのパスを返す
 */
path path::parent() const
{
	std::string const p = to_canonical(pimpl->path_);
	path_element_stack stack;
	split(p, stack);
	if (!stack.empty()) {
		stack.pop();
	}
	return build(stack);
}

/**
 * ファイル名に接尾辞を追加したパスを生成する
 * @param suffix 追加する接尾辞の文字列
 * @return 生成されたパスを返す
 */
path path::add_file_name_suffix(std::string const &suffix) const
{
	if (pimpl->path_.empty()) {
		return path(suffix);
	}

	std::string file_name(this->file_name());
	file_name += suffix;

	return this->parent() + file_name;
}

static void split(std::string const &str, path_element_stack &elements)
{
	if (str.empty()) {
		return;
	}
	if (elements.empty() && (0 == str.find_first_of(C_FILE_SEPARATOR))) {
		elements.push(S_FILE_SEPARATOR); // push root element
	}
	std::string tmp = str;
	std::size_t offset = 0;
	for (;;) {
		std::size_t const pos = tmp.find_first_of(C_FILE_SEPARATOR, offset);
		if (pos == std::string::npos) {
			elements.push(tmp.substr(offset));
			return;
		}
		if (offset == pos) {
			++offset;
		} else {
			elements.push(tmp.substr(offset, pos - offset));
			tmp = tmp.substr(pos + 1);
			offset = 0;
		}
	}
}

static std::string build(path_element_stack &elements)
{
	std::string ret;
	int pop_count = 0;
	while(!elements.empty()) {
		path_element_stack::value_type elem = elements.top();

		elements.pop();

		if (elem.empty()) {
			continue;
		}

		if (elem == std::string(".")) {
			continue;
		}

		if (elem == std::string("..")) {
			++pop_count;
			continue;
		}

		if (0 < pop_count) {
			elements.push(elem);
			for (int i = 0; i < pop_count; ++i) {
				if (elements.empty()) {
					if (elem == S_FILE_SEPARATOR) {
						break;
					} else {
						ret.insert(0, "../");
					}
				} else {
					elem = elements.top();
					elements.pop();
				}
			}
			if (elements.empty() && (elem == S_FILE_SEPARATOR)) {
				throw std::runtime_error("cannot over the top level directory");
			}
			pop_count = 0;
			continue;
		}

		if (!ret.empty() && (elem != std::string(S_FILE_SEPARATOR))) {
			ret.insert(0, 1, C_FILE_SEPARATOR);
		}
		ret.insert(0, elem);
	}
	for (int i = 0; i < pop_count; ++i) {
		ret.insert(0, "../");
	}
	return ret;
}

static std::string to_canonical(std::string const &str)
{
	path_element_stack stack;
	split(str, stack);
	return build(stack);
}

HUMANITY_IO_NS_END

