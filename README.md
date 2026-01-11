# `str.h`

A single-file header-only (stb-style) implementation of a non-null-terminated `string` type that can act as both a dynamically-sized string and a string view (i.e. similar to C++ `std::string` and `std::string_view`).

- Implementation of C++ style `std::string` and `std::string_view`
- C89/C99/ANSI-C/C++ compatible library
- Single-file header-only [stb-style](https://github.com/nothings/stb) library
- Cross-platform and highly portable: no dependencies except C standard library

## Usage

Download `str.h` and include it. _By default, it only includes type/function declarations. If you want to include function definitions/implementations, define `STR_IMPLEMENTATION` BEFORE including `str.h`_.

### Example

```c
#define STR_IMPLEMENTATION
#include "str.h"

#include <stdio.h>

int
main(void)
{
	Str msg = str("\t\n       Hello, world!     \n       ");

	msg = str_trim_space(msg);

	msg = str_trim_str_front(msg, str(", world!"));
	printf("'%.*s'\n", STR_FMT(msg));
	msg = str_trim_str_back(msg, str("Hello"));
	printf("'%.*s'\n", STR_FMT(msg));
	msg = str_trim_str_front(msg, str("Hello"));
	printf("'%.*s'\n", STR_FMT(msg));
	msg = str_trim_str_back(msg, str(", world!"));
	printf("'%.*s'\n", STR_FMT(msg));
	msg = str_trim_str_front(msg, str("Hello"));
	printf("'%.*s'\n", STR_FMT(msg));
	msg = str_trim_str_back(msg, str(", world!"));
	printf("'%.*s'\n", STR_FMT(msg));

	{
		StrArray arr = str_split_by_byte(str("/wjfoi/ejwo"), '/', &str_std_allocator);
		for (usize i = 0; i < arr.len; i++) {
			printf("%zu: %.*s\n", i, STR_FMT(arr.data[i]));
		}

		{
			Str path = str_filepath_join(arr, &str_std_allocator);
			printf("str_filepath_join: %.*s\n", STR_FMT(path));

			Str stem = str_filepath_stem(path);
			printf("str_filepath_stem: %.*s\n", STR_FMT(stem));

			Str parent = str_filepath_parent(path);
			printf("str_filepath_parent: %.*s\n", STR_FMT(parent));

			str_destroy(&path, &str_std_allocator);
		}

		{
			Str path = str("/");
			printf("str: %.*s\n", STR_FMT(path));

			Str stem = str_filepath_stem(path);
			printf("str_filepath_stem: %.*s\n", STR_FMT(stem));

			Str parent = str_filepath_parent(path);
			printf("str_filepath_parent: %.*s\n", STR_FMT(parent));
		}

		printf("find first 1: %zu\n", str_find_first_cstr(str_zero(), "hello"));
		printf("find first 2: %zu\n", str_find_first_cstr(str("Hello world"), "Hello"));
		printf("find first 3: %zu\n", str_find_first_cstr(str("Hello world"), "world"));
		printf("find first 4: %zu\n", str_find_first_cstr(str("Hello world"), "jfwioj"));
		printf("find first 5: %zu\n", str_find_first_cstr(str("Hello world"), ""));

		printf("find last 1: %zu\n", str_find_last_cstr(str_zero(), ""));
		printf("find last 2: %zu\n", str_find_last_cstr(str("Hello world"), "Hello"));
		printf("find last 3: %zu\n", str_find_last_cstr(str("Hello world"), "world"));
		printf("find last 4: %zu\n", str_find_last_cstr(str("Hello world"), "weoupqr"));
		printf("find last 5: %zu\n", str_find_last_cstr(str("Hello world"), ""));
		printf("find last 6: %zu\n", str_find_last_cstr(str("Hello world"), "weoupqrewojoiwejfoiwej"));
		printf("find last 7: %zu\n", str_find_last_cstr(str("Hello world"), "o w"));

		str_array_destroy_all(&arr, &str_std_allocator);
	}

	return 0;
}
```
