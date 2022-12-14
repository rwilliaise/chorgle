/**
 * Turns an input binary file into a C file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char FMT[] = "\nconst char %s_data[] = {%s};\n\nconst int %s_size = %llu;";

struct options {
	char *output;
	char *input;
};

int main(int argc, char *argv[]) {
	struct options opts = { NULL, NULL };
	char mode = ' ';
	for (int i = 1; i < argc; i++) {
		if (opts.input) { break; }
		char *arg = argv[i];
		switch (mode) {
		case 'o':
			opts.output = arg;
			mode = ' ';
			break;
		case ' ':
			if (arg[0] == '-') {
				if (arg[1] == 'o') {
					mode = 'o';
				} else if (arg[1] == '-') {
					mode = '-';
				}
				break;
			}
		case '-':
			opts.input = arg;
			break;
		}
	}
	if (!opts.input || !opts.output) {
		fprintf(stderr, "Usage: resbuild -o [OUTPUT] [FILE]");
		return 1;
	}

	FILE *fp = fopen(opts.input, "rb");

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	printf("size: %llu\n", size);
	fseek(fp, 0, SEEK_SET);

	char *buff = calloc(size * 5, 1);

	fread(&buff[size * 4], sizeof(char), size, fp);
	fclose(fp);

	size_t a;
	for (size_t i = size * 4; i < size * 5; i++) {
		a = i - size * 4;
		sprintf(&buff[a * 5], "0x%02X,", buff[i]);
	}
	buff[size * 5 - 1] = 0;

	FILE *wp = fopen(opts.output, "w");
	char *p = opts.input;

	for (; *p != 0; p++) {}
	for (; *p != '\\' && *p != '/'; p--) {
		if (*p == '.' || isspace(*p)) {
			*p = '_';
		}
	}
	p++;

	fprintf(wp, FMT, p, buff, p, size);
	fclose(wp);

	return 0;
}
