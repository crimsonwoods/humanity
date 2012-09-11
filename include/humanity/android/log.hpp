/**
 * ログ出力用マクロの定義ファイル
 * @file android/log.hpp
 */

#ifndef HUMANITY_ANDROID_LOG_H
#define HUMANITY_ANDROID_LOG_H

#ifndef HUMANITY_LOG_TAG
/** ログに出力されるタグ文字列 */
#  define HUMANITY_LOG_TAG "humanity"
#endif

#ifdef __ANDROID__
#  include <android/log.h>
#  if defined(HUMANITY_LOG_ENABLE_DETAIL)
#    define LOG_(fp, type, fmt, file, line, ...) __android_log_print(type, HUMANITY_LOG_TAG, "(%s:%d)" fmt, file, line, ##__VA_ARGS__)
#  else
#    define LOG_(fp, type, fmt, file, line, ...) __android_log_print(type, HUMANITY_LOG_TAG, fmt, ##__VA_ARGS__)
#  endif
#  define HUMANITY_LOG_LEVEL_INFO  ANDROID_LOG_INFO
#  define HUMANITY_LOG_LEVEL_VERB  ANDROID_LOG_VERB
#  define HUMANITY_LOG_LEVEL_ERROR ANDROID_LOG_ERROR
#  define HUMANITY_LOG_LEVEL_WARN  ANDROID_LOG_WARN
#  define HUMANITY_LOG_LEVEL_DEBUG ANDROID_LOG_DEBUG
#endif

#endif // end of HUMANITY_ANDROID_LOG_H

