/* Составить программу построчной фильтрации текста, вводимого пользователем. 
Суть фильтра — отбор строк, содержащих, как минимум, три различных символа. 
Фильтр должен быть реализован как функция, принимающая на вход указатель на 
вектор строк, их количество и указатель на результирующую структуру. На выход 
функция должна возвращать количество строк в результирующей структуре. 
Результат обработки выводится на экран. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_COUNT 32
#define LINE_LENGTH 10

#define BAD_ALLOC -1

size_t getlines(char*** textptr, size_t* scount, size_t* slen, FILE* stream) {
	size_t max_slen = 0;
	size_t real_slen = *slen;
	size_t fill = 0;
	ssize_t n = 0;
	while ((n = getline((*textptr + fill), &real_slen, stream)) != -1 && fill != BAD_ALLOC) {
		if (fill >= *scount - 1) {
			char** temp_textptr = (char**)realloc(*textptr, (*scount + 1) * 2 * sizeof(char*));
			if (!temp_textptr) {
				fill = BAD_ALLOC;
			}
			//free(*textptr);
			*textptr = temp_textptr;
			*scount = (*scount + 1) * 2;	
		}
		if (max_slen < real_slen) {
			max_slen = real_slen;
		}
		real_slen = *slen; 
		++fill;
	}
	*slen = max_slen;
	return fill;
}

size_t filter(char** v_str, const size_t str_count, char*** res_v_str) {
	size_t res_str_count = 0;
	char** tmp_res_v_str = *res_v_str;

	if (!v_str || !res_v_str){
		res_str_count = -1;
	}

	for (size_t i = 0; i < str_count && res_str_count + 1 > 0; ++i) {
		char c[3] = {'\0', '\0', '\0'};
		size_t size = 0;
		size = i == str_count - 1 ? strlen(v_str[i]) : strlen(v_str[i]) - 1;
		c[0] = v_str[i][0];
		size_t j = 1;
		while (!c[1] && j < size) {
			if (v_str[i][j] != c[0]) {
				c[1] = v_str[i][j];
			}
			++j;
		}
		size_t k = j;
		while (!c[2] && k < size) {
			if (v_str[i][k] != c[1]) {
				c[2] = v_str[i][k];
			}
			++k;
		}

		if (c[2]) {
			*tmp_res_v_str = (char*)realloc(v_str[i], (strlen(v_str[i]) + 1) * sizeof(char));
			++res_str_count;
			if (!(*tmp_res_v_str)) {
				res_str_count = BAD_ALLOC;
			}
			++tmp_res_v_str;
		}
	}

	return res_str_count;
}

int main() {
	int rcode = 0;
	size_t scount = LINE_COUNT;
	char** v_str = (char**)malloc(scount * sizeof(char*));
	if (!v_str) {
		rcode = BAD_ALLOC;
	}

	size_t slen = LINE_LENGTH;
	for (size_t i = 0; (i < scount) && (slen > 0); ++i) {
		v_str[i] = (char*)malloc(slen * sizeof(char));
		if (!v_str[i]) {
			rcode = BAD_ALLOC;
		}
	}

	size_t fillscount = getlines(&v_str, &scount, &slen, stdin);
	if (fillscount < 0) {
		rcode = BAD_ALLOC;
	};

	char** res_v_str = (char**)malloc(fillscount * sizeof(char*));
	if (!res_v_str) {
		rcode = BAD_ALLOC;
	}

	size_t res_str_count = filter(v_str, fillscount, &res_v_str);
	if (res_str_count < 0) {
		rcode = BAD_ALLOC;
	}

	if(!rcode) {
		printf("\n%ld\n", res_str_count);

		for (size_t i = 0; i < res_str_count; ++i) {
			printf("%s", res_v_str[i]);
		}
	}
	else {
		printf("\nBAD ALLOC\n");
	}

	for(size_t i = 0; i < scount; ++i)
		free(v_str[i]);
	free(v_str);

	for(size_t i = 0; i < res_str_count; ++i)
		free(res_v_str[i]);
	free(res_v_str);

	return rcode;
}