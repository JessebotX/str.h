/* clang-format off */
#ifndef STR_H_
#define STR_H_

#include <stdlib.h>

#ifndef STR_ASSERT
	#include <assert.h>
	#define STR_ASSERT assert
#endif

#ifndef STR_BYTE_T
	#define STR_BYTE_T char
#endif

#ifndef STR_BOOL_T
	#define STR_BOOL_T int
	#define STR_TRUE   1
	#define STR_FALSE  0
#endif

#ifndef STR_SIZE_T
	#include <stddef.h>
	#define STR_SIZE_T size_t
#endif

#ifndef STR_DEF
	#define STR_DEF
#endif /* #ifndef STR_DEF */

#define STR_FMT(s) ((int)(s).len), (s).data

typedef struct str_allocator {
	void* (*malloc)(STR_SIZE_T len, void* ctx);
	void* (*realloc)(void* ptr, STR_SIZE_T len_old, STR_SIZE_T len_new, void* ctx);
	void  (*free)(void* ptr, STR_SIZE_T len, void* ctx);
	void* ctx;
} StrAllocator;

/**
Str represents a counted string type that does not have to be
null terminated. It can either be a view into another string or a
dynamically-sized string.

NOTE: A dynamically-sized Str will have to be deallocated using
str_destroy.
**/
typedef struct str {
	STR_SIZE_T len;
	STR_SIZE_T cap;
	STR_BYTE_T* data;
} Str;

/**
StrArray is a dynamically-sized array that stores 0 or more Str
objects.
**/
typedef struct str_array {
	STR_SIZE_T len;
	STR_SIZE_T cap;
	Str* data;
} StrArray;

#ifdef __cplusplus
extern "C" {
#endif /* #ifdef __cplusplus */

/** FUNCTIONS **/

/**
Initialize an empty string, representing its default zero value.
**/
STR_DEF Str
str_zero(void);

/**
Initialize a string view from a null-terminated C string.
**/
STR_DEF Str
str(STR_BYTE_T* data);

/**
Initialize a string view from an array of bytes.
**/
STR_DEF Str
str_with_len(STR_BYTE_T* data, STR_SIZE_T len);

/**
Create a dynamically-sized string from a null-terminated c string.

NOTE: must be deallocated by caller using str_destroy.
**/
STR_DEF Str
str_dynamic(STR_BYTE_T* s, StrAllocator* allocator);

/**
Create a dynamically-sized string from an array of bytes.

NOTE: data must be deallocated by caller using str_destroy.
**/
STR_DEF Str
str_dynamic_with_len(STR_BYTE_T* s, STR_SIZE_T len, StrAllocator* allocator);

/**
Create a dynamically-sized string from an array of bytes. cap specifies
the initial buffer size in bytes.

NOTE: data must be deallocated by caller using str_destroy.
**/
STR_DEF Str
str_dynamic_with_len_cap(STR_BYTE_T* s, STR_SIZE_T len, STR_SIZE_T cap, StrAllocator* allocator);

/**
Set string S to a zero value. If it is a dynamically sized string,
deallocate the underlying data buffer
**/
STR_DEF void
str_destroy(Str* s, StrAllocator* allocator);

/**
Convert a string S to a standard null-terminated C string.

NOTE: must be deallocated by caller.
**/
STR_DEF STR_BYTE_T*
str_cstr(Str s, StrAllocator* allocator);

/**
Check if string S is a dynamically-sized string with memory allocated
to it.
**/
STR_DEF STR_BOOL_T
str_is_dynamic(Str s);

/**
Compare two strings. If returned value > 0, string A is greater than
string B. If returned value < 0, string A is less than string B. If
returned value == 0, string A is equal to string B.
**/
STR_DEF int
str_compare(Str a, Str b);

STR_DEF int
str_compare_cstr(Str a, STR_BYTE_T* b);

/**
Check if string A is equal to string B.
**/
STR_DEF STR_BOOL_T
str_is_equal(Str a, Str b);

STR_DEF STR_BOOL_T
str_is_equal_cstr(Str a, STR_BYTE_T* b);

/**
Check if string S is empty.
**/
STR_DEF STR_BOOL_T
str_is_zero(Str s);

/**
Get substring of string S of size len bytes, starting at BEGIN
(inclusive).
**/
STR_DEF Str
str_substr(Str s, STR_SIZE_T begin, STR_SIZE_T len);

/**
Trim space, indentation and newline characters from both ends of the
string.
**/
STR_DEF Str
str_trim_space(Str s);

/**
Trim space, indentation and newline characters at the front of the
string.
**/
STR_DEF Str
str_trim_space_front(Str s);

/**
Trim space, indentation and newline characters at the back of the
string.
**/
STR_DEF Str
str_trim_space_back(Str s);

/**
Remove prefix substring from string S.
**/
STR_DEF Str
str_trim_str_front(Str s, Str prefix);

STR_DEF Str
str_trim_cstr_front(Str s, STR_BYTE_T* prefix);

/**
Remove suffix substring from string S.
**/
STR_DEF Str
str_trim_str_back(Str s, Str suffix);

STR_DEF Str
str_trim_cstr_back(Str s, STR_BYTE_T* suffix);

/**
Get the index of the first substring A found in string S. Returns the
length of string S if substring A does not exist.
**/
STR_DEF STR_SIZE_T
str_find_first_str(Str s, Str a);

STR_DEF STR_SIZE_T
str_find_first_cstr(Str s, STR_BYTE_T* a);

/**
Get the index of the last substring A found in string S. Returns the
length of string S if substring A does not exist.
**/
STR_DEF STR_SIZE_T
str_find_last_str(Str s, Str a);

STR_DEF STR_SIZE_T
str_find_last_cstr(Str s, STR_BYTE_T* a);

/**
Get the index of the first (i.e. leftmost) byte C in string S, or the
len of the string S if byte C does not exist.
**/
STR_DEF STR_SIZE_T
str_find_first_byte(Str s, STR_BYTE_T c);

/**
Get the index of the last (i.e. rightmost) byte C in string S, or the
len of the string S if byte C does not exist.
**/
STR_DEF STR_SIZE_T
str_find_last_byte(Str s, STR_BYTE_T c);

/**
Append string A to string S.

NOTE: string S must be deallocated by caller using str_destroy.
**/
STR_DEF void
str_append(Str* s, Str a, StrAllocator* allocator);

STR_DEF void
str_append_cstr(Str* s, STR_BYTE_T* a, StrAllocator* allocator);

/**
Split string S by substring DELIM. If DELIM == str_zero(), an array
of bytes represented as  substrings of length 1 will be returned.

NOTE: must be deallocated by caller using str_array_destroy.
**/
STR_DEF StrArray
str_split_by_str(Str s, Str delim, StrAllocator* allocator);

STR_DEF StrArray
str_split_by_byte(Str s, STR_BYTE_T delim, StrAllocator* allocator);

/**
Reserve NEW_cap amount of bytes for the string S. If S is not a
dynamically-sized string, it will be converted to one.

NOTE: must be deallocated by caller using str_destroy.
**/
STR_DEF void
str_reserve_mem(Str* s, STR_SIZE_T new_cap, StrAllocator* allocator);

/**
Initialize an empty StrArray, representing its default zero value.
**/
STR_DEF StrArray
str_array_zero(void);

/**
Create a dynamically-sized array of Str objects. Returns an empty
StrArray on failure.

NOTE: must be deallocated by caller using str_array_destroy.
**/
STR_DEF StrArray
str_array_with_len(Str* data, STR_SIZE_T len, StrAllocator* allocator);

/**
Create a dynamically-sized array of Str objects, and reserved enough
space for CAP amount of Str objects. Returns an empty StrArray
on failure.

NOTE: must be deallocated by caller using str_array_destroy.
**/
STR_DEF StrArray
str_array_with_len_cap(Str* data, STR_SIZE_T len, STR_SIZE_T cap, StrAllocator* allocator);

/**
Deallocate a StrArray object.

NOTE: the individual Str elements in StrArray are not
deallocated by this function.
**/
STR_DEF void
str_array_destroy(StrArray* arr, StrAllocator* allocator);

/**
Deallocate a StrArray object and all Str elements.
**/
STR_DEF void
str_array_destroy_all(StrArray* arr, StrAllocator* allocator);

/**
Join file path str elements.

NOTE: must be deallocated by caller using str_destroy.
**/
STR_DEF Str
str_filepath_join(StrArray elems, StrAllocator* allocator);

/**
Get the last component of file name in PATH.
**/
STR_DEF Str
str_filepath_stem(Str path);

/**
Get the parent component of the path (i.e. exclude the last component of the file name)
**/
STR_DEF Str
str_filepath_parent(Str path);

STR_DEF void*
str__malloc(STR_SIZE_T len, void* ctx);

STR_DEF void*
str__realloc(void* ptr, STR_SIZE_T len_old, STR_SIZE_T len_new, void* ctx);

STR_DEF void
str__free(void* ptr, STR_SIZE_T len, void* ctx);

#ifdef __cplusplus
} /* extern "C" */
#endif /* #ifdef __cplusplus */

#endif /* #ifndef STR_H_ */

/** IMPLEMENTATION **/

#ifdef STR_IMPLEMENTATION
#undef STR_IMPLEMENTATION

StrAllocator str_std_allocator = { str__malloc, str__realloc, str__free, NULL };

STR_DEF void*
str__malloc(STR_SIZE_T len, void* ctx)
{
	(void)ctx;
	return malloc(len);
}

STR_DEF void*
str__realloc(void* ptr, STR_SIZE_T len_old, STR_SIZE_T len_new, void* ctx)
{
	(void)ctx;
	(void)len_old;
	return realloc(ptr, len_new);
}

STR_DEF void
str__free(void* ptr, STR_SIZE_T len, void* ctx)
{
	(void)ctx;
	(void)len;
	free(ptr);
}

STR_DEF STR_BOOL_T
str__byte_isspace(STR_BYTE_T c)
{
	return (c == ' ') || (c == '\r') || (c == '\n') || (c == '\t') || (c == '\f') || (c == '\v');
}

STR_DEF Str
str_zero(void)
{
	return str_with_len("", 0);
}

STR_DEF Str
str(STR_BYTE_T* s)
{
	STR_SIZE_T len = 0;

	while (s[len] != '\0') {
		len++;
	}

	return str_with_len(s, len);
}

STR_DEF Str
str_with_len(STR_BYTE_T* s, STR_SIZE_T len)
{
	Str result = { 0 };

	result.data = s;
	result.len = len;
	result.cap = 0;

	return result;
}

STR_DEF Str
str_dynamic(STR_BYTE_T* s, StrAllocator* allocator)
{
	STR_SIZE_T len = 0;

	while (s[len] != '\0') {
		len++;
	}

	return str_dynamic_with_len(s, len, allocator);
}

STR_DEF Str
str_dynamic_with_len(STR_BYTE_T* s, STR_SIZE_T len, StrAllocator* allocator)
{
	/* ensure cap at least larger than (len + 1) */
	STR_SIZE_T cap = ((STR_SIZE_T)(len * 1.5) > len) ? (STR_SIZE_T)(len * 1.5) : (len + 1);

	return str_dynamic_with_len_cap(s, len, cap, allocator);
}

STR_DEF Str
str_dynamic_with_len_cap(STR_BYTE_T* s, STR_SIZE_T len, STR_SIZE_T cap, StrAllocator* allocator)
{
	STR_BYTE_T* new_data;
	STR_SIZE_T i = 0;
	Str result = { 0 };

	STR_ASSERT(len < cap);
	STR_ASSERT(allocator);

	new_data = allocator->malloc(cap * sizeof(*new_data), allocator->ctx);
	if (new_data == NULL) {
	} /* TODO: handle alloc fail */

	for (i = 0; i < len; i++) {
		new_data[i] = s[i];
	}
	new_data[len] = '\0';

	result.data = new_data, result.len = len, result.cap = cap;

	return result;
}

STR_DEF void
str_destroy(Str* s, StrAllocator* allocator)
{
	STR_ASSERT(s != NULL);

	if (s->cap > 0) {
		STR_ASSERT(allocator);

		allocator->free(s->data, sizeof(*s->data) * s->cap, allocator->ctx);
	}
	*s = str_zero();
}

STR_DEF STR_BYTE_T*
str_cstr(Str s, StrAllocator* allocator)
{
	STR_BYTE_T* cstr;
	STR_SIZE_T i = 0;

	STR_ASSERT(allocator);

	cstr = allocator->malloc((s.len + 1) * sizeof(*cstr), allocator->ctx);
	if (cstr == NULL) {
		return NULL;
	}

	for (i = 0; i < s.len; i++) {
		cstr[i] = s.data[i];
	}
	cstr[s.len] = '\0';

	return cstr;
}

STR_DEF STR_BOOL_T
str_is_dynamic(Str s)
{
	return s.cap > 0;
}

STR_DEF int
str_compare(Str a, Str b)
{
	STR_SIZE_T min_len = (a.len < b.len) ? a.len : b.len;
	STR_SIZE_T i = 0;

	for (i = 0; i < min_len; i++) {
		if (a.data[i] != b.data[i]) {
			return a.data[i] - b.data[i];
		}
	}

	return (int)a.len - (int)b.len;
}

STR_DEF int
str_compare_cstr(Str a, STR_BYTE_T* b)
{
	return str_compare(a, str(b));
}

STR_DEF STR_BOOL_T
str_is_equal(Str a, Str b)
{
	return str_compare(a, b) == 0;
}

STR_DEF STR_BOOL_T
str_is_equal_cstr(Str a, STR_BYTE_T* b)
{
	return str_is_equal(a, str(b));
}

STR_DEF STR_BOOL_T
str_is_zero(Str s)
{
	return str_compare(s, str_zero()) == 0;
}

STR_DEF Str
str_substr(Str s, STR_SIZE_T begin, STR_SIZE_T len)
{
	STR_ASSERT(s.len >= (begin + len));

	return str_with_len(s.data + begin, len);
}

STR_DEF Str
str_trim_space(Str s)
{
	return str_trim_space_front(str_trim_space_back(s));
}

STR_DEF Str
str_trim_space_front(Str s)
{
	STR_SIZE_T i = 0;

	for (i = 0; i < s.len; i++) {
		if (!str__byte_isspace(s.data[i])) {
			break;
		}
	}

	return str_with_len(s.data + i, s.len - i);
}

STR_DEF Str
str_trim_space_back(Str s)
{
	STR_SIZE_T i = 0;

	for (i = 0; i < s.len; i++) {
		if (!str__byte_isspace(s.data[s.len - 1 - i])) {
			break;
		}
	}

	return str_with_len(s.data, s.len - i);
}

STR_DEF Str
str_trim_str_front(Str s, Str prefix)
{
	if (s.len < prefix.len) {
		return s;
	}

	if (str_is_equal(str_substr(s, 0, prefix.len), prefix)) {
		return str_substr(s, prefix.len, s.len - prefix.len);
	}

	return s;
}

STR_DEF Str
str_trim_cstr_front(Str s, STR_BYTE_T* prefix)
{
	return str_trim_str_front(s, str(prefix));
}

STR_DEF Str
str_trim_str_back(Str s, Str suffix)
{
	if (s.len < suffix.len) {
		return s;
	}

	if (str_is_equal(
			  str_substr(s, s.len - suffix.len, suffix.len),
			  suffix)) {
		return str_substr(s, 0, s.len - suffix.len);
	}

	return s;
}

STR_DEF Str
str_trim_cstr_back(Str s, STR_BYTE_T* suffix)
{
	return str_trim_str_back(s, str(suffix));
}

STR_DEF STR_SIZE_T
str_find_first_str(Str s, Str a)
{
	STR_SIZE_T i;

	if (s.len < a.len) {
		return s.len;
	}

	if (a.len == s.len) {
		if (str_is_equal(s, a)) {
			return 0;
		}
	}

	for (i = 0; i < s.len; i++) {
		if (s.data[i] != a.data[0]) {
			continue;
		}

		if ((i + a.len) > s.len) {
			return s.len;
		}

		if (str_is_equal(a, str_substr(s, i, a.len))) {
			return i;
		}
	}

	return s.len;
}

STR_DEF STR_SIZE_T
str_find_first_cstr(Str s, STR_BYTE_T* a)
{
	return str_find_first_str(s, str(a));
}

STR_DEF STR_SIZE_T
str_find_last_str(Str s, Str a)
{
	STR_SIZE_T i;

	if (s.len < a.len) {
		return s.len;
	}

	/* reverse for loop with unsigned `for (i = n; i-- > 0; ) => [n-1, ..., 0]` */
	for (i = s.len - a.len + 1; i-- > 0;) {
		if (s.data[i] != a.data[0]) {
			continue;
		}

		if (str_is_equal(a, str_substr(s, i, a.len))) {
			return i;
		}
	}

	return s.len;
}

STR_DEF STR_SIZE_T
str_find_last_cstr(Str s, STR_BYTE_T* a)
{
	return str_find_last_str(s, str(a));
}

STR_DEF STR_SIZE_T
str_find_first_byte(Str s, STR_BYTE_T c)
{
	STR_SIZE_T i = 0;

	for (i = 0; i < s.len; i++) {
		if (s.data[i] == c) {
			return i;
		}
	}

	return s.len;
}

STR_DEF STR_SIZE_T
str_find_last_byte(Str s, STR_BYTE_T c)
{
	STR_SIZE_T i = 0;

	if (s.len == 0) {
		return s.len;
	}

	for (i = s.len - 1; i > 0; i--) {
		if (s.data[i] == c) {
			return i;
		}
	}

	/* separate this from for loop because of warning since [unsigned] i >= 0 is
	 * always true */
	if (s.data[0] == c) {
		return 0;
	}

	return s.len;
}

STR_DEF void
str_append(Str* s, Str a, StrAllocator* allocator)
{
	STR_SIZE_T old_len = s->len;
	STR_SIZE_T new_len = s->len + a.len;

	/* don't realloc if there is enough space */
	STR_SIZE_T new_cap = (s->cap > new_len) ? (s->cap) : (STR_SIZE_T)(new_len * 1.5);
	STR_BYTE_T* new_data = s->data;

	STR_SIZE_T i = 0;

	STR_ASSERT(s != NULL);
	STR_ASSERT(allocator);

	if (str_is_zero(a)) {
		/* TODO: return success */
	}

	if (s->cap == 0) {
		new_data = allocator->malloc(sizeof(s->data) * new_cap, allocator->ctx);
		if (new_data == NULL) {
			/* TODO: handle alloc fail error */
		}

		for (i = 0; i < s->len; i++) {
			new_data[i] = s->data[i];
		}
	} else if (s->cap != new_cap) {
		new_data = allocator->realloc(s->data, sizeof(*s->data) * s->cap, sizeof(*s->data) * new_cap, allocator->ctx);
		if (new_data == NULL) {
			/* TODO: handle realloc fail error */
		}
	}

	s->data = new_data;
	s->len = new_len;
	s->cap = new_cap;

	for (i = old_len; i < new_len; i++) {
		s->data[i] = a.data[i - old_len];
	}
	s->data[new_len] = '\0';

	/* TODO: return success */
}

STR_DEF void
str_append_cstr(Str* s, STR_BYTE_T* a, StrAllocator* allocator)
{
	str_append(s, str(a), allocator);
}

STR_DEF StrArray
str_split_by_str(Str s, Str delim, StrAllocator* allocator)
{
	STR_SIZE_T num_delim;

	STR_SIZE_T i;
	STR_SIZE_T begin_i;
	StrArray parts;

	/* TODO: if delim == str_zero() return array of chars */

	num_delim = 0;
	for (i = 0; i < s.len; i++) {
		if ((i + delim.len) > s.len) {
			break;
		}

		if (str_is_equal(str_substr(s, i, delim.len), delim)) {
			num_delim++;
		}
	}

	parts.len = 0;
	parts.cap = num_delim + 1;
	parts.data = allocator->malloc(sizeof(*parts.data) * parts.cap, allocator->ctx);

	begin_i = 0;
	for (i = 0; i < s.len; i++) {
		if ((i + delim.len) > s.len) {
			break;
		}

		if (str_is_equal(str_substr(s, i, delim.len), delim)) {
			parts.data[parts.len] = str_substr(s, begin_i, i - begin_i);
			parts.len++;

			begin_i = i + 1;
		}
	}

	parts.data[parts.len] = str_substr(s, begin_i, s.len - begin_i);
	parts.len++;

	return parts;
}

STR_DEF StrArray
str_split_by_byte(Str s, STR_BYTE_T delim, StrAllocator* allocator)
{
	return str_split_by_str(s, str_with_len(&delim, 1), allocator);
}

STR_DEF void
str_reserve_mem(Str* s, STR_SIZE_T new_cap, StrAllocator* allocator)
{
	STR_BYTE_T* new_data;
	STR_SIZE_T i;

	STR_ASSERT(s != NULL);
	STR_ASSERT(new_cap >= s->len);

	if (s->cap > 0) {
		STR_ASSERT(new_cap >= s->cap);

		s->data = allocator->realloc(s->data, sizeof(*s->data) * s->cap, sizeof(*s->data) * new_cap, allocator->ctx);
		s->cap = new_cap;

		return;
	}

	new_data = allocator->malloc(new_cap * sizeof(*new_data), allocator->ctx);
	if (new_data == NULL) {
		/* TODO: handle alloc fail */
	}

	for (i = 0; i < s->len; i++) {
		new_data[i] = s->data[i];
	}

	if (s->len < s->cap) {
		new_data[s->len] = '\0';
	}

	s->cap = new_cap;
	s->data = new_data;
}

STR_DEF StrArray
str_array_zero(void)
{
	StrArray arr = { 0 };
	return arr;
}

STR_DEF StrArray
str_array_with_len(Str* data, STR_SIZE_T len, StrAllocator* allocator)
{
	return str_array_with_len_cap(data, len, len, allocator);
}

STR_DEF StrArray
str_array_with_len_cap(Str* data, STR_SIZE_T len, STR_SIZE_T cap, StrAllocator* allocator)
{
	StrArray arr = { 0 };
	Str* new_data = NULL;
	STR_SIZE_T new_cap = sizeof(*data) * cap;

	STR_SIZE_T i = 0;

	STR_ASSERT(data != NULL);

	if (len > 0) {
		new_data = allocator->malloc(new_cap, allocator->ctx);
		if (new_data != NULL) {
			return arr;
		}
	}

	for (i = 0; i < len; i++) {
		new_data[i] = data[i];
	}

	arr.data = new_data;
	arr.cap = new_cap;
	arr.len = len;

	return arr;
}

STR_DEF void
str_array_destroy(StrArray* arr, StrAllocator* allocator)
{
	STR_ASSERT(arr != NULL);

	if (arr->cap > 0) {
		STR_ASSERT(allocator);

		allocator->free(arr->data, sizeof(*arr->data) * arr->cap, allocator->ctx);
	}
	*arr = str_array_zero();
}

STR_DEF void
str_array_destroy_all(StrArray* arr, StrAllocator* allocator)
{
	STR_SIZE_T i;

	STR_ASSERT(arr != NULL);

	for (i = 0; i < arr->len; i++) {
		STR_ASSERT(allocator);

		str_destroy(&arr->data[i], allocator);
	}

	if (arr->cap > 0) {
		STR_ASSERT(allocator);

		allocator->free(arr->data, sizeof(*arr->data) * arr->cap, allocator->ctx);
	}
	*arr = str_array_zero();
}

STR_DEF Str
str_filepath_join(StrArray elems, StrAllocator* allocator)
{
	STR_SIZE_T i;
	Str result = str_zero();

	STR_ASSERT(elems.len > 0);

	for (i = 0; i < elems.len; i++) {
		if (i == 0) {
			/* TODO: handle alloc fail? */
			str_append(&result, elems.data[i], allocator);
			continue;
		}

		if (str_is_zero(elems.data[i])) {
			continue;
		}

		/* TODO: handle alloc fail? */
		str_append_cstr(&result, "/", allocator);
		str_append(
			&result,
			str_trim_cstr_front(str_trim_cstr_back(elems.data[i], "/"), "/"),
			allocator
		);
	}

	return result;
}

STR_DEF Str
str_filepath_stem(Str path)
{
	STR_SIZE_T i;

	path = str_trim_cstr_back(path, "/");
	if (str_find_first_byte(path, '/')) {
		return path;
	}

	i = str_find_last_byte(path, '/');
	if (i == path.len) {
		return str_trim_cstr_front(path, "/");
	}

	return str_trim_cstr_front(str_substr(path, i, path.len - i), "/");
}

STR_DEF Str
str_filepath_parent(Str path)
{
	Str stem = str_filepath_stem(path);

	if (!str_is_equal_cstr(path, "/")) {
		path = str_trim_cstr_back(path, "/");
	}
	path = str_trim_str_back(path, stem);

	return path;
}

#endif /* #ifdef STR_IMPLEMENTATION */

/* clang-format on */
