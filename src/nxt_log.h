
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) NGINX, Inc.
 */

#ifndef _NXT_LOG_H_INCLUDED_
#define _NXT_LOG_H_INCLUDED_


#define NXT_LOG_EMERG      0
#define NXT_LOG_ALERT      1
#define NXT_LOG_CRIT       2
#define NXT_LOG_ERR        3
#define NXT_LOG_WARN       4
#define NXT_LOG_NOTICE     5
#define NXT_LOG_INFO       6
#define NXT_LOG_DEBUG      7


#define NXT_MAX_ERROR_STR  2048


typedef void nxt_cdecl (*nxt_log_handler_t)(nxt_uint_t level, nxt_log_t *log,
    const char *fmt, ...);
typedef u_char *(*nxt_log_ctx_handler_t)(void *ctx, u_char *pos, u_char *end);


struct nxt_log_s {
    uint32_t               level;
    uint32_t               ident;
    nxt_log_handler_t      handler;
    nxt_log_ctx_handler_t  ctx_handler;
    void                   *ctx;
};


NXT_EXPORT void nxt_log_start(const char *name);
NXT_EXPORT nxt_log_t *nxt_log_set_ctx(nxt_log_t *log,
    nxt_log_ctx_handler_t handler, void *ctx);

NXT_EXPORT void nxt_cdecl nxt_log_handler(nxt_uint_t level, nxt_log_t *log,
    const char *fmt, ...);


#define                                                                       \
nxt_log_level_enough(log, level)                                              \
    ((log)->level >= (level))


#define                                                                       \
nxt_log_emerg(_log, ...)                                                      \
    do {                                                                      \
        nxt_log_t  *_log_ = (_log);                                           \
                                                                              \
        _log_->handler(NXT_LOG_EMERG, _log_, __VA_ARGS__);                    \
    } while (0)


#define                                                                       \
nxt_log_alert(_log, ...)                                                      \
    do {                                                                      \
        nxt_log_t  *_log_ = (_log);                                           \
                                                                              \
        _log_->handler(NXT_LOG_ALERT, _log_, __VA_ARGS__);                    \
    } while (0)


#define nxt_log(task, _level, ...)                                            \
    do {                                                                      \
        nxt_log_t   *log = (task)->log;                                       \
        nxt_uint_t  _level_ = (_level);                                       \
                                                                              \
        if (nxt_slow_path(log->level >= _level_)) {                           \
            log->handler(_level_, log, __VA_ARGS__);                          \
        }                                                                     \
    } while (0)


#define nxt_trace(task, ...)                                                  \
    do {                                                                      \
        nxt_log_t  *log = (task)->log;                                        \
                                                                              \
        if (nxt_slow_path(log->level >= NXT_LOG_NOTICE || nxt_trace)) {       \
            log->handler(NXT_LOG_NOTICE, log, __VA_ARGS__);                   \
        }                                                                     \
    } while (0)


#define                                                                       \
nxt_log_error(_level, _log, ...)                                              \
    do {                                                                      \
        nxt_log_t   *_log_ = (_log);                                          \
        nxt_uint_t  _level_ = (_level);                                       \
                                                                              \
        if (nxt_slow_path(_log_->level >= _level_)) {                         \
            _log_->handler(_level_, _log_, __VA_ARGS__);                      \
        }                                                                     \
    } while (0)


#if (NXT_DEBUG)

#define nxt_debug(task, ...)                                                  \
    do {                                                                      \
        nxt_log_t  *log = (task)->log;                                        \
                                                                              \
        if (nxt_slow_path(log->level == NXT_LOG_DEBUG || nxt_debug)) {        \
            log->handler(NXT_LOG_DEBUG, log, __VA_ARGS__);                    \
        }                                                                     \
    } while (0)


#define                                                                       \
nxt_log_debug(_log, ...)                                                      \
    do {                                                                      \
        nxt_log_t  *_log_ = (_log);                                           \
                                                                              \
        if (nxt_slow_path(_log_->level == NXT_LOG_DEBUG || nxt_debug)) {      \
            _log_->handler(NXT_LOG_DEBUG, _log_, __VA_ARGS__);                \
        }                                                                     \
    } while (0)


#define nxt_debug_alloc(_fmt, ARGS...)                                        \
    nxt_thread_log_debug("%08p: " _fmt, mp, ##ARGS)

#else

#define nxt_debug(...)
#define nxt_debug_alloc(...)

#define                                                                       \
nxt_log_debug(...)

#endif


#define                                                                       \
nxt_main_log_emerg(...)                                                       \
    nxt_log_emerg(&nxt_main_log, __VA_ARGS__)


#define                                                                       \
nxt_main_log_alert(...)                                                       \
    nxt_log_alert(&nxt_main_log, __VA_ARGS__)


#define                                                                       \
nxt_main_log_error(level, ...)                                                \
    nxt_log_error(level, &nxt_main_log, __VA_ARGS__)


#define                                                                       \
nxt_main_log_debug(...)                                                       \
    nxt_log_debug(&nxt_main_log, __VA_ARGS__)


NXT_EXPORT extern nxt_uint_t  nxt_debug;
NXT_EXPORT extern nxt_uint_t  nxt_trace;
NXT_EXPORT extern nxt_log_t   nxt_main_log;
NXT_EXPORT extern nxt_str_t   nxt_log_levels[];


#endif /* _NXT_LOG_H_INCLUDED_ */
