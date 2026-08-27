#ifndef STR_H
#define STR_H

#ifndef STR_DEF
   #define STR_DEF
#endif

#ifndef STR_INTERNAL
   #define STR_INTERNAL static
#endif

#ifndef STR_SIZE_T
   #include <stddef.h>
   #define STR_SIZE_T size_t
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
      #define STR_REALLOC(ptr, n, n_old) realloc((ptr), (n))
   #endif

   #ifndef STR_FREE
      #define STR_FREE(ptr, n) free((ptr))
   #endif
#endif

#ifndef STR_STRLEN
   #define STR_ENABLE_INTERNAL_STRLEN
   #define STR_STRLEN str__strlen
#endif

#ifndef STR_STRNCMP
   #define STR_ENABLE_INTERNAL_STRNCMP
   #define STR_STRNCMP str__strncmp
#endif

#ifndef STR_MEMCPY
   #define STR_ENABLE_INTERNAL_MEMCPY
   #define STR_MEMCPY str__memcpy
#endif

#ifndef STR_MEMSET
   #define STR_ENABLE_INTERNAL_MEMSET
   #define STR_MEMSET str__memset
#endif

#ifndef STR_ISSPACE
   #define STR_ENABLE_INTERNAL_ISSPACE
   #define STR_ISSPACE str__isspace
#endif

#ifndef STR_ASSERT
   #include <assert.h>
   #define STR_ASSERT assert
#endif

#define STR__EMPTY_VALUE {0}

#ifndef NULL
   #define NULL ((void*)0)
#endif

/**
 * Read-only view of a string *not* guaranteed to have a null
 * terminator '\0'.
 */
struct str {
   const STR_CHAR_T* bytes;
   STR_SIZE_T len;
};

/**
 * Read-only view of a string *guaranteed* to have a null terminator
 * '\0'.
 *
 * Useful for functions that require a null-terminated string (e.g.
 * POSIX open(), Win32 CreateFileA(), etc.).
 */
struct cstr {
   const STR_CHAR_T* bytes;
   STR_SIZE_T len;
};

/**
 * A dynamically-sized string of length LEN with a null terminator
 * '\0' at index LEN.
 *
 * CAP stores the allocated size of BYTES. Max length before a
 * reallocation is (CAP - 1). CAP includes the byte for null
 * terminator.
 */
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
STR_DEF struct cstrbuf cstrbuf_from_cstr(struct cstr s);

STR_DEF struct cstrbuf cstrbuf_clone(struct cstrbuf s);
STR_DEF void cstrbuf_delete(struct cstrbuf* s);
STR_DEF STR_BOOL_T cstrbuf_resize(struct cstrbuf *s, STR_SIZE_T new_cap);
STR_DEF STR_BOOL_T cstrbuf_shrink_to_fit(struct cstrbuf *s);

STR_DEF int str_compare(struct str a, struct str b);
STR_DEF STR_BOOL_T str_equal(struct str a, struct str b);

STR_DEF struct str str_substr(struct str s, STR_SIZE_T begin, STR_SIZE_T end);
STR_DEF struct str str_substr_n(struct str s, STR_SIZE_T begin, STR_SIZE_T len);

STR_DEF STR_SIZE_T str_find_char_front(struct str s, STR_CHAR_T c);
STR_DEF STR_SIZE_T str_find_char_back(struct str s, STR_CHAR_T c);
STR_DEF STR_SIZE_T str_find_str_front(struct str s, struct str a);
STR_DEF STR_SIZE_T str_find_str_back(struct str s, struct str a);

STR_DEF struct str str_cut_front(struct str s, STR_SIZE_T n);
STR_DEF struct str str_cut_back(struct str s, STR_SIZE_T n);
STR_DEF struct cstr cstr_cut_front(struct cstr s, STR_SIZE_T n);

STR_DEF STR_BOOL_T str_has_str_prefix(struct str s, struct str a);
STR_DEF STR_BOOL_T str_has_str_suffix(struct str s, struct str a);
STR_DEF struct str str_trim_str_prefix(struct str s, struct str a);
STR_DEF struct str str_trim_str_suffix(struct str s, struct str a);
STR_DEF struct cstr cstr_trim_str_prefix(struct cstr s, struct str a);

STR_DEF struct str str_trim_func(struct str s, int trim_func(int c));
STR_DEF struct str str_trim_func_front(struct str s, int trim_func(int c));
STR_DEF struct str str_trim_func_back(struct str s, int trim_func(int c));
STR_DEF struct str str_trim_space(struct str s);
STR_DEF struct str str_trim_space_front(struct str s);
STR_DEF struct str str_trim_space_back(struct str s);

STR_DEF struct cstr cstr_trim_func_front(struct cstr s, int trim_func(int c));
STR_DEF struct cstr cstr_trim_space_front(struct cstr s);

STR_DEF struct cstrbuf cstrbuf_append_str(struct cstrbuf* s, struct str a);
STR_DEF struct cstrbuf cstrbuf_copy_from_str(struct cstrbuf* s, struct str a);

STR_INTERNAL struct cstr cstr__from_bytes_n(const STR_CHAR_T* bytes, STR_SIZE_T len);

#ifdef STR_ENABLE_INTERNAL_STRLEN
STR_INTERNAL STR_SIZE_T str__strlen(const STR_CHAR_T* bytes);
#endif

#ifdef STR_ENABLE_INTERNAL_STRNCMP
STR_INTERNAL int str__strncmp(const STR_CHAR_T* a, const STR_CHAR_T* b, STR_SIZE_T n);
#endif

#ifdef STR_ENABLE_INTERNAL_MEMCPY
STR_INTERNAL void* str__memcpy(void* destination, const void* source, STR_SIZE_T n);
#endif

#ifdef STR_ENABLE_INTERNAL_MEMSET
STR_INTERNAL void* str__memset(void* destination, char c, STR_SIZE_T n);
#endif

#ifdef STR_ENABLE_INTERNAL_ISSPACE
STR_INTERNAL int str__isspace(int c);
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

STR_DEF struct cstrbuf cstrbuf_from_cstr(struct cstr s)
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

STR_DEF STR_BOOL_T cstrbuf_resize(struct cstrbuf *s, STR_SIZE_T new_cap)
{
   char* tmp;
   STR_ASSERT(new_cap >= 0);
   if (!s) {
      return 0;
   }
   if (new_cap <= s->len) {
      return 0;
   }
   if (s->cap == new_cap) {
      return 0;
   }

   if (s->cap == 0) {
      *s = cstrbuf_from_bytes_reserve(NULL, 0, new_cap);
      return 1;
   }

   tmp = (char*)STR_REALLOC(s->bytes, new_cap * sizeof(*s->bytes), s->cap * sizeof(*s->bytes));
   if (!tmp) {
      return 0;
   }
   s->bytes = tmp;
   s->cap = new_cap;

   return 1;
}

STR_DEF STR_BOOL_T cstrbuf_shrink_to_fit(struct cstrbuf *s)
{
   if (!s) {
      return 0;
   }
   return cstrbuf_resize(s, s->len + 1);
}

STR_DEF int str_compare(struct str a, struct str b)
{
   STR_SIZE_T min_len = a.len < b.len ? a.len : b.len;
   int diff = STR_STRNCMP(a.bytes, b.bytes, min_len);
   if (diff != 0) {
      return diff;
   }

   if (a.len > b.len) {
      return 1;
   } else if (a.len < b.len) {
      return -1;
   } else {
      return 0;
   }
}

STR_DEF STR_BOOL_T str_equal(struct str a, struct str b)
{
   return str_compare(a, b) == 0;
}

STR_DEF struct str str_substr(struct str s, STR_SIZE_T begin, STR_SIZE_T end)
{
   struct str result = STR__EMPTY_VALUE;
   STR_ASSERT(begin >= 0);
   STR_ASSERT(end >= 0);

   end = end < s.len ? end : s.len;
   begin = begin < end ? begin : end;

   result.bytes = s.bytes + begin;
   result.len = end - begin;
   return result;
}

STR_DEF struct str str_substr_n(struct str s, STR_SIZE_T begin, STR_SIZE_T len)
{
   struct str result = STR__EMPTY_VALUE;
   STR_ASSERT(begin >= 0);
   STR_ASSERT(len >= 0);

   if (begin > s.len) {
      begin = s.len;
      len = 0;
   }

   if (len > s.len) {
      len = s.len - begin;
   }

   if ((begin + len) > s.len) {
      len = s.len - begin;
   }

   result.bytes = s.bytes + begin;
   result.len = len;
   return result;
}

STR_DEF struct str str_cut_front(struct str s, STR_SIZE_T n)
{
   struct str result;
   STR_ASSERT(n >= 0);

   n = n < s.len ? n : s.len;

   result.bytes = s.bytes + n;
   result.len = s.len - n;
   return result;
}

STR_DEF struct str str_cut_back(struct str s, STR_SIZE_T n)
{
   struct str result;
   STR_ASSERT(n >= 0);

   n = n < s.len ? n : s.len;

   result.bytes = s.bytes;
   result.len = s.len - n;
   return result;
}

STR_DEF struct cstr cstr_cut_front(struct cstr s, STR_SIZE_T n)
{
   struct cstr result;
   STR_ASSERT(n >= 0);

   n = n < s.len ? n : s.len;

   result.bytes = s.bytes + n;
   result.len = s.len - n;
   return result;
}

STR_DEF STR_SIZE_T str_find_char_front(struct str s, STR_CHAR_T c)
{
   STR_SIZE_T i;
   for (i = 0; i < s.len; i++) {
      if (s.bytes[i] == c) {
         return i;
      }
   }
   return s.len;
}

STR_DEF STR_SIZE_T str_find_char_back(struct str s, STR_CHAR_T c)
{
   STR_SIZE_T i;
   for (i = s.len; i-- > 0;) {
      if (s.bytes[i] == c) {
         return i;
      }
   }
   return s.len;
}

STR_DEF STR_SIZE_T str_find_str_front(struct str s, struct str a)
{
   STR_SIZE_T i;
   for (i = 0; i < s.len - a.len + 1; i++) {
      struct str it = str_substr_n(s, i, a.len);
      if (str_equal(it, a)) {
         return i;
      }
   }
   return s.len;
}

STR_DEF STR_SIZE_T str_find_str_back(struct str s, struct str a)
{
   STR_SIZE_T i;
   for (i = s.len - a.len + 1; i-- > 0;) {
      struct str it = str_substr_n(s, i, a.len);
      if (str_equal(it, a)) {
         return i;
      }
   }
   return s.len;
}

STR_DEF STR_BOOL_T str_has_str_prefix(struct str s, struct str a)
{
   struct str current;
   if (s.len < a.len) {
      return 0;
   }
   current = str_substr(s, 0, a.len);
   return str_equal(current, a);
}

STR_DEF STR_BOOL_T str_has_str_suffix(struct str s, struct str a)
{
   struct str current;
   if (s.len < a.len) {
      return 0;
   }
   current = str_substr(s, s.len - a.len, s.len);
   return str_equal(current, a);
}

STR_DEF struct str str_trim_str_prefix(struct str s, struct str a)
{
   if (!str_has_str_prefix(s, a)) {
      return s;
   }
   return str_substr(s, a.len, s.len);
}

STR_DEF struct str str_trim_str_suffix(struct str s, struct str a)
{
   if (!str_has_str_suffix(s, a)) {
      return s;
   }
   return str_substr(s, 0, s.len - a.len);
}

STR_DEF struct cstr cstr_trim_str_prefix(struct cstr s, struct str a)
{
   if (!str_has_str_prefix(str_from_cstr(s), a)) {
      return s;
   }
   return cstr_cut_front(s, a.len);
}

STR_DEF struct str str_trim_func(struct str s, int trim_func(int c))
{
   return str_trim_func_front(str_trim_func_back(s, trim_func), trim_func);
}

STR_DEF struct str str_trim_func_front(struct str s, int trim_func(int c))
{
   STR_SIZE_T i;

   if (!trim_func) {
      return s;
   }

   for (i = 0; i < s.len; i++) {
      if (!trim_func(s.bytes[i])) {
         return str_substr(s, i, s.len);
      }
   }
   return str_from_bytes_n(s.bytes + s.len, 0);
}

STR_DEF struct str str_trim_func_back(struct str s, int trim_func(int c))
{
   STR_SIZE_T i;

   if (!trim_func) {
      return s;
   }

   for (i = s.len; i-- > 0;) {
      if (!trim_func(s.bytes[i])) {
         return str_substr(s, 0, i + 1);
      }
   }
   return str_from_bytes_n(s.bytes, 0);
}

STR_DEF struct str str_trim_space(struct str s)
{
   return str_trim_space_front(str_trim_space_back(s));
}

STR_DEF struct str str_trim_space_front(struct str s)
{
   return str_trim_func_front(s, STR_ISSPACE);
}

STR_DEF struct str str_trim_space_back(struct str s)
{
   return str_trim_func_back(s, STR_ISSPACE);
}

STR_DEF struct cstr cstr_trim_func_front(struct cstr s, int trim_func(int c))
{
   STR_SIZE_T i;
   struct cstr empty = STR__EMPTY_VALUE;

   if (!trim_func) {
      return s;
   }

   for (i = 0; i < s.len; i++) {
      if (!trim_func(s.bytes[i])) {
         return cstr_cut_front(s, i);
      }
   }
   return empty;
}

STR_DEF struct cstr cstr_trim_space_front(struct cstr s)
{
   return cstr_trim_func_front(s, STR_ISSPACE);
}

STR_DEF struct cstrbuf cstrbuf_append_str(struct cstrbuf* s, struct str a)
{
   STR_SIZE_T new_len;
   struct cstrbuf empty = STR__EMPTY_VALUE;
   if (!s) {
      return empty;
   }

   new_len = s->len + a.len;
   if (s->cap == 0) {
      *s = cstrbuf_from_str(s);
      return *s;
   } else if (s->cap <= new_len) {
      STR_CHAR_T* tmp = NULL;
      STR_SIZE_T new_cap = s->cap * 2;

      while (new_cap <= new_len) {
         new_cap *= 2;
         STR_ASSERT(new_cap > 0 && "overflow");
      }

      tmp = (STR_CHAR_T*)STR_REALLOC(s->bytes, new_cap * sizeof(*s->bytes), s->cap * sizeof(*s->bytes));
      if (!tmp) {
         return empty;
      }
      s->bytes = tmp;
      s->cap = new_cap;
   }

   STR_ASSERT(s->bytes[s->len] == '\0' && "invariant violated somehow");
   STR_MEMCPY(s->bytes + s->len, a.bytes, a.len * sizeof(*s->bytes));
   s->len = new_len;
   s->bytes[s->len] = '\0';

   return *s;
}

STR_DEF struct cstrbuf cstrbuf_copy_from_str(struct cstrbuf* s, struct str a)
{
   STR_SIZE_T new_len;
   struct cstrbuf empty = STR__EMPTY_VALUE;
   if (!s) {
      return empty;
   }

   new_len = a.len;
   if (s->cap == 0) {
      *s = cstrbuf_from_str(s);
      return *s;
   } else if (s->cap <= new_len) {
      STR_CHAR_T* tmp = NULL;
      STR_SIZE_T new_cap = s->cap * 2;

      while (new_cap <= new_len) {
         new_cap *= 2;
         STR_ASSERT(new_cap > 0 && "overflow");
      }

      tmp = (STR_CHAR_T*)STR_REALLOC(s->bytes, new_cap * sizeof(*s->bytes), s->cap * sizeof(*s->bytes));
      if (!tmp) {
         return empty;
      }
      s->bytes = tmp;
      s->cap = new_cap;
   }

   STR_MEMSET(s->bytes, 0, s->len * sizeof(*s->bytes));
   STR_MEMCPY(s->bytes, a.bytes, a.len * sizeof(*s->bytes));
   s->len = new_len;
   s->bytes[s->len] = '\0';

   return *s;
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

#ifdef STR_ENABLE_INTERNAL_STRNCMP
STR_INTERNAL int str__strncmp(const STR_CHAR_T* a, const STR_CHAR_T* b, STR_SIZE_T n)
{
   STR_SIZE_T i;
   for (i = 0; i < n; i++) {
      if (a[i] < b[i]) {
         return -1;
      } else if (a[i] > b[i]) {
         return 1;
      }
   }
   return 0;
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

#ifdef STR_ENABLE_INTERNAL_MEMSET
STR_INTERNAL void* str__memset(void* destination, char c, STR_SIZE_T n)
{
   STR_SIZE_T i;
   for (i = 0; i < n; i++) {
      ((char*)destination)[i] = c;
   }
   return destination;
}
#endif

#ifdef STR_ENABLE_INTERNAL_ISSPACE
STR_INTERNAL int str__isspace(int c)
{
   return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}
#endif

#endif /* STR_IMPLEMENTATION */
