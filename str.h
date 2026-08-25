#ifndef STR_H
#define STR_H

#ifndef STR_DEF
   #define STR_DEF
#endif

#ifndef STR_INTERNAL
   #define STR_INTERNAL static
#endif

#ifndef STR_SIZE_T
   #define STR_SIZE_T int
#endif

#ifndef STR_CHAR_T
   #define STR_CHAR_T char
#endif

#ifndef STR_BOOL_T
   #if __STDC_VERSION__ >= 199901L
      #if __STDC_VERSION__ < 202311L
         #include <stdbool.h>
      #endif
      #define STR_BOOL_T bool
   #else
      #define STR_BOOL_T int
   #endif
#endif

#if !defined(STR_MALLOC) || !defined(STR_REALLOC) || !defined(STR_FREE)
   #include <stdlib.h>

   #ifndef STR_MALLOC
      #define STR_MALLOC(n) malloc((n))
   #endif

   #ifndef STR_REALLOC
      #define STR_REALLOC(ptr, n, n_old) \
         do {                            \
            (void)(n_old);               \
            realloc((ptr), (n));         \
         } while (0)
   #endif

   #ifndef STR_FREE
      #define STR_FREE(ptr, n) \
         do {                  \
            (void)(n);         \
            free((ptr));       \
         } while (0)
   #endif
#endif

#ifndef STR_STRLEN
   #define STR_ENABLE_INTERNAL_STRLEN
   #define STR_STRLEN str__strlen
#endif

#ifndef STR_MEMCPY
   #define STR_ENABLE_INTERNAL_MEMCPY
   #define STR_MEMCPY str__memcpy
#endif

#ifndef STR_ASSERT
   #include <assert.h>
   #define STR_ASSERT assert
#endif

#define STR__EMPTY_VALUE {0}

#ifndef NULL
   #define NULL ((void*)0)
#endif

struct str {
   const STR_CHAR_T* bytes;
   STR_SIZE_T len;
};

struct cstr {
   const STR_CHAR_T* bytes;
   STR_SIZE_T len;
};

struct cstrbuf {
   STR_CHAR_T* bytes;
   STR_SIZE_T len;
   STR_SIZE_T cap;
};

#ifdef __cplusplus
extern "C" {
#endif

STR_DEF struct str str_from_bytes(const STR_CHAR_T* bytes);
STR_DEF struct str str_from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len);
STR_DEF struct str str_from_cstr(struct cstr s);

STR_DEF struct cstr cstr_from_bytes(const STR_CHAR_T* bytes);

STR_DEF struct cstrbuf cstrbuf_from_bytes(const STR_CHAR_T* bytes);
STR_DEF struct cstrbuf cstrbuf_from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len);
STR_DEF struct cstrbuf cstrbuf_from_bytes_reserve(const STR_CHAR_T* bytes, STR_SIZE_T len, STR_SIZE_T cap);
STR_DEF struct cstrbuf cstrbuf_from_str(struct str s);
STR_DEF struct cstrbuf cstrbuf_clone(struct cstrbuf s);
STR_DEF void cstrbuf_delete(struct cstrbuf* s);

STR_INTERNAL struct cstr cstr__from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len);

#ifdef STR_ENABLE_INTERNAL_STRLEN
STR_INTERNAL STR_SIZE_T str__strlen(const STR_CHAR_T* bytes);
#endif

#ifdef STR_ENABLE_INTERNAL_MEMCPY
STR_INTERNAL void* str__memcpy(void* destination, const void* source, STR_SIZE_T n);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STR_H */

#ifdef STR_IMPLEMENTATION

STR_DEF struct str str_from_bytes(const STR_CHAR_T* bytes)
{
   return str_from_bytes_n(bytes, STR_STRLEN(bytes));
}

STR_DEF struct str str_from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len)
{
   struct str result = STR__EMPTY_VALUE;
   STR_ASSERT(len >= 0);

   result.bytes = bytes;
   result.len = len;
   return result;
}

STR_DEF struct str str_from_cstr(struct cstr s)
{
   return str_from_bytes_n(s.bytes, s.len);
}

STR_DEF struct cstr cstr_from_bytes(const STR_CHAR_T* bytes)
{
   struct cstr result = STR__EMPTY_VALUE;

   result.bytes = bytes;
   result.len = STR_STRLEN(bytes);
   STR_ASSERT(result.bytes[result.len] == '\0');
   return result;
}

STR_DEF struct cstrbuf cstrbuf_from_bytes(const STR_CHAR_T* bytes)
{
   return cstrbuf_from_bytes_n(bytes, STR_STRLEN(bytes));
}

STR_DEF struct cstrbuf cstrbuf_from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len)
{
   return cstrbuf_from_bytes_reserve(bytes, len, len + 1);
}

STR_DEF struct cstrbuf cstrbuf_from_bytes_reserve(const STR_CHAR_T* bytes, STR_SIZE_T len, STR_SIZE_T cap)
{
   struct cstrbuf result = STR__EMPTY_VALUE;
   STR_ASSERT(len >= 0);
   STR_ASSERT(cap >= 0);
   if (len >= cap) {
      return result;
   }

   result.bytes = (STR_CHAR_T*)STR_MALLOC(cap * sizeof(*result.bytes));
   result.cap = cap;
   STR_MEMCPY(result.bytes, bytes, len);
   result.len = len;
   result.bytes[result.len] = '\0';
   return result;
}

STR_DEF struct cstrbuf cstrbuf_from_str(struct str s)
{
   return cstrbuf_from_bytes_n(s.bytes, s.len);
}

STR_DEF struct cstrbuf cstrbuf_clone(struct cstrbuf s)
{
   return cstrbuf_from_bytes_reserve(s.bytes, s.len, s.cap);
}

STR_DEF void cstrbuf_delete(struct cstrbuf* s)
{
   if (!s) {
      return;
   }

   if (s->bytes) {
      STR_FREE(s->bytes, s->cap * sizeof(*s->bytes));
      s->bytes = NULL;
   }
   s->len = 0;
   s->cap = 0;
}

STR_INTERNAL struct cstr cstr__from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len)
{
   struct cstr result;
   STR_ASSERT(len >= 0);
   STR_ASSERT((!bytes && !len) || (bytes && bytes[len] == '\0'));

   result.bytes = bytes;
   result.len = len;
   return result;
}

#ifdef STR_ENABLE_INTERNAL_STRLEN
STR_INTERNAL STR_SIZE_T str__strlen(const STR_CHAR_T* bytes)
{
   STR_SIZE_T len = 0;
   if (bytes) {
      while (*bytes++) {
         len++;
      }
   }
   return len;
}
#endif

#ifdef STR_ENABLE_INTERNAL_MEMCPY
STR_INTERNAL void* str__memcpy(void* destination, const void* source, STR_SIZE_T n)
{
   STR_SIZE_T i;
   for (i = 0; i < n; i++) {
      ((char*)destination)[i] = ((const char*)source)[i];
   }
   return destination;
}
#endif

#endif /* STR_IMPLEMENTATION */
