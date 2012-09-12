#ifndef HUMANITY_CONFIG_H
#define HUMANITY_CONFIG_H

#if defined(_EXCEPTIONS) || defined(__EXCEPTIONS)
/** 例外の使用を許可する設定 */
#define HUMANITY_ENABLE_EXCEPTIONS
#endif

#if !defined(HUMANITY_ENABLE_EXCEPTIONS)
/** THROW系のマクロをassertに置換する設定 */
#define HUMANITY_USE_ASSERT_INSTEAD_OF_EXCEPTIONS
#endif

#endif // end of HUMANITY_CONFIG_H

