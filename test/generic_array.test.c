#include <socomm/generic_array.h>

int main()
{
  const int     ARRAY_SIZE = 3;
  socomm_array *int_arr = socomm_array_create_reserve(sizeof(int), ARRAY_SIZE);

  socomm_array_destroy(&int_arr);
  return 0;
}
