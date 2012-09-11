/**
 * @file log.hpp
 */

#ifndef HUMANITY_LOG_H
#define HUMANITY_LOG_H

#if defined(__ANDROID__)
#  include <humanity/android/log.hpp>
#elif defined(HUMANITY_LOG_STDIO)
#  include <cstdio>
#  define HUMANITY_LOG_LEVEL_INFO  ":INFO "
#  define HUMANITY_LOG_LEVEL_VERB  ":VERB "
#  define HUMANITY_LOG_LEVEL_ERROR ":ERR  "
#  define HUMANITY_LOG_LEVEL_WARN  ":WARN "
#  define HUMANITY_LOG_LEVEL_DEBUG ":DBG  "
#  if defined(HUMANITY_LOG_ENABLE_DETAIL)
#    define LOG_(fp, type, fmt, file, line, ...) std::fprintf(fp, HUMANITY_LOG_TAG type fmt "\n", ##__VA_ARGS__)
#  else
#    define LOG_(fp, type, fmt, file, line, ...) std::fprintf(fp, HUMANITY_LOG_TAG type "(%s:%d)" fmt "\n", file, line, ##__VA_ARGS__)
#  endif
#else
#  define HUMANITY_LOG_LEVEL_INFO
#  define HUMANITY_LOG_LEVEL_VERB
#  define HUMANITY_LOG_LEVEL_ERROR
#  define HUMANITY_LOG_LEVEL_WARN
#  define HUMANITY_LOG_LEVEL_DEBUG
#  define LOG_(fp, type, fmt, file, line, ...)
#endif

#define LOGI(fmt, ...)    LOG_(stdout, HUMANITY_LOG_LEVEL_INFO,  fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGV(fmt, ...)    LOG_(stdout, HUMANITY_LOG_LEVEL_VERB,  fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(fmt, ...)    LOG_(stderr, HUMANITY_LOG_LEVEL_ERROR, fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGW(fmt, ...)    LOG_(stderr, HUMANITY_LOG_LEVEL_WARN,  fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#if defined(HUMANITY_LOG_ENABLE_DEBUG)
#  define LOGD(fmt, ...)  LOG_(stdout, HUMANITY_LOG_LEVEL_DEBUG, fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#  define LOGED(fmt, ...) LOG_(stdout, HUMANITY_LOG_LEVEL_ERROR, fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#  define LOGWD(fmt, ...) LOG_(stdout, HUMANITY_LOG_LEVEL_WARN,  fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#  define LOGD(fmt, ...)
#  define LOGED(fmt, ...)
#  define LOGWD(fmt, ...)
#endif


#endif // end of HUMANITY_LOG_H

