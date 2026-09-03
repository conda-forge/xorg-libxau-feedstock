#include <X11/Xauth.h>

#include <stdio.h>
#include <string.h>

static int
same_field(const char *left, unsigned short left_length,
           const char *right, unsigned short right_length)
{
    return left_length == right_length &&
           memcmp(left, right, left_length) == 0;
}

int
main(void)
{
    char address[] = "localhost";
    char number[] = "0";
    char name[] = "MIT-MAGIC-COOKIE-1";
    char data[] = "native-arm64";
    Xauth input = {
        FamilyLocal,
        (unsigned short) (sizeof(address) - 1), address,
        (unsigned short) (sizeof(number) - 1), number,
        (unsigned short) (sizeof(name) - 1), name,
        (unsigned short) (sizeof(data) - 1), data
    };
    Xauth *output;
    FILE *stream = tmpfile();
    int matches;

    if (stream == NULL)
        return 1;
    if (!XauWriteAuth(stream, &input)) {
        fclose(stream);
        return 2;
    }
    rewind(stream);
    output = XauReadAuth(stream);
    fclose(stream);
    if (output == NULL)
        return 3;

    matches = output->family == input.family &&
              same_field(output->address, output->address_length,
                         input.address, input.address_length) &&
              same_field(output->number, output->number_length,
                         input.number, input.number_length) &&
              same_field(output->name, output->name_length,
                         input.name, input.name_length) &&
              same_field(output->data, output->data_length,
                         input.data, input.data_length);
    XauDisposeAuth(output);

    return matches ? 0 : 4;
}
